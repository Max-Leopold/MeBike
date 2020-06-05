package de.hhn.mebike.repository;

import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;

import de.hhn.mebike.model.Tour;

@Repository
public interface TourRepository extends JpaRepository<Tour, Long> {
    Tour findById(long id);
}
