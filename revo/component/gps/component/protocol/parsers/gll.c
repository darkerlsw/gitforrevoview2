#include "../nmea/parser_types.h"
#include "gll.h"
#include "parse.h"
#include "revo_os.h"

int
nmea_gll_init(nmea_parser_s *parser)
{
	/* Declare what sentence type to parse */
	NMEA_PARSER_TYPE(parser, NMEA_GLL);
	NMEA_PARSER_PREFIX(parser, "GLL");
	return 0;
}

int
nmea_gll_allocate_data(nmea_parser_s *parser)
{
	parser->data = revo_malloc(sizeof (nmea_gll_s));
	if (NULL == parser->data) {
		return -1;
	}

	return 0;
}

int
nmea_gll_set_default(nmea_parser_s *parser)
{
	memset(parser->data, 0, sizeof (nmea_gll_s));
	return 0;
}

int
nmea_gll_free_data(nmea_s *data)
{
	revo_free(data);
	return 0;
}

int
nmea_gll_parse(nmea_parser_s *parser, char *value, int val_index)
{
	nmea_gll_s *data = (nmea_gll_s *) parser->data;

	switch (val_index) {
	case NMEA_GLL_TIME:
		/* Parse time */
		if (-1 == nmea_time_parse(value, &data->time)) {
			return -1;
		}
		break;

	case NMEA_GLL_LATITUDE:
		/* Parse latitude */
		if (-1 == nmea_position_parse(value, &data->latitude)) {
			return -1;
		}
		break;

	case NMEA_GLL_LATITUDE_CARDINAL:
		/* Parse cardinal direction */
		data->latitude.cardinal = nmea_cardinal_direction_parse(value);
		if (NMEA_CARDINAL_DIR_UNKNOWN == data->latitude.cardinal) {
			return -1;
		}
		break;

	case NMEA_GLL_LONGITUDE:
		/* Parse longitude */
		if (-1 == nmea_position_parse(value, &data->longitude)) {
			return -1;
		}
		break;

	case NMEA_GLL_LONGITUDE_CARDINAL:
		/* Parse cardinal direction */
		data->longitude.cardinal = nmea_cardinal_direction_parse(value);
		if (NMEA_CARDINAL_DIR_UNKNOWN == data->longitude.cardinal) {
			return -1;
		}
		break;
	case NMEA_GLL_FIX:
		if( 'A' == *value )
			data->fix_status = 1;
		else
			data->fix_status = 0;
	default:
		break;
	}

	return 0;
}
