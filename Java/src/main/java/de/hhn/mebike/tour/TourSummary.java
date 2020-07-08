package de.hhn.mebike.tour;

public class TourSummary {

    private long tourId;

    private float distance;
    private float pulseMedium;
    private int time;


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

    public int getTime() {
        return time;
    }

    public void setTime(int time) {
        this.time = time;
    }

    @Override
    public String toString() {
        return "TourSummary{" +
                "tourId=" + tourId +
                ", distance=" + distance +
                ", pulseMedium=" + pulseMedium +
                ", time=" + time +
                '}';
    }
}
