#ifndef INC_NMEA_GLL_H
#define INC_NMEA_GLL_H

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../nmea/nmea.h"

typedef struct {
	nmea_s base;
	nmea_position longitude;
	nmea_position latitude;
	struct tm time;
	int8_t fix_status;
} nmea_gll_s;

/* Value indexes */
#define NMEA_GLL_LATITUDE		0
#define NMEA_GLL_LATITUDE_CARDINAL	1
#define NMEA_GLL_LONGITUDE		2
#define NMEA_GLL_LONGITUDE_CARDINAL	3
#define NMEA_GLL_TIME			4
#define NMEA_GLL_FIX			5

#endif  /* INC_NMEA_GLL_H */
