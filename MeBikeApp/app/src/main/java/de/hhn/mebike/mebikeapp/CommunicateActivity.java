package de.hhn.mebike.mebikeapp;

import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.MenuItem;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;
import androidx.lifecycle.ViewModelProviders;

import org.json.JSONObject;

import java.util.Calendar;

import de.hhn.mebike.mebikeapp.util.NetworkManager;
import de.hhn.mebike.mebikeapp.util.NetworkResponse;

public class CommunicateActivity extends AppCompatActivity implements DataChangedListener {

    private TextView connectionText, speedText, distanceText, rpmText, pitchText, pulseText,
            temperatureText, calorieText, tripDurationText, gpsText;
    private EditText clientId;
    private Button connectButton, loginButton, startTourButton;
    private long lastSentMillis = 0;
    private boolean tourStarted = false;


    private CommunicateViewModel viewModel;
    private long tourId;

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
        tripDurationText = findViewById(R.id.tripTimeValue);
        gpsText = findViewById(R.id.gpsValue);
        clientId = findViewById(R.id.clientID);

        // This method return false if there is an error, so if it does, we should close.
        if (!viewModel.setupViewModel(getIntent().getStringExtra("device_name"), getIntent().getStringExtra("device_mac"), this)) {
            finish();
            return;
        }


        loginButton = findViewById(R.id.loginBtn);
        loginButton.setOnClickListener(v -> viewModel.login(clientId));

        startTourButton = findViewById(R.id.startTourBtn);
        startTourButton.setOnClickListener(v -> {
                    if (!tourStarted) {
                        viewModel.startTour(clientId.getText().toString());
                        startTourButton.setText(R.string.stopBtn);
                        tourStarted = true;
                    } else {
                        tourStarted = false;
                        startTourButton.setText(R.string.startBtn);
                        Intent intent = new Intent(getApplicationContext(), TourOverview.class);
                        intent.putExtra("clientId", clientId.getText().toString());
                        intent.putExtra("tourId", tourId);
                        startActivity(intent);
                    }
                }
        );

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
    public boolean onOptionsItemSelected(MenuItem item) {
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

    public void setTourId(long tourId) {
        this.tourId = tourId;
    }

    private void updateUI(ArduinoData data) {
        //    private TextView connectionText, speedText, distanceText, rpmText, pitchText, pulseText, temperatureText, calorieText, tripDurationText, gpsText;
        temperatureText.setText("" + data.getTemperature() + " °C");
        speedText.setText("" + Math.round(data.getSpeed() * 100f) / 100f + " km/h");
        pulseText.setText("" + data.getPulse()+" b/m");
        rpmText.setText("" + data.getRotationsPerMinute()+" r/m");
        String longitudeText = "" + data.getLongitude();
        String latitudeText = "" + data.getLatitude();
        if(longitudeText.length() > 8){
            longitudeText = longitudeText.substring(0, 8);
        }
        if(latitudeText.length() > 8){
            latitudeText = latitudeText.substring(0, 8);
        }
        gpsText.setText(longitudeText +"°" + "\n" + latitudeText+"°");
        distanceText.setText("" + Math.round(data.getTripDistance() * 100f) / 100f + " m");
        pitchText.setText("Pitch: " + data.getPitch() + "°\n Frw Acc: " + data.getAccelerationForeward() + " G\nSd Acc: " + data.getAccelerationSideways()+ " G");
        if(tourStarted)
            tripDurationText.setText(viewModel.getTourDuration());
        calorieText.setText(""+data.getScore());

        if ((lastSentMillis + 1000) <= Calendar.getInstance().getTimeInMillis() && tourStarted) {
            lastSentMillis = Calendar.getInstance().getTimeInMillis();

            JSONObject tourPointData = new JSONObject();
            JSONObject tourObject = new JSONObject();

            try {
                tourObject.put("id", tourId);
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
                Log.d("TourPoint SENDING:", tourPointData.toString());
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
            } catch (Exception e) {
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
