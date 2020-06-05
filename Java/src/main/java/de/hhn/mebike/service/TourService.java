package de.hhn.mebike.service;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import de.hhn.mebike.model.Tour;
import de.hhn.mebike.repository.TourRepository;

@Service
public class TourService {

    private final TourRepository tourRepository;

    @Autowired
    public TourService(TourRepository tourRepository) {
        this.tourRepository = tourRepository;
    }

    public Tour store(Tour tour) {
        return tourRepository.save(tour);
    }

    public Tour getTour(long tourId) {
        return tourRepository.findById(tourId);
    }
}
