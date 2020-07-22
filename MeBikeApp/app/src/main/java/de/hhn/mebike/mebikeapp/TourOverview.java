package de.hhn.mebike.mebikeapp;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.widget.TextView;

import org.w3c.dom.Text;

public class TourOverview extends AppCompatActivity {

    private int clientId;
    private TextView clientIdTextView;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_tour_overview);
        if (getSupportActionBar() != null) {
            getSupportActionBar().setDisplayHomeAsUpEnabled(true);
        }

        String clientIdString = getIntent().getStringExtra("clientId");
        if(clientIdString.length()<1){
            finish();
            return;
        } else {
            clientId = Integer.parseInt(clientIdString);
        }

        clientIdTextView.setText(""+clientId);
    }
}