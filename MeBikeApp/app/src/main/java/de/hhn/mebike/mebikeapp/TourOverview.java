package de.hhn.mebike.mebikeapp;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;
import android.widget.Toast;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

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
        if(clientIdString == null || clientIdString.length() < 1 || tourId < 1){
            finish();
            return;
        }


        TextView title = findViewById(R.id.tourOverview);
        title.setText("Tour overview (tourId: "+tourId+")");

         time = findViewById(R.id.timeValue);
         avrgPulse = findViewById(R.id.avrgPulseValue);
         distance = findViewById(R.id.distanceOverviewValue);
         avrgSpeed = findViewById(R.id.averageSpeedValue);
         avrgRpm = findViewById(R.id.averageRpmValue);
         maxPitch = findViewById(R.id.maxPitchValue);

        init();
    }

    private void init(){

        NetworkManager.getInstance().get("/tourSummary?clientId=" + clientId, new NetworkResponse() {
            @Override
            public void onSuccess(JSONObject result) {
                try {
                    JSONArray summaries = result.getJSONArray("tourSummaries");
                    for (int i = 0; i < summaries.length(); i++) {
                        if(summaries.getJSONObject(i).getLong("tourId") == TourOverview.this.tourId){
                            summaryData = new TourSummaryData(summaries.getJSONObject(i));
                            break;  // Break out of loop
                        }
                    }
                    time.setText(""+summaryData.time);
                    avrgPulse.setText(""+summaryData.pulse);
                    distance.setText(""+summaryData.distance);
                    avrgSpeed.setText(""+summaryData.speed);
                    avrgRpm.setText(""+summaryData.rpm);
                    maxPitch.setText(""+summaryData.maxPitch);
                } catch (JSONException e) {
                    e.printStackTrace();
                }
            }

            @Override
            public void onError(Exception e) {
                Toast.makeText(getApplicationContext(), "Failed to get tour summary", Toast.LENGTH_LONG).show();
                Log.e("On Error", "Failed to get tour summary", e);
            }
        });
    }

    private class TourSummaryData{
        public long tourId;
        public float distance;
        public float time;
        public float pulse;
        public float maxPitch;
        public float rpm;
        public float speed;

        public  TourSummaryData(JSONObject data){
            try{
                tourId = data.getLong("tourId");
                distance = data.getLong("distance");
                time = data.getLong("time");
                pulse = data.getLong("pulse");
                maxPitch = data.getLong("maxPitch");
                rpm = data.getLong("rpm");
                speed = data.getLong("speed");
            }catch (Exception e){
                Log.e("Parse error", "Failed to parse JSON data into object", e);
                Toast.makeText(getApplicationContext(), "Failed to parse json", Toast.LENGTH_LONG).show();
            }
        }
    }
}