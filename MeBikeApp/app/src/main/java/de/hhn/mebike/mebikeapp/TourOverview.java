package de.hhn.mebike.mebikeapp;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.util.Log;
import android.widget.TextView;
import android.widget.Toast;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.sql.Timestamp;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Locale;

import de.hhn.mebike.mebikeapp.util.NetworkManager;
import de.hhn.mebike.mebikeapp.util.NetworkResponse;

public class TourOverview extends AppCompatActivity {

    private int clientId;
    private TextView clientIdTextView;
    private long tourId;
    private TourSummaryData summaryData;

    private TextView title, time, avrgPulse, distance, avrgSpeed, avrgRpm, maxPitch;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_tour_overview);
        if (getSupportActionBar() != null) {
            getSupportActionBar().setDisplayHomeAsUpEnabled(true);
        }


        String clientIdString = getIntent().getStringExtra("clientId");
        tourId = getIntent().getLongExtra("tourId", 0);
        if (clientIdString == null || clientIdString.length() < 1 || tourId < 1) {
            Log.e("Finishing", "FInishing");
            finish();
            return;
        }

        clientId = Integer.parseInt(clientIdString);
        TextView title = findViewById(R.id.tourOverview);
        title.setText("Tour overview (tourId: " + tourId + ")");

        time = findViewById(R.id.timeValue);
        avrgPulse = findViewById(R.id.avrgPulseValue);
        distance = findViewById(R.id.distanceOverviewValue);
        avrgSpeed = findViewById(R.id.averageSpeedValue);
        avrgRpm = findViewById(R.id.averageRpmValue);
        maxPitch = findViewById(R.id.maxPitchValue);

        init();
    }

    private void init() {

        NetworkManager.getInstance().get("/tourSummary?clientId=" + clientId, new NetworkResponse() {
            @Override
            public void onSuccess(JSONObject result) {
                try {
                    JSONArray summaries = result.getJSONArray("tourSummaries");
                    for (int i = 0; i < summaries.length(); i++) {
                        if (summaries.getJSONObject(i).getLong("tourId") == 108) {
                            summaryData = new TourSummaryData(summaries.getJSONObject(i));
                            break;  // Break out of loop
                        }
                    }
                    if(summaryData == null){
                        Log.e("Finishing 2", "FInishing 2");
                        finish();
                        return;
                    }
                    new Handler(Looper.getMainLooper()).post(() -> {
                        SimpleDateFormat formatter = new SimpleDateFormat("HH:mm:ss");
                        Calendar calendar = Calendar.getInstance();
                        calendar.setTimeInMillis(summaryData.time);
                        time.setText(formatter.format(calendar.getTime()));

                        //time.setText("" + summaryData.time);
                        avrgPulse.setText("" + Math.round(summaryData.pulse * 100f) / 100f);
                        distance.setText("" + Math.round(summaryData.distance * 100f) / 100f);
                        avrgSpeed.setText("" + Math.round(summaryData.speed * 100f) / 100f);
                        avrgRpm.setText("" + summaryData.rpm);
                        maxPitch.setText("" + summaryData.maxPitch);
                    });
                } catch (JSONException e) {
                    e.printStackTrace();
                }
            }

            @Override
            public void onError(Exception e) {
                Log.e("On Error", "Failed to get tour summary", e);
            }
        });
    }

    private class TourSummaryData {
        public long tourId;
        public double distance;
        public int time;
        public double pulse;
        public double maxPitch;
        public double rpm;
        public double speed;

        public TourSummaryData(JSONObject data) {
            try {
                tourId = data.getLong("tourId");
                distance = data.getDouble("distance");
                time = data.getInt("time");
                pulse = data.getDouble("pulseMedium");
                maxPitch = data.getDouble("maxPitch");
                rpm = data.getDouble("rpmMedium");
                speed = data.getDouble("speedMedium");
            } catch (Exception e) {
                Log.e("Parse error", "Failed to parse JSON data into object", e);
                Toast.makeText(getApplicationContext(), "Failed to parse json", Toast.LENGTH_LONG).show();
            }
        }
    }
}