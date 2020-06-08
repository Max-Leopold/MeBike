package de.hhn.mebike.model;

import java.io.Serializable;

import javax.persistence.Entity;
import javax.persistence.Id;

@Entity
public class GPSPoint implements Serializable {

    @Id
    private float longitudeDegree;

    @Id
    private float latitudeDegree;

    public GPSPoint() {
    }

    public float getLongitudeDegree() {
        return longitudeDegree;
    }

    public void setLongitudeDegree(float longitudeDegree) {
        this.longitudeDegree = longitudeDegree;
    }

    public float getLatitudeDegree() {
        return latitudeDegree;
    }

    public void setLatitudeDegree(float latitudeDegree) {
        this.latitudeDegree = latitudeDegree;
    }
}
