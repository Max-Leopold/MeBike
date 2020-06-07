package de.hhn.mebike.controller;

import java.util.List;

import org.springframework.validation.annotation.Validated;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.ResponseBody;
import org.springframework.web.bind.annotation.RestController;

import de.hhn.mebike.model.Client;
import de.hhn.mebike.model.Tour;
import de.hhn.mebike.service.ClientService;

@RestController
public class ClientController {

    private final ClientService clientService;

    public ClientController(ClientService clientService) {
        this.clientService = clientService;
    }

    @ResponseBody
    @RequestMapping(value = "/client", method = RequestMethod.POST)
    public Client saveClient(
            @RequestBody Client client
    ) {
        return clientService.store(client);
    }

    @ResponseBody
    @RequestMapping(value = "/client", method = RequestMethod.GET)
    public Client getClient(
            @RequestParam(value = "clientId") long clientId
    ) {
        return clientService.getClient(clientId);
    }
}
