package de.hhn.mebike.service;

import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import de.hhn.mebike.model.Tour;
import de.hhn.mebike.model.TourPoint;
import de.hhn.mebike.repository.TourPointRepository;

@Service
public class TourPointService {

    private final TourPointRepository tourPointRepository;

    @Autowired
    public TourPointService(TourPointRepository tourPointRepository) {
        this.tourPointRepository = tourPointRepository;
    }

    public TourPoint store(TourPoint tourPoint) {
        return tourPointRepository.save(tourPoint);
    }

    public List<TourPoint> getTourPoints(Tour tour) {
        return tourPointRepository.findByTour(tour);
    }
}
