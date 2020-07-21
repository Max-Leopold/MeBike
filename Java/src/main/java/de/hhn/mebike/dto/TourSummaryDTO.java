package de.hhn.mebike.dto;

import java.util.List;

import de.hhn.mebike.tour.TourSummary;

public class TourSummaryDTO {

    private List<TourSummary> tourSummaries;

    public TourSummaryDTO(List<TourSummary> tourSummaries) {
        this.tourSummaries = tourSummaries;
    }

    public List<TourSummary> getTourSummaries() {
        return tourSummaries;
    }

    public void setTourSummaries(List<TourSummary> tourSummaries) {
        this.tourSummaries = tourSummaries;
    }

    @Override
    public String toString() {
        return "TourSummaryDTO{" +
                "tourSummaries=" + tourSummaries +
                '}';
    }
}
