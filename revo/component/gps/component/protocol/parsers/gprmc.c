#include "../nmea/parser_types.h"
#include "gprmc.h"
#include "parse.h"
#include "revo_os.h"

int
nmea_gprmc_init(nmea_parser_s *parser)
{
	/* Declare what sentence type to parse */
	NMEA_PARSER_TYPE(parser, NMEA_GPRMC);
	NMEA_PARSER_PREFIX(parser, "GPRMC");
	return 0;
}

int
nmea_gprmc_allocate_data(nmea_parser_s *parser)
{
	parser->data = revo_malloc(sizeof (nmea_gprmc_s));
	if (NULL == parser->data) {
		return -1;
	}

	return 0;
}

int
nmea_gprmc_set_default(nmea_parser_s *parser)
{
	memset(parser->data, 0, sizeof (nmea_gprmc_s));
	return 0;
}

int
nmea_gprmc_free_data(nmea_s *data)
{
	revo_free(data);
	return 0;
}

int
nmea_gprmc_parse(nmea_parser_s *parser, char *value, int val_index)
{
	nmea_gprmc_s *data = (nmea_gprmc_s *) parser->data;

	switch (val_index) {
	case NMEA_GPRMC_TIME:
		/* Parse time */
		if (-1 == nmea_time_parse(value, &data->time)) {
			return -1;
		}
		break;

	case NMEA_GPRMC_LATITUDE:
		/* Parse latitude */
		if (-1 == nmea_position_parse(value, &data->latitude)) {
			return -1;
		}
		break;

	case NMEA_GPRMC_LATITUDE_CARDINAL:
		/* Parse cardinal direction */
		data->latitude.cardinal = nmea_cardinal_direction_parse(value);
		if (NMEA_CARDINAL_DIR_UNKNOWN == data->latitude.cardinal) {
			return -1;
		}
		break;

	case NMEA_GPRMC_LONGITUDE:
		/* Parse longitude */
		if (-1 == nmea_position_parse(value, &data->longitude)) {
			return -1;
		}
		break;

	case NMEA_GPRMC_LONGITUDE_CARDINAL:
		/* Parse cardinal direction */
		data->longitude.cardinal = nmea_cardinal_direction_parse(value);
		if (NMEA_CARDINAL_DIR_UNKNOWN == data->longitude.cardinal) {
			return -1;
		}
		break;

	case NMEA_GPRMC_DATE:
		/* Parse date */
		if (-1 == nmea_date_parse(value, &data->time)) {
			return -1;
		}
		break;

	case NMEA_GPRMC_SPEED:
		/* Parse ground speed in knots */
		data->speed = atof(value);
		break;
	case NMEA_GPRMC_COURSE:
		/* Parse ground course. 0-360度 */
		data->course = atof(value);
		break;
		
	case NMEA_GPRMC_FIX:
		if( 'A' == *value )
			data->fix_status = 1;
		else
			data->fix_status = 0;
		break;
	default:
		break;
	}

	return 0;
}
