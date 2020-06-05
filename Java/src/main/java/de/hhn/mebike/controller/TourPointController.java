package de.hhn.mebike.controller;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.ResponseBody;
import org.springframework.web.bind.annotation.RestController;

import de.hhn.mebike.model.TourPoint;
import de.hhn.mebike.service.TourPointService;

@RestController
public class TourPointController {

    private final TourPointService tourPointService;

    @Autowired
    public TourPointController(TourPointService tourPointService) {
        this.tourPointService = tourPointService;
    }

    @ResponseBody
    @RequestMapping(value = "/tourPoint", method = RequestMethod.POST)
    public TourPoint saveTourPoint(
            @RequestBody TourPoint tourPoint
    ) {
        return tourPointService.store(tourPoint);
    }
}
