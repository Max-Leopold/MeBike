package de.hhn.mebike.tour;

public class FlattenedTourEntry {

    private long tourId;
    private float pitch;
    private float pulse;
    private float speed;
    private float temperature;
    private float latitude;
    private float longitude;
    private long timestamp;
    private int rpm;

    public FlattenedTourEntry(long tourId, float pitch, float pulse, float speed, float temperature, float latitude, float longitude, long timestamp, int rpm) {
        this.tourId = tourId;
        this.pitch = pitch;
        this.pulse = pulse;
        this.speed = speed;
        this.temperature = temperature;
        this.latitude = latitude;
        this.longitude = longitude;
        this.timestamp = timestamp;
        this.rpm = rpm;
    }

    public long getTourId() {
        return tourId;
    }

    public FlattenedTourEntry setTourId(long tourId) {
        this.tourId = tourId;
        return this;
    }

    public float getPitch() {
        return pitch;
    }

    public FlattenedTourEntry setPitch(float pitch) {
        this.pitch = pitch;
        return this;
    }

    public float getPulse() {
        return pulse;
    }

    public FlattenedTourEntry setPulse(float pulse) {
        this.pulse = pulse;
        return this;
    }

    public float getSpeed() {
        return speed;
    }

    public FlattenedTourEntry setSpeed(float speed) {
        this.speed = speed;
        return this;
    }

    public float getTemperature() {
        return temperature;
    }

    public FlattenedTourEntry setTemperature(float temperature) {
        this.temperature = temperature;
        return this;
    }

    public float getLatitude() {
        return latitude;
    }

    public FlattenedTourEntry setLatitude(float latitude) {
        this.latitude = latitude;
        return this;
    }

    public float getLongitude() {
        return longitude;
    }

    public FlattenedTourEntry setLongitude(float longitude) {
        this.longitude = longitude;
        return this;
    }

    public long getTimestamp() {
        return timestamp;
    }

    public FlattenedTourEntry setTimestamp(long timestamp) {
        this.timestamp = timestamp;
        return this;
    }

    public int getRpm() {
        return rpm;
    }

    public FlattenedTourEntry setRpm(int rpm) {
        this.rpm = rpm;
        return this;
    }
}
