#ifndef INC_NMEA_GSV_H
#define INC_NMEA_GSV_H

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../nmea/nmea.h"

typedef struct {
	nmea_s base;
	unsigned int sentences; //Number of sentences for full data
	unsigned int sentence_number; //Current sentence number
	unsigned int satellites; //Number of satellites in view
	unsigned int prn; //Satellite PRN number
	int elevation; //Elevation, degrees
	int azimuth; //Azimuth, degrees
	int snr; //SNR - higher is better
} nmea_gsv_s;

/* Value indexes */
enum {
	NMEA_GSV_SENTENCES,
	NMEA_GSV_SENTENCE_NUMBER,
	NMEA_GSV_SATELLITES,
	NMEA_GSV_PRN,
	NMEA_GSV_ELEVATION,
	NMEA_GSV_AZIMUTH,
	NMEA_GSV_SNR
};

#endif  /* INC_NMEA_GSV_H */
