#ifndef INC_NMEA_RMC_H
#define INC_NMEA_RMC_H

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../nmea/nmea.h"

typedef struct {
	nmea_s base;
	nmea_position longitude;
	nmea_position latitude;
	double speed;
	float course;
	struct tm time;
	int8_t fix_status;
} nmea_rmc_s;

/* Value indexes */
#define NMEA_RMC_TIME		0
#define NMEA_RMC_FIX		1
#define NMEA_RMC_LATITUDE		2
#define NMEA_RMC_LATITUDE_CARDINAL	3
#define NMEA_RMC_LONGITUDE		4
#define NMEA_RMC_LONGITUDE_CARDINAL	5
#define NMEA_RMC_SPEED		6
#define NMEA_RMC_COURSE		7
#define NMEA_RMC_DATE		8


#endif  /* INC_NMEA_RMC_H */
