package de.hhn.mebike.service;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.stereotype.Service;
import org.springframework.web.server.ResponseStatusException;

import de.hhn.mebike.model.Client;
import de.hhn.mebike.repository.ClientRepository;

@Service
public class ClientService {

    private final ClientRepository clientRepository;

    @Autowired
    public ClientService(ClientRepository clientRepository) {
        this.clientRepository = clientRepository;
    }

    public Client store() {
        return clientRepository.save(new Client());
    }

    public Client getClient(long id) {
        Client client = clientRepository.findByClientId(id);
        if (client == null) {
            throw new ResponseStatusException(
                    HttpStatus.BAD_REQUEST, "Client does not exist"
            );
        }

        return clientRepository.findByClientId(id);
    }
}
