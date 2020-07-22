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


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_tour_overview);
        if (getSupportActionBar() != null) {
            getSupportActionBar().setDisplayHomeAsUpEnabled(true);
        }


        String clientIdString = getIntent().getStringExtra("clientId");
        String tourId = getIntent().getStringExtra("tourId");
        if(clientIdString == null || clientIdString.length() < 1 || tourId == null || tourId.length() < 1){
            finish();
            return;
        }
        clientId = Integer.parseInt(clientIdString);
        this.tourId = Long.parseLong(tourId);
        clientIdTextView.setText(""+clientId);

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
                    // TODO Set all the labels here

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