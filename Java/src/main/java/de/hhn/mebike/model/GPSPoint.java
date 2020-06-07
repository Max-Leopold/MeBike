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

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (!(o instanceof GPSPoint)) return false;
        GPSPoint gpsPoint = (GPSPoint) o;
        return Float.compare(gpsPoint.getLongitudeDegree(), getLongitudeDegree()) == 0 &&
                Float.compare(gpsPoint.getLatitudeDegree(), getLatitudeDegree()) == 0;
    }

    @Override
    public int hashCode() {
        return Objects.hash(getLongitudeDegree(), getLatitudeDegree());
    }
}
