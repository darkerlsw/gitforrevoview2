#include "../nmea/parser_types.h"
#include "gga.h"
#include "parse.h"
#include "revo_os.h"

int
nmea_gga_init(nmea_parser_s *parser)
{
	/* Declare what sentence type to parse */
	NMEA_PARSER_TYPE(parser, NMEA_GGA);
	NMEA_PARSER_PREFIX(parser, "GGA");
	return 0;
}

int
nmea_gga_allocate_data(nmea_parser_s *parser)
{
	parser->data = revo_malloc(sizeof (nmea_gga_s));
	if (NULL == parser->data) {
		return -1;
	}

	return 0;
}

int
nmea_gga_set_default(nmea_parser_s *parser)
{
	memset(parser->data, 0, sizeof (nmea_gga_s));
	return 0;
}

int
nmea_gga_free_data(nmea_s *data)
{
	revo_free(data);
	return 0;
}

int
nmea_gga_parse(nmea_parser_s *parser, char *value, int val_index)
{
	nmea_gga_s *data = (nmea_gga_s *) parser->data;

	switch (val_index) {
	case NMEA_GGA_TIME:
		/* Parse time */
		if (-1 == nmea_time_parse(value, &data->time)) {
			return -1;
		}
		break;

	case NMEA_GGA_LATITUDE:
		/* Parse latitude */
		if (-1 == nmea_position_parse(value, &data->latitude)) {
			return -1;
		}
		break;

	case NMEA_GGA_LATITUDE_CARDINAL:
		/* Parse cardinal direction */
		data->latitude.cardinal = nmea_cardinal_direction_parse(value);
		if (NMEA_CARDINAL_DIR_UNKNOWN == data->latitude.cardinal) {
			return -1;
		}
		break;

	case NMEA_GGA_LONGITUDE:
		/* Parse longitude */
		if (-1 == nmea_position_parse(value, &data->longitude)) {
			return -1;
		}
		break;

	case NMEA_GGA_LONGITUDE_CARDINAL:
		/* Parse cardinal direction */
		data->longitude.cardinal = nmea_cardinal_direction_parse(value);
		if (NMEA_CARDINAL_DIR_UNKNOWN == data->longitude.cardinal) {
			return -1;
		}
		break;

	case NMEA_GGA_FIX:
		data->fix_status = atoi(value);
		break;
	case NMEA_GGA_N_SATELLITES:
		/* Parse number of satellies */
		data->n_satellites = atoi(value);
		break;

	case NMEA_GGA_ALTITUDE:
		/* Parse altitude */
		data->altitude = atoi(value);
		break;

	case NMEA_GGA_ALTITUDE_UNIT:
		/* Parse altitude unit */
		data->altitude_unit = *value;
		break;
	case NMEA_GGA_HDOP:
		if (-1 == nmea_float_parse(value, &data->hdop)) {
			return -1;
		}
		break;
	default:
		break;
	}

	return 0;
}
