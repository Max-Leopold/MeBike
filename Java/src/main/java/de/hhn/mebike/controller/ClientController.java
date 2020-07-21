package de.hhn.mebike.controller;

import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.ResponseBody;
import org.springframework.web.bind.annotation.RestController;

import de.hhn.mebike.model.Client;
import de.hhn.mebike.service.ClientService;

@RestController
public class ClientController {

    private final ClientService clientService;

    public ClientController(ClientService clientService) {
        this.clientService = clientService;
    }

    @ResponseBody
    @RequestMapping(value = "/client", method = RequestMethod.POST)
    public Client saveClient() {
        return clientService.store();
    }

    @ResponseBody
    @RequestMapping(value = "/client", method = RequestMethod.GET)
    public boolean checkForClient(
            @RequestParam long id
    ) {
        return clientService.getClient(id) != null;
    }
}
