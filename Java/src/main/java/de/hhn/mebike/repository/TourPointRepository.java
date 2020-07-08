package de.hhn.mebike.repository;

import java.util.List;

import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;

import de.hhn.mebike.model.Tour;
import de.hhn.mebike.model.TourPoint;

@Repository
public interface TourPointRepository extends JpaRepository<TourPoint, Long> {
    List<TourPoint> findByTour(Tour tour);
}
