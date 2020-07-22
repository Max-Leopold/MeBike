package de.hhn.mebike.tour;

public class TourSummary {

    private long tourId;

    private float distance;
    private float pulseMedium;
    private int time;
    private float speedMedium;
    private float rpmMedium;
    private float maxPitch;

    public long getTourId() {
        return tourId;
    }

    public TourSummary setTourId(long tourId) {
        this.tourId = tourId;
        return this;
    }

    public float getDistance() {
        return distance;
    }

    public TourSummary setDistance(float distance) {
        this.distance = distance;
        return this;
    }

    public float getPulseMedium() {
        return pulseMedium;
    }

    public TourSummary setPulseMedium(float pulseMedium) {
        this.pulseMedium = pulseMedium;
        return this;
    }

    public int getTime() {
        return time;
    }

    public TourSummary setTime(int time) {
        this.time = time;
        return this;
    }

    public float getSpeedMedium() {
        return speedMedium;
    }

    public TourSummary setSpeedMedium(float speedMedium) {
        this.speedMedium = speedMedium;
        return this;
    }

    public float getRpmMedium() {
        return rpmMedium;
    }

    public TourSummary setRpmMedium(float rpmMedium) {
        this.rpmMedium = rpmMedium;
        return this;
    }

    public float getMaxPitch() {
        return maxPitch;
    }

    public TourSummary setMaxPitch(float maxPitch) {
        this.maxPitch = maxPitch;
        return this;
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
