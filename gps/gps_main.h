//
// Created by Max Leopold on 07/06/2020.
//

#ifndef ME_BIKE_GPS_MAIN_H
#define ME_BIKE_GPS_MAIN_H

struct gps_coordinates {
    char *longitude;
    char *latitude;
    char *gmt_time;
};

void gps_init(void);

void gps_main(void);

void check_for_coordinates(char c);

struct gps_coordinates create_gps_coordinates();

#endif //ME_BIKE_GPS_MAIN_H
