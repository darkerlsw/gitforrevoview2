#include "../nmea/parser_types.h"
#include "gpgsa.h"
#include "parse.h"
#include "revo_os.h"

int
nmea_gpgsa_init(nmea_parser_s *parser)
{
	/* Declare what sentence type to parse */
	NMEA_PARSER_TYPE(parser, NMEA_GPGSA);
	NMEA_PARSER_PREFIX(parser, "GPGSA");
	return 0;
}

int
nmea_gpgsa_allocate_data(nmea_parser_s *parser)
{
	parser->data = revo_malloc(sizeof (nmea_gpgsa_s));
	if (NULL == parser->data) {
		return -1;
	}

	return 0;
}

int
nmea_gpgsa_set_default(nmea_parser_s *parser)
{
	memset(parser->data, 0, sizeof (nmea_gpgsa_s));
	return 0;
}

int
nmea_gpgsa_free_data(nmea_s *data)
{
	revo_free(data);
	return 0;
}

int
nmea_gpgsa_parse(nmea_parser_s *parser, char *value, int val_index)
{
	nmea_gpgsa_s *data = (nmea_gpgsa_s *) parser->data;

	switch (val_index) {
	case NMEA_GPGSA_OPMODE:
		/* Parse altitude */
		if( 'M' == *value )
			data->op_mode = 0; //manual
		else if( 'A' == *value )
			data->fix_status = 1; //auto
		else
			data->fix_status = -1; //invalid
		break;
	case NMEA_GPGSA_FIX:
		/* Parse altitude unit */
		data->fix_status = atoi(value);
		break;
	default:
		break;
	}

	return 0;
}
