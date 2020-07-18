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

import de.hhn.mebike.mebikeapp.util.NetworkManager;
import de.hhn.mebike.mebikeapp.util.NetworkResponse;

public class CommunicateActivity extends AppCompatActivity implements DataChangedListener {

    private TextView connectionText, speedText, distanceText, rpmText, pitchText, pulseText, temperatureText, calorieText, tripDurationText, gpsText;
    private Button connectButton, loginButton;




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

        // This method return false if there is an error, so if it does, we should close.
        if (!viewModel.setupViewModel(getIntent().getStringExtra("device_name"), getIntent().getStringExtra("device_mac"))) {
            finish();
            return;
        }

        loginButton.findViewById(R.id.loginBtn);
        loginButton.setOnClickListener(v -> NetworkManager.getInstance().post(null, "/client", new NetworkResponse() {
            @Override
            public void onSuccess(JSONObject result) {

            }

            @Override
            public void onError(Exception e) {

            }
        }));

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
        speedText.setText(""+data.getSpeed());
        pitchText.setText("" + data.getPitch());
        pulseText.setText("" + data.getPulse());
        rpmText.setText(""+ data.getRotationsPerMinute());
        gpsText.setText(""+ data.getLongitude() + "\n" + data.getLatitude());
        distanceText.setText(""+data.getTripDistance());
        pitchText.setText(""+data.getPitch()+"\n"+data.getAccelerationForeward()+"\n"+data.getAccelerationSideways());
    }

}
