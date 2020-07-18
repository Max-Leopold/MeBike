package de.hhn.mebike.mebikeapp;

import android.os.Bundle;
import android.text.TextUtils;
import android.util.Log;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;
import androidx.lifecycle.ViewModelProviders;

import org.json.JSONObject;

import java.time.Instant;
import java.util.Calendar;

import de.hhn.mebike.mebikeapp.util.NetworkManager;
import de.hhn.mebike.mebikeapp.util.NetworkResponse;

public class CommunicateActivity extends AppCompatActivity implements DataChangedListener {

    private TextView connectionText, speedText, distanceText, rpmText, pitchText, pulseText,
            temperatureText, calorieText, tripDurationText, gpsText, tourID;
    private EditText clientId;
    private Button connectButton, loginButton, startTourButton;
    private long lastSentMillis = 0;



    private CommunicateViewModel viewModel;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        // Setup our activity
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main_display);
        // Enable the back button in the action bar if possible
        if (getSupportActionBar() != null) {
            getSupportActionBar().setDisplayHomeAsUpEnabled(true);
        }

        // Setup our ViewModel
        viewModel = ViewModelProviders.of(this).get(CommunicateViewModel.class);
        viewModel.setDataChangedListener(this);

        //get UI elements
        rpmText = findViewById(R.id.rpmValue);
        pulseText = findViewById(R.id.pulseValue);
        speedText = findViewById(R.id.speedValue);
        distanceText = findViewById(R.id.distanceValue);
        pitchText = findViewById(R.id.pitchValue);
        temperatureText = findViewById(R.id.temperatureValue);
        calorieText = findViewById(R.id.calorieValue);
        tripDurationText = findViewById(R.id.tripDurationValue);
        gpsText = findViewById(R.id.gpsValue);
        clientId = findViewById(R.id.clientID);
        tourID = findViewById(R.id.tourId);

        // This method return false if there is an error, so if it does, we should close.
        if (!viewModel.setupViewModel(getIntent().getStringExtra("device_name"), getIntent().getStringExtra("device_mac"))) {
            finish();
            return;
        }

        loginButton = findViewById(R.id.loginBtn);
        loginButton.setOnClickListener(v -> viewModel.login(clientId));

        startTourButton = findViewById(R.id.startTourBtn);
        startTourButton.setOnClickListener(v -> viewModel.startTour(tourID, clientId.getText().toString()));

        // Setup our Views
        connectionText = findViewById(R.id.communicate_connection_text);
        connectButton = findViewById(R.id.communicate_connect);

        // Start observing the data sent to us by the ViewModel
        viewModel.getConnectionStatus().observe(this, this::onConnectionStatus);
        viewModel.getDeviceName().observe(this, name -> setTitle(getString(R.string.device_name_format, name)));

    }

    // Called when the ViewModel updates us of our connectivity status
    private void onConnectionStatus(CommunicateViewModel.ConnectionStatus connectionStatus) {
        switch (connectionStatus) {
            case CONNECTED:
                connectionText.setText(R.string.status_connected);
                connectButton.setEnabled(true);
                connectButton.setText(R.string.disconnect);
                connectButton.setOnClickListener(v -> viewModel.disconnect());
                break;

            case CONNECTING:
                connectionText.setText(R.string.status_connecting);
                connectButton.setEnabled(false);
                connectButton.setText(R.string.connect);
                break;

            case DISCONNECTED:
                connectionText.setText(R.string.status_disconnected);
                connectButton.setEnabled(true);
                connectButton.setText(R.string.connect);
                connectButton.setOnClickListener(v -> viewModel.connect());
                break;
        }
    }

    // Called when a button in the action bar is pressed
    @Override
    public boolean onOptionsItemSelected(MenuItem item)
    {
        switch (item.getItemId()) {
            case android.R.id.home:
                // If the back button was pressed, handle it the normal way
                onBackPressed();
                return true;

            default:
                return super.onOptionsItemSelected(item);
        }
    }

    // Called when the user presses the back button
    @Override
    public void onBackPressed() {
        // Close the activity
        finish();
    }

    @Override
    public void update(ArduinoData data) {
        updateUI(data);
    }

    private void updateUI(ArduinoData data){
        //    private TextView connectionText, speedText, distanceText, rpmText, pitchText, pulseText, temperatureText, calorieText, tripDurationText, gpsText;
        temperatureText.setText(""+data.getTemperature());
        speedText.setText(""+Math.round( data.getSpeed()*100f)/100f);
        pitchText.setText("" + data.getPitch());
        pulseText.setText("" + data.getPulse());
        rpmText.setText(""+ data.getRotationsPerMinute());
        gpsText.setText(""+ data.getLongitude() + "\n" +data.getLatitude());
        distanceText.setText(""+Math.round(data.getTripDistance()*100f)/100f);
        pitchText.setText(""+data.getPitch()+"\n"+data.getAccelerationForeward()+"\n"+data.getAccelerationSideways());
        tripDurationText.setText(viewModel.getTourDuration());

        if((lastSentMillis+1000) <= Calendar.getInstance().getTimeInMillis() ){
            lastSentMillis = Calendar.getInstance().getTimeInMillis();

            JSONObject tourPointData = new JSONObject();
            JSONObject tourObject = new JSONObject();

            try {
                tourObject.put("id", tourID.getText().toString());
                tourPointData.put("tour", tourObject);
                tourPointData.put("pulse", data.getPulse());
                tourPointData.put("pitch", data.getPitch());
                tourPointData.put("speed", data.getSpeed());
                tourPointData.put("temperature", data.getTemperature());
                tourPointData.put("timestamp", Calendar.getInstance().getTimeInMillis());
                tourPointData.put("rpm", data.getRotationsPerMinute());
                tourPointData.put("forwardAccel", data.getAccelerationForeward());
                tourPointData.put("sideAccel", data.getAccelerationSideways());
                tourPointData.put("longitudeDegree", data.getLongitude());
                tourPointData.put("latitudeDegree", data.getLatitude());
                NetworkManager.getInstance().post(tourPointData, "/tourPoint", new NetworkResponse() {
                    @Override
                    public void onSuccess(JSONObject result) {
                        Log.d("Success", "Data successufully sent");
                    }

                    @Override
                    public void onError(Exception e) {
                        Log.e("Error", "Failed to send data to server", e);
                    }
                });
            }catch (Exception e){
                Log.e("JSON FAIL", "Failed to prepare json body", e);
            }
        }

    }

    /**
     *  {
     *         "tourPointId": 3,
     *         "pulse": 2,
     *         "tour": {
     *             "id": 2,
     *             "client": {
     *                 "clientId": 1
     *             }
     *         },
     *         "pitch": 2,
     *         "speed": 2,
     *         "temperature": 2,
     *         "timestamp": 2,
     *         "rpm": 2,
     *         "forwardAccel": 2,
     *         "sideAccel": 2,
     *         "longitudeDegree": 2,
     *         "latitudeDegree": 2
     *     }
     */

}
