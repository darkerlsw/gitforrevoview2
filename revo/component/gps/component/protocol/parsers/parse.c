#include <stdio.h>
#include <string.h>
#include <time.h>
#include "parse.h"

#define TM_YEAR_START 1900
#define RMC_YEAR_START 2000

int
nmea_position_parse(char *s, nmea_position *pos)
{
	char *cursor;

	pos->degrees = 0;
	pos->minutes = 0;

	if (s == NULL || *s == '\0') {
		return -1;
	}

	/* decimal minutes */
	if (NULL == (cursor = strchr(s, '.'))) {
		return -1;
	}
	/* minutes starts 2 digits before dot */
	cursor -= 2;
	pos->minutes = atof(cursor);
	*cursor = '\0';

	/* integer degrees */
	cursor = s;
	pos->degrees = atoi(cursor);

	return 0;
}

int
nmea_float_parse(char *s, double *out)
{
	if (s == NULL || *s == '\0') {
		return -1;
	}
	*out = atof(s);

	return 0;
}

nmea_cardinal_t
nmea_cardinal_direction_parse(char *s)
{
	if (NULL == s || '\0'== *s) {
		return NMEA_CARDINAL_DIR_UNKNOWN;
	}

	switch (*s) {
	case NMEA_CARDINAL_DIR_NORTH:
		return NMEA_CARDINAL_DIR_NORTH;
	case NMEA_CARDINAL_DIR_EAST:
		return NMEA_CARDINAL_DIR_EAST;
	case NMEA_CARDINAL_DIR_SOUTH:
		return NMEA_CARDINAL_DIR_SOUTH;
	case NMEA_CARDINAL_DIR_WEST:
		return NMEA_CARDINAL_DIR_WEST;
	default:
		break;
	}

	return NMEA_CARDINAL_DIR_UNKNOWN;
}

int
nmea_time_parse(char *s, struct tm *time)
{
#ifdef STRPTIME
	char *rv;
#endif
	memset(time, 0, sizeof (struct tm));

	if (s == NULL || *s == '\0') {
		return -1;
	}
#ifdef STRPTIME
	rv = strptime(s, NMEA_TIME_FORMAT, time);
	if (NULL == rv || (int) (rv - s) != NMEA_TIME_FORMAT_LEN) {
		return -1;
	}
#else	//revo tjd
	sscanf(s, "%02d%02d%02d", &time->tm_hour, &time->tm_min, &time->tm_sec);
#endif
	return 0;
}

int
nmea_date_parse(char *s, struct tm *time)
{
#ifdef STRPTIME
	char *rv;
#endif
	// Assume it has been already cleared
	// memset(time, 0, sizeof (struct tm));

	if (s == NULL || *s == '\0') {
		return -1;
	}

#ifdef STRPTIME
	rv = strptime(s, NMEA_DATE_FORMAT, time);
	if (NULL == rv || (int) (rv - s) != NMEA_TIME_FORMAT_LEN) {
		return -1;
	}
#else	//revo tjd
	sscanf(s, "%02d%02d%02d", &time->tm_mday, &time->tm_mon, &time->tm_year);
	time->tm_year += (2000 - 1900);//after 1900
	time->tm_mon -= 1; //mon(0~11)
#endif
#if 0
	// Normalize tm_year according to C standard library
	if (time->tm_year > 1900)
		time->tm_year -= TM_YEAR_START; // ZDA message case
	else
		time->tm_year += (RMC_YEAR_START - TM_YEAR_START); // RMC message case
#endif
	return 0;
}
