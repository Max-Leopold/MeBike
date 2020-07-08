package de.hhn.mebike.service;

import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import de.hhn.mebike.model.Client;
import de.hhn.mebike.model.Tour;
import de.hhn.mebike.repository.TourRepository;

@Service
public class TourService {

    private final TourRepository tourRepository;

    @Autowired
    public TourService(TourRepository tourRepository) {
        this.tourRepository = tourRepository;
    }

    public Tour getTour(long tourId) {
        return tourRepository.findById(tourId);
    }

    public List<Tour> getTours(Client client) {
        return tourRepository.findByClient(client);
    }

    public Tour startTour(Client client) {
        return tourRepository.save(new Tour().setClient(client));
    }
}
