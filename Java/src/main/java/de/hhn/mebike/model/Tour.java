package de.hhn.mebike.model;

import java.io.Serializable;
import java.util.List;

import javax.persistence.CascadeType;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.Id;
import javax.persistence.OneToMany;


@Entity
public class Tour implements Serializable {

    @Id
    @GeneratedValue
    private long id;

    @OneToMany(cascade = CascadeType.ALL)
    private List<TourPoint> tourPoints;

    public Tour() {
    }

    public long getId() {
        return id;
    }

    public void setId(long id) {
        this.id = id;
    }

    public List<TourPoint> getTourPoints() {
        return tourPoints;
    }

    public void setTourPoints(List<TourPoint> tourPoints) {
        this.tourPoints = tourPoints;
    }
}
