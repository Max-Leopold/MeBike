package de.hhn.mebike.controller;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.ResponseBody;
import org.springframework.web.bind.annotation.RestController;

import de.hhn.mebike.model.Tour;
import de.hhn.mebike.service.TourService;

@RestController
public class TourController {

    private final TourService tourService;

    @Autowired
    public TourController(TourService tourService) {
        this.tourService = tourService;
    }

    @ResponseBody
    @RequestMapping(value = "/tour", method = RequestMethod.POST)
    public Tour saveTour(
        @RequestBody Tour tour
    ) {
        return tourService.store(tour);
    }

    @ResponseBody
    @RequestMapping(value = "/tour", method = RequestMethod.GET)
    public Tour getTour(
            @RequestParam(value = "tourId") long tourId
    ) {
        return tourService.getTour(tourId);
    }
}
