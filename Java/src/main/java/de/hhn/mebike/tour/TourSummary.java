package de.hhn.mebike.tour;

public class TourSummary {

    private long tourId;

    private float distance;
    private float pulseMedium;


    public long getTourId() {
        return tourId;
    }

    public void setTourId(long tourId) {
        this.tourId = tourId;
    }

    public float getDistance() {
        return distance;
    }

    public void setDistance(float distance) {
        this.distance = distance;
    }

    public float getPulseMedium() {
        return pulseMedium;
    }

    public void setPulseMedium(float pulseMedium) {
        this.pulseMedium = pulseMedium;
    }
}
