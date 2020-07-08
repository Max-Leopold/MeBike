package de.hhn.mebike.repository;

import java.util.List;

import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;

import de.hhn.mebike.model.Client;
import de.hhn.mebike.model.Tour;

@Repository
public interface TourRepository extends JpaRepository<Tour, Long> {
    Tour findById(long id);

    List<Tour> findByClient(Client client);
}
