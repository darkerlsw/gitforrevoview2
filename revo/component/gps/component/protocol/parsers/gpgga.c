#include "../nmea/parser_types.h"
#include "gpgga.h"
#include "parse.h"
#include "revo_os.h"

int
nmea_gpgga_init(nmea_parser_s *parser)
{
	/* Declare what sentence type to parse */
	NMEA_PARSER_TYPE(parser, NMEA_GPGGA);
	NMEA_PARSER_PREFIX(parser, "GPGGA");
	return 0;
}

int
nmea_gpgga_allocate_data(nmea_parser_s *parser)
{
	parser->data = revo_malloc(sizeof (nmea_gpgga_s));
	if (NULL == parser->data) {
		return -1;
	}

	return 0;
}

int
nmea_gpgga_set_default(nmea_parser_s *parser)
{
	memset(parser->data, 0, sizeof (nmea_gpgga_s));
	return 0;
}

int
nmea_gpgga_free_data(nmea_s *data)
{
	revo_free(data);
	return 0;
}

int
nmea_gpgga_parse(nmea_parser_s *parser, char *value, int val_index)
{
	nmea_gpgga_s *data = (nmea_gpgga_s *) parser->data;

	switch (val_index) {
	case NMEA_GPGGA_TIME:
		/* Parse time */
		if (-1 == nmea_time_parse(value, &data->time)) {
			return -1;
		}
		break;

	case NMEA_GPGGA_LATITUDE:
		/* Parse latitude */
		if (-1 == nmea_position_parse(value, &data->latitude)) {
			return -1;
		}
		break;

	case NMEA_GPGGA_LATITUDE_CARDINAL:
		/* Parse cardinal direction */
		data->latitude.cardinal = nmea_cardinal_direction_parse(value);
		if (NMEA_CARDINAL_DIR_UNKNOWN == data->latitude.cardinal) {
			return -1;
		}
		break;

	case NMEA_GPGGA_LONGITUDE:
		/* Parse longitude */
		if (-1 == nmea_position_parse(value, &data->longitude)) {
			return -1;
		}
		break;

	case NMEA_GPGGA_LONGITUDE_CARDINAL:
		/* Parse cardinal direction */
		data->longitude.cardinal = nmea_cardinal_direction_parse(value);
		if (NMEA_CARDINAL_DIR_UNKNOWN == data->longitude.cardinal) {
			return -1;
		}
		break;

	case NMEA_GPGGA_FIX:
		data->fix_status = atoi(value);
		break;

	case NMEA_GPGGA_N_SATELLITES:
		/* Parse number of satellies */
		data->n_satellites = atoi(value);
		break;

	case NMEA_GPGGA_ALTITUDE:
		/* Parse altitude */
		data->altitude = atoi(value);
		break;

	case NMEA_GPGGA_ALTITUDE_UNIT:
		/* Parse altitude unit */
		data->altitude_unit = *value;
		break;
	case NMEA_GPGGA_HDOP:
	if (-1 == nmea_float_parse(value, &data->hdop)) {
		return -1;
	}
	default:
		break;
	}

	return 0;
}
