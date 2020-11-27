#ifndef INC_NMEA_GGA_H
#define INC_NMEA_GGA_H

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../nmea/nmea.h"

typedef struct {
	nmea_s base;
	struct tm time;
	nmea_position longitude;
	nmea_position latitude;
	int n_satellites;
	int8_t fix_status;
	int altitude;
	char altitude_unit;
	double hdop;
} nmea_gga_s;

/* Value indexes */
#define NMEA_GGA_TIME			0
#define NMEA_GGA_LATITUDE		1
#define NMEA_GGA_LATITUDE_CARDINAL	2
#define NMEA_GGA_LONGITUDE		3
#define NMEA_GGA_LONGITUDE_CARDINAL	4
#define NMEA_GGA_FIX		5
#define NMEA_GGA_N_SATELLITES		6
#define NMEA_GGA_HDOP		7
#define NMEA_GGA_ALTITUDE		8
#define NMEA_GGA_ALTITUDE_UNIT	9

#endif  /* INC_NMEA_GGA_H */
