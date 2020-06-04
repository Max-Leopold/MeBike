package de.hhn.mebike.model;

import java.io.Serializable;
import java.util.Objects;

import javax.persistence.Entity;
import javax.persistence.Id;
import javax.persistence.OneToOne;
import javax.persistence.Table;

@Entity
public class GPSPoint implements Serializable {

    @Id
    private float longitudeDegree;
    @Id
    private int longitudeMinutes;
    @Id
    private int longitudeSeconds;

    @Id
    private float latitudeDegree;
    @Id
    private int latitudeMinutes;
    @Id
    private int latitudeSeconds;

    public GPSPoint() {
    }

    public float getLongitudeDegree() {
        return longitudeDegree;
    }

    public void setLongitudeDegree(float longitudeDegree) {
        this.longitudeDegree = longitudeDegree;
    }

    public int getLongitudeMinutes() {
        return longitudeMinutes;
    }

    public void setLongitudeMinutes(int longitudeMinutes) {
        this.longitudeMinutes = longitudeMinutes;
    }

    public int getLongitudeSeconds() {
        return longitudeSeconds;
    }

    public void setLongitudeSeconds(int longitudeSeconds) {
        this.longitudeSeconds = longitudeSeconds;
    }

    public float getLatitudeDegree() {
        return latitudeDegree;
    }

    public void setLatitudeDegree(float latitudeDegree) {
        this.latitudeDegree = latitudeDegree;
    }

    public int getLatitudeMinutes() {
        return latitudeMinutes;
    }

    public void setLatitudeMinutes(int latitudeMinutes) {
        this.latitudeMinutes = latitudeMinutes;
    }

    public int getLatitudeSeconds() {
        return latitudeSeconds;
    }

    public void setLatitudeSeconds(int latitudeSeconds) {
        this.latitudeSeconds = latitudeSeconds;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (!(o instanceof GPSPoint)) return false;
        GPSPoint gpsPoint = (GPSPoint) o;
        return Float.compare(gpsPoint.longitudeDegree, longitudeDegree) == 0 &&
                longitudeMinutes == gpsPoint.longitudeMinutes &&
                longitudeSeconds == gpsPoint.longitudeSeconds &&
                Float.compare(gpsPoint.latitudeDegree, latitudeDegree) == 0 &&
                latitudeMinutes == gpsPoint.latitudeMinutes &&
                latitudeSeconds == gpsPoint.latitudeSeconds;
    }

    @Override
    public int hashCode() {
        return Objects.hash(longitudeDegree, longitudeMinutes, longitudeSeconds, latitudeDegree, latitudeMinutes, latitudeSeconds);
    }
}
