package de.hhn.mebike.tour;

public class FlattenedTourEntry {

    private long tourId;
    private float pitch;
    private float pulse;
    private float speed;
    private float temperature;
    private float latitude;
    private float longitude;

    public FlattenedTourEntry(long tourId, float pitch, float pulse, float speed, float temperature, float latitude, float longitude) {
        this.tourId = tourId;
        this.pitch = pitch;
        this.pulse = pulse;
        this.speed = speed;
        this.temperature = temperature;
        this.latitude = latitude;
        this.longitude = longitude;
    }

    public long getTourId() {
        return tourId;
    }

    public void setTourId(long tourId) {
        this.tourId = tourId;
    }

    public float getPitch() {
        return pitch;
    }

    public void setPitch(float pitch) {
        this.pitch = pitch;
    }

    public float getPulse() {
        return pulse;
    }

    public void setPulse(float pulse) {
        this.pulse = pulse;
    }

    public float getSpeed() {
        return speed;
    }

    public void setSpeed(float speed) {
        this.speed = speed;
    }

    public float getTemperature() {
        return temperature;
    }

    public void setTemperature(float temperature) {
        this.temperature = temperature;
    }

    public float getLatitude() {
        return latitude;
    }

    public void setLatitude(float latitude) {
        this.latitude = latitude;
    }

    public float getLongitude() {
        return longitude;
    }

    public void setLongitude(float longitude) {
        this.longitude = longitude;
    }
}
