package de.hhn.mebike.controller;

import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.ResponseBody;
import org.springframework.web.bind.annotation.RestController;

import de.hhn.mebike.model.TourPoint;
import de.hhn.mebike.service.TourPointService;
import de.hhn.mebike.service.TourService;

@RestController
public class TourPointController {

    private final TourPointService tourPointService;
    private final TourService tourService;

    @Autowired
    public TourPointController(TourPointService tourPointService, TourService tourService) {
        this.tourPointService = tourPointService;
        this.tourService = tourService;
    }

    @ResponseBody
    @RequestMapping(value = "/tourPoint", method = RequestMethod.POST)
    public TourPoint saveTourPoint(
            @RequestBody TourPoint tourPoint
    ) {
        return tourPointService.store(tourPoint);
    }

    @ResponseBody
    @RequestMapping(value = "/tourPoint", method = RequestMethod.GET)
    public List<TourPoint> getTourPoints(
            @RequestParam long tourId
    ) {
        return tourPointService.getTourPoints(tourService.getTour(tourId));
    }
}
