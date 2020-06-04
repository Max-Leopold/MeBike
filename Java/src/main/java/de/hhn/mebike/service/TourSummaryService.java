package de.hhn.mebike.service;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.jdbc.core.RowMapper;
import org.springframework.jdbc.core.namedparam.NamedParameterJdbcTemplate;
import org.springframework.stereotype.Service;

import com.google.common.collect.ImmutableMap;

import de.hhn.mebike.tour.FlattenedTourEntry;
import de.hhn.mebike.tour.TourSummary;
import de.hhn.mebike.util.HaversineAlgorithm;

@Service
public class TourSummaryService {

    private static final String SELECT_TOURS =
            "SELECT t.id                          as tourId,\n" +
                    "       tp.pitch                      as pitch,\n" +
                    "       tp.pitch                      as pulse,\n" +
                    "       tp.speed                      as speed,\n" +
                    "       tp.temperature                as temperature,\n" +
                    "       tp.gps_point_latitude_degree  as latitude,\n" +
                    "       tp.gps_point_longitude_degree as longitude,\n" +
                    "       tp.timestamp                  as time\n" +
                    "FROM tour as t,\n" +
                    "     tour_tour_points as ttp,\n" +
                    "     tour_point as tp,\n" +
                    "     client_tours as ct\n" +
                    "WHERE ct.client_client_id = (:client_id)\n" +
                    "  AND t.id = ct.tours_id\n" +
                    "  AND t.id = ttp.tour_id\n" +
                    "  AND ttp.tour_points_tour_point_id = tp.tour_point_id\n" +
                    "GROUP BY t.id,\n" +
                    "         tp.pitch,\n" +
                    "         tp.pitch,\n" +
                    "         tp.speed,\n" +
                    "         tp.temperature,\n" +
                    "         tp.gps_point_latitude_degree,\n" +
                    "         tp.gps_point_longitude_degree";

    private static final String TOUR_ID = "tourId";
    private static final String PITCH = "pitch";
    private static final String PULSE = "pulse";
    private static final String SPEED = "speed";
    private static final String TEMPERATURE = "temperature";
    private static final String LATITUDE = "latitude";
    private static final String LONGITUDE = "longitude";
    private static final String TIMESTAMP = "time";

    private final NamedParameterJdbcTemplate npjt;

    @Autowired
    public TourSummaryService(
            NamedParameterJdbcTemplate npjt
    ) {
        this.npjt = npjt;
    }

    public List<TourSummary> getTourSummaries(long clientId) {
        List<FlattenedTourEntry> flattenedTourEntries = npjt.query(
                SELECT_TOURS,
                ImmutableMap.of(
                        "client_id", clientId
                ),
                new TourSummaryRowMapper()
        );

        return createTourSummaries(flattenedTourEntries);
    }

    private List<TourSummary> createTourSummaries(List<FlattenedTourEntry> flattenedTourEntries) {
        flattenedTourEntries.sort(Comparator.comparingLong(FlattenedTourEntry::getTourId));

        int currentIndex = 0;
        List<List<FlattenedTourEntry>> flattenedTourEntries2d = new ArrayList<>();
        flattenedTourEntries2d.add(new ArrayList<>());

        for (int i = 0; i < flattenedTourEntries.size(); i++) {
            if (flattenedTourEntries2d.get(currentIndex).size() == 0) {
                flattenedTourEntries2d.get(currentIndex).add(flattenedTourEntries.get(i));
            } else if (flattenedTourEntries2d.get(currentIndex).get(0).getTourId() == flattenedTourEntries.get(i).getTourId()) {
                flattenedTourEntries2d.get(currentIndex).add(flattenedTourEntries.get(i));
            } else {
                currentIndex += 1;
                flattenedTourEntries2d.add(new ArrayList<>());
                flattenedTourEntries2d.get(currentIndex).add(flattenedTourEntries.get(i));
            }
        }

        List<TourSummary> tourSummaries = new ArrayList<>();

        for (int i = 0; i < flattenedTourEntries2d.size(); i++) {

            TourSummary tourSummary = new TourSummary();
            for (int j = 0; j < flattenedTourEntries2d.get(i).size(); j++) {
                if (j == 0) {
                    FlattenedTourEntry flattenedTourEntry = flattenedTourEntries2d.get(i).get(j);

                    tourSummary.setDistance(0);
                    tourSummary.setTime(0);
                    tourSummary.setPulseMedium(flattenedTourEntry.getPulse());
                    tourSummary.setTourId(flattenedTourEntry.getTourId());
                } else {
                    FlattenedTourEntry lastTourEntry = flattenedTourEntries2d.get(i).get(j - 1);
                    FlattenedTourEntry flattenedTourEntry = flattenedTourEntries2d.get(i).get(j);

                    tourSummary.setPulseMedium((tourSummary.getPulseMedium() * j + flattenedTourEntry.getPulse()) / j + 1);
                    tourSummary.setDistance(
                            tourSummary.getDistance() +
                                    HaversineAlgorithm.HaversineInM(
                                            flattenedTourEntry.getLatitude(),
                                            flattenedTourEntry.getLongitude(),
                                            lastTourEntry.getLatitude(),
                                            lastTourEntry.getLongitude()
                                    ));
                    tourSummary.setTime((int) (tourSummary.getTime() + (flattenedTourEntry.getTimestamp() - lastTourEntry.getTimestamp())));
                }
            }

            tourSummaries.add(tourSummary);
        }

        return tourSummaries;
    }

    private class TourSummaryRowMapper implements RowMapper<FlattenedTourEntry> {

        @Override
        public FlattenedTourEntry mapRow(ResultSet resultSet, int i) throws SQLException {
            return new FlattenedTourEntry(
                    resultSet.getLong(TOUR_ID),
                    resultSet.getFloat(PITCH),
                    resultSet.getFloat(PULSE),
                    resultSet.getFloat(SPEED),
                    resultSet.getFloat(TEMPERATURE),
                    resultSet.getFloat(LATITUDE),
                    resultSet.getFloat(LONGITUDE),
                    resultSet.getLong(TIMESTAMP));
        }
    }

}
