package de.hhn.mebike.mebikeapp;

import android.app.Application;
import android.content.res.Resources;
import android.os.Debug;
import android.os.Handler;
import android.os.Looper;
import android.text.Editable;
import android.text.InputType;
import android.text.TextUtils;
import android.util.Log;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import androidx.annotation.StringRes;
import androidx.lifecycle.AndroidViewModel;
import androidx.lifecycle.LiveData;
import androidx.lifecycle.MutableLiveData;

import com.harrysoft.androidbluetoothserial.BluetoothManager;
import com.harrysoft.androidbluetoothserial.SimpleBluetoothDeviceInterface;

import org.jetbrains.annotations.NotNull;
import org.jetbrains.annotations.Nullable;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;
import java.sql.Timestamp;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;
import java.util.Locale;

import de.hhn.mebike.mebikeapp.util.NetworkManager;
import de.hhn.mebike.mebikeapp.util.NetworkResponse;
import io.reactivex.android.schedulers.AndroidSchedulers;
import io.reactivex.disposables.CompositeDisposable;
import io.reactivex.schedulers.Schedulers;

public class CommunicateViewModel extends AndroidViewModel {

    // A CompositeDisposable that keeps track of all of our asynchronous tasks
    private CompositeDisposable compositeDisposable = new CompositeDisposable();

    // Our BluetoothManager!
    private BluetoothManager bluetoothManager;

    // Our Bluetooth Device! When disconnected it is null, so make sure we know that we need to deal with it potentially being null
    @Nullable
    private SimpleBluetoothDeviceInterface deviceInterface;

    // The messages feed that the activity sees
    private MutableLiveData<String> messagesData = new MutableLiveData<>();
    // The connection status that the activity sees
    private MutableLiveData<ConnectionStatus> connectionStatusData = new MutableLiveData<>();
    // The device name that the activity sees
    private MutableLiveData<String> deviceNameData = new MutableLiveData<>();
    // The message in the message box that the activity sees
    private MutableLiveData<String> messageData = new MutableLiveData<>();

    // Our modifiable record of the conversation
    private StringBuilder messages = new StringBuilder();

    // Our configuration
    private String deviceName;
    private String mac;

    // A variable to help us not double-connect
    private boolean connectionAttemptedOrMade = false;
    // A variable to help us not setup twice
    private boolean viewModelSetup = false;


    private ArduinoData arduinoData = new ArduinoData();
    private DataChangedListener dataChangedListener;
    private long tourStartDate = 0;
    private boolean connectedToServer = false;
    private CommunicateActivity communicateActivity;

    public void setDataChangedListener(DataChangedListener dataChangedListener) {
        this.dataChangedListener = dataChangedListener;
    }

    // Called by the system, this is just a constructor that matches AndroidViewModel.
    public CommunicateViewModel(@NotNull Application application) {
        super(application);
    }

    // Called in the activity's onCreate(). Checks if it has been called before, and if not, sets up the data.
    // Returns true if everything went okay, or false if there was an error and therefore the activity should finish.
    public boolean setupViewModel(String deviceName, String mac, CommunicateActivity communicateActivity) {
        this.communicateActivity = communicateActivity;
        // Check we haven't already been called
        if (!viewModelSetup) {
            viewModelSetup = true;

            // Setup our BluetoothManager
            bluetoothManager = BluetoothManager.getInstance();
            if (bluetoothManager == null) {
                // Bluetooth unavailable on this device :( tell the user
                toast(R.string.bluetooth_unavailable);
                // Tell the activity there was an error and to close
                return false;
            }

            // Remember the configuration
            this.deviceName = deviceName;
            this.mac = mac;

            // Tell the activity the device name so it can set the title
            deviceNameData.postValue(deviceName);
            // Tell the activity we are disconnected.
            connectionStatusData.postValue(ConnectionStatus.DISCONNECTED);
        }
        // If we got this far, nothing went wrong, so return true
        return true;
    }

    // Called when the user presses the connect button
    public void connect() {
        // Check we are not already connecting or connected
        if (!connectionAttemptedOrMade) {
            // Connect asynchronously
            compositeDisposable.add(bluetoothManager.openSerialDevice(mac)
                    .subscribeOn(Schedulers.io())
                    .observeOn(AndroidSchedulers.mainThread())
                    .subscribe(device -> onConnected(device.toSimpleDeviceInterface()), t -> {
                        toast(R.string.connection_failed);
                        connectionAttemptedOrMade = false;
                        connectionStatusData.postValue(ConnectionStatus.DISCONNECTED);
                    }));
            // Remember that we made a connection attempt.
            connectionAttemptedOrMade = true;
            // Tell the activity that we are connecting.
            connectionStatusData.postValue(ConnectionStatus.CONNECTING);
        }
    }

    // Called when the user presses the disconnect button
    public void disconnect() {
        // Check we were connected
        if (connectionAttemptedOrMade && deviceInterface != null) {
            connectionAttemptedOrMade = false;
            // Use the library to close the connection
            bluetoothManager.closeDevice(deviceInterface);
            // Set it to null so no one tries to use it
            deviceInterface = null;
            // Tell the activity we are disconnected
            connectionStatusData.postValue(ConnectionStatus.DISCONNECTED);
        }
    }

    // Called once the library connects a bluetooth device
    private void onConnected(SimpleBluetoothDeviceInterface deviceInterface) {
        this.deviceInterface = deviceInterface;
        if (this.deviceInterface != null) {
            // We have a device! Tell the activity we are connected.
            connectionStatusData.postValue(ConnectionStatus.CONNECTED);
            // Setup the listeners for the interface
            this.deviceInterface.setListeners(this::onMessageReceived, this::onMessageSent, t -> Log.e("connection error", String.valueOf(t), t));
            // Tell the user we are connected.
            toast(R.string.connected);
            // Reset the conversation
            messages = new StringBuilder();
            messagesData.postValue(messages.toString());
        } else {
            // deviceInterface was null, so the connection failed
            toast(R.string.connection_failed);
            connectionStatusData.postValue(ConnectionStatus.DISCONNECTED);
        }
    }

    // Adds a received message to the conversation
    private void onMessageReceived(String message) {
        Log.d("mess", message);
        arduinoData.parseMesssage(message);
        dataChangedListener.update(arduinoData);
    }

    // Adds a sent message to the conversation
    private void onMessageSent(String message) {
        // Add it to the conversation
        messages.append(getApplication().getString(R.string.you_sent)).append(": ").append(message).append('\n');
        messagesData.postValue(messages.toString());
        // Reset the message box
        messageData.postValue("");
    }

    // Send a message
    public void sendMessage(String message) {
        // Check we have a connected device and the message is not empty, then send the message
        if (deviceInterface != null && !TextUtils.isEmpty(message)) {
            deviceInterface.sendMessage(message);
        }
    }

    // Called when the activity finishes - clear up after ourselves.
    @Override
    protected void onCleared() {
        // Dispose any asynchronous operations that are running
        compositeDisposable.dispose();
        // Shutdown bluetooth connections
        bluetoothManager.close();
    }

    // Helper method to create toast messages.
    private void toast(@StringRes int messageResource) {
        Toast.makeText(getApplication(), messageResource, Toast.LENGTH_LONG).show();
    }

    // Getter method for the activity to use.
    public LiveData<String> getMessages() {
        return messagesData;
    }

    // Getter method for the activity to use.
    public LiveData<ConnectionStatus> getConnectionStatus() {
        return connectionStatusData;
    }

    // Getter method for the activity to use.
    public LiveData<String> getDeviceName() {
        return deviceNameData;
    }

    // Getter method for the activity to use.
    public LiveData<String> getMessage() {
        return messageData;
    }

    public void login(EditText clientId) {
        Log.d("Login start", clientId.getText().toString());
        if (clientId.getText().toString().length() < 1) {
            NetworkManager.getInstance().post(null, "/client", new NetworkResponse() {
                @Override
                public void onSuccess(JSONObject result) {
                    try {
                        clientId.setText("" + result.getLong("clientId"));
                        clientId.setInputType(InputType.TYPE_NULL);
                        connectedToServer = true;
                    } catch (JSONException e) {
                        Log.e("JSON Parse failed", "Error", e);
                    }
                }

                @Override
                public void onError(Exception e) {
                    Log.e("login", "Login failed", e);

                }
            });
        } else {
            connectedToServer = true;
            /*
            NetworkManager.getInstance().get("/client?id=" + clientId.getText().toString(), new NetworkResponse() {
                @Override
                public void onSuccess(JSONObject result) {
                    clientId.setInputType(InputType.TYPE_NULL);
                }

                @Override
                public void onError(Exception e) {
                    Log.e("login", "Login failed", e);
                }
            });
            */
        }

    }

    public void startTour(String clientID) {
        if (connectedToServer) {
            tourStartDate = Calendar.getInstance().getTimeInMillis();
            NetworkManager.getInstance().post(null, "/tour/start?clientId=" + clientID, new NetworkResponse() {
                @Override
                public void onSuccess(JSONObject result) {
                    try {
                        communicateActivity.setTourId(result.getLong("id"));
                    } catch (JSONException e) {
                        Log.e("Start", "Start tour failed", e);
                    }
                }

                @Override
                public void onError(Exception e) {
                    Log.e("Failed to start", "Failed to start tour", e);
                }
            });
        }

    }

    public String getTourDuration() {
        long tourDurationMillis = Calendar.getInstance().getTimeInMillis() - tourStartDate - (1000 * 60 * 60);
        return new SimpleDateFormat("HH:mm:ss", Locale.GERMANY).format(new Timestamp(tourDurationMillis));
    }


    // An enum that is passed to the activity to indicate the current connection status
    enum ConnectionStatus {
        DISCONNECTED,
        CONNECTING,
        CONNECTED
    }

}
