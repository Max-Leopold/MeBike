package de.hhn.mebike.mebikeapp;

import androidx.lifecycle.MutableLiveData;

public class ArduinoData extends MutableLiveData {

    private int rotationsPerMinute = 0;
    private int pulse = 0;
    private int temperature = 0;
    private int pitch = 0;
    private float accelerationForeward = 0;
    private float accelerationSideways = 0;
    private  String gmtTime;
    private  float latitude = 0;
    private float longitude = 0;

    public void parseMesssage(String message){
        message = message.replaceAll("<BOM>", "");
        message = message.replaceAll("<EOM>", "");
        String[] splitMessage = message.split("\\|");
        switch (splitMessage[0]) {
            case "gps":
                gmtTime = splitMessage[1];
                latitude = Float.parseFloat(splitMessage[2]);
                longitude = Float.parseFloat(splitMessage[2]);
                break;
            case "temp":
                temperature = Integer.parseInt(splitMessage[1]);
                break;
            case "pulse":
                pulse = Integer.parseInt(splitMessage[1]);
                break;
            case "gyro":
                pitch  = Integer.parseInt(splitMessage[1]);
                accelerationForeward = Float.parseFloat(splitMessage[2]);
                accelerationSideways = Float.parseFloat(splitMessage[3]);
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

    public float getLongitude() {
        return longitude;
    }

    public float getLatitude() {
        return latitude;
    }
}
