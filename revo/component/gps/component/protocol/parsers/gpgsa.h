#ifndef INC_NMEA_GPGSA_H
#define INC_NMEA_GPGSA_H

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../nmea/nmea.h"

typedef struct {
	nmea_s base;
	int8_t op_mode;//A/M-->0/1
	int8_t fix_status;
} nmea_gpgsa_s;

/* Value indexes */
#define NMEA_GPGSA_OPMODE			0
#define NMEA_GPGSA_FIX		1

#endif  /* INC_NMEA_GPGSA_H */
