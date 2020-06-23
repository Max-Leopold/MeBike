package de.hhn.mebike.controller;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.ResponseBody;
import org.springframework.web.bind.annotation.RestController;

import de.hhn.mebike.dto.TourSummaryDTO;
import de.hhn.mebike.service.TourSummaryService;

@RestController
public class TourSummaryController {

    private final TourSummaryService tourSummaryService;

    @Autowired
    public TourSummaryController(TourSummaryService tourSummaryService) {
        this.tourSummaryService = tourSummaryService;
    }

    @ResponseBody
    @RequestMapping(value = "/tourSummary", method = RequestMethod.GET)
    public TourSummaryDTO getTourSummaries(
            @RequestParam long clientId
    ) {
        return new TourSummaryDTO(tourSummaryService.getTourSummaries(clientId));
    }
}
