package de.hhn.mebike.service;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import de.hhn.mebike.model.Client;
import de.hhn.mebike.repository.ClientRepository;

@Service
public class ClientService {

    private final ClientRepository clientRepository;

    @Autowired
    public ClientService(ClientRepository clientRepository) {
        this.clientRepository = clientRepository;
    }

    public Client store(Client client) {
        return clientRepository.save(client);
    }

    public Client getClient(long clientId) {
        return clientRepository.findByClientId(clientId);
    }
}
