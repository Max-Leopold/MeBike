package de.hhn.mebike.repository;

import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;

import de.hhn.mebike.model.TourPoint;

@Repository
public interface TourPointRepository extends JpaRepository<TourPoint, Long> {
}
