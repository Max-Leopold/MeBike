package de.hhn.mebike.model;

import java.io.Serializable;

import javax.persistence.CascadeType;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.Id;
import javax.persistence.OneToOne;

@Entity
public class TourPoint implements Serializable {

    @Id
    @GeneratedValue
    private long tourPointId;

    private float pulse;

    @OneToOne(cascade = CascadeType.ALL)
    private GPSPoint gpsPoint;

    private float pitch;

    private float speed;

    private float temperature;

    public TourPoint() {
    }

    public long getTourPointId() {
        return tourPointId;
    }

    public void setTourPointId(long tourPointId) {
        this.tourPointId = tourPointId;
    }

    public float getPulse() {
        return pulse;
    }

    public void setPulse(float pulse) {
        this.pulse = pulse;
    }

    public GPSPoint getGpsPoint() {
        return gpsPoint;
    }

    public void setGpsPoint(GPSPoint gpsPoint) {
        this.gpsPoint = gpsPoint;
    }

    public float getPitch() {
        return pitch;
    }

    public void setPitch(float pitch) {
        this.pitch = pitch;
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
}
