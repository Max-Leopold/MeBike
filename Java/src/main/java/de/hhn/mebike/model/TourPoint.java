package de.hhn.mebike.model;

import java.io.Serializable;

import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.Id;
import javax.persistence.ManyToOne;

@Entity
public class TourPoint implements Serializable {

    @Id
    @GeneratedValue
    private long tourPointId;

    private float pulse;

    @ManyToOne
    private Tour tour;

    private float pitch;

    private float speed;

    private float temperature;

    private long timestamp;

    private int rpm;

    private float forwardAccel;

    private float sideAccel;

    private float longitudeDegree;

    private float latitudeDegree;

    public TourPoint() {
    }

    public long getTourPointId() {
        return tourPointId;
    }

    public TourPoint setTourPointId(long tourPointId) {
        this.tourPointId = tourPointId;
        return this;
    }

    public float getPulse() {
        return pulse;
    }

    public TourPoint setPulse(float pulse) {
        this.pulse = pulse;
        return this;
    }

    public Tour getTour() {
        return tour;
    }

    public TourPoint setTour(Tour tour) {
        this.tour = tour;
        return this;
    }

    public float getPitch() {
        return pitch;
    }

    public TourPoint setPitch(float pitch) {
        this.pitch = pitch;
        return this;
    }

    public float getSpeed() {
        return speed;
    }

    public TourPoint setSpeed(float speed) {
        this.speed = speed;
        return this;
    }

    public float getTemperature() {
        return temperature;
    }

    public TourPoint setTemperature(float temperature) {
        this.temperature = temperature;
        return this;
    }

    public long getTimestamp() {
        return timestamp;
    }

    public TourPoint setTimestamp(long timestamp) {
        this.timestamp = timestamp;
        return this;
    }

    public int getRpm() {
        return rpm;
    }

    public TourPoint setRpm(int rpm) {
        this.rpm = rpm;
        return this;
    }

    public float getForwardAccel() {
        return forwardAccel;
    }

    public TourPoint setForwardAccel(float forwardAccel) {
        this.forwardAccel = forwardAccel;
        return this;
    }

    public float getSideAccel() {
        return sideAccel;
    }

    public TourPoint setSideAccel(float sideAccel) {
        this.sideAccel = sideAccel;
        return this;
    }

    public float getLongitudeDegree() {
        return longitudeDegree;
    }

    public TourPoint setLongitudeDegree(float longitudeDegree) {
        this.longitudeDegree = longitudeDegree;
        return this;
    }

    public float getLatitudeDegree() {
        return latitudeDegree;
    }

    public TourPoint setLatitudeDegree(float latitudeDegree) {
        this.latitudeDegree = latitudeDegree;
        return this;
    }
}
