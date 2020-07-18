package de.hhn.mebike.mebikeapp;

import androidx.lifecycle.MutableLiveData;

import java.util.Calendar;
import java.util.concurrent.TimeUnit;

import de.hhn.mebike.mebikeapp.util.Haversine;

public class ArduinoData extends MutableLiveData {

    private int rotationsPerMinute = 0;
    private int pulse = 0;
    private int temperature = 0;
    private int pitch = 0;
    private float accelerationForeward = 0;
    private float accelerationSideways = 0;
    private  String gmtTime;
    private  double latitude = 0;
    private double longitude = 0;
    private double lastLatitude = 0;
    private double lastLongitude = 0;
    private double tripDistance = 0;
    private double speed = 0;
    private long lastLocationMillis = 0;

    public void parseMesssage(String message){
        message = message.replaceAll("<BOM>", "");
        message = message.replaceAll("<EOM>", "");
        String[] splitMessage = message.split("\\|");
        switch (splitMessage[0]) {
            case "gps":
                gmtTime = splitMessage[1];
                latitude = Double.parseDouble(splitMessage[2])/100f;
                longitude = Double.parseDouble(splitMessage[3])/100f;
                speed = calcSpeed();
                break;
            case "temp":
                temperature = Integer.parseInt(splitMessage[1]);
                break;
            case "pulse":
                pulse = Integer.parseInt(splitMessage[1]);
                break;
            case "gyro":
                pitch  = Integer.parseInt(splitMessage[1]);
                accelerationForeward = Float.parseFloat(splitMessage[2])/100f;
                accelerationSideways = Float.parseFloat(splitMessage[3])/100f;
                break;
            case "rpm":
                rotationsPerMinute = Integer.parseInt(splitMessage[1]);
                break;
        };
    }

    public int getRotationsPerMinute() {
        return rotationsPerMinute;
    }

    public int getPulse() {
        return pulse;
    }

    public int getTemperature() {
        return temperature;
    }

    public int getPitch() {
        return pitch;
    }

    public float getAccelerationForeward() {
        return accelerationForeward;
    }

    public float getAccelerationSideways() {
        return accelerationSideways;
    }

    public String getGmtTime() {
        return gmtTime;
    }

    public double getTripDistance() {
        return tripDistance;
    }

    public double getLongitude() {
        return longitude;
    }

    public double getLatitude() {
        return latitude;
    }

    public double getSpeed() {
        return speed;
    }

    public double calcSpeed() {
        long currentMillis = Calendar.getInstance().getTimeInMillis();
        if (lastLocationMillis == 0 || lastLatitude == 0) {
            lastLocationMillis = currentMillis;
            lastLatitude = latitude;
            lastLongitude = longitude;
            return 0;
        }
        float deltaTime = (currentMillis - lastLocationMillis) / 1000f;
        //long secondsPassed = TimeUnit.MILLISECONDS.toSeconds(currentMillis - lastLocationMillis);
        //double timePassed = (currentMillis - lastLocationMillis)/3600000f;
        double distanceInM = Haversine.HaversineInM(lastLatitude, lastLongitude, latitude, longitude);
        tripDistance += distanceInM;
        lastLongitude = longitude;
        lastLatitude = latitude;
        lastLocationMillis = currentMillis;
        return (distanceInM / deltaTime) * 3.6;
    }
}
