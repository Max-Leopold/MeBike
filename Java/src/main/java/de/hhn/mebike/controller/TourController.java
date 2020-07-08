package de.hhn.mebike.controller;

import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.ResponseBody;
import org.springframework.web.bind.annotation.RestController;

import de.hhn.mebike.model.Tour;
import de.hhn.mebike.service.ClientService;
import de.hhn.mebike.service.TourService;

@RestController
public class TourController {

    private final TourService tourService;
    private final ClientService clientService;

    @Autowired
    public TourController(TourService tourService, ClientService clientService) {
        this.tourService = tourService;
        this.clientService = clientService;
    }

    @RequestMapping(value = "/tour/start", method = RequestMethod.POST)
    public Tour startTour(
            @RequestParam(value = "clientId") long clientId
    ) {
        return tourService.startTour(clientService.getClient(clientId));
    }

    @ResponseBody
    @RequestMapping(value = "/tour", method = RequestMethod.GET, params = {"tourId"})
    public Tour getTour(
            @RequestParam(value = "tourId", required = false) long tourId
    ) {
        return tourService.getTour(tourId);
    }

    @ResponseBody
    @RequestMapping(value = "/tour", method = RequestMethod.GET)
    public List<Tour> getTours(
            @RequestParam(value = "clientId") long clientId
    ) {
        return tourService.getTours(clientService.getClient(clientId));
    }
}
