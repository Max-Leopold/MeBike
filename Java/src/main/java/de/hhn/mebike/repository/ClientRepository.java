package de.hhn.mebike.repository;

import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;

import de.hhn.mebike.model.Client;

@Repository
public interface ClientRepository extends JpaRepository<Client, Long> {

    Client findByClientId(long id);
}
