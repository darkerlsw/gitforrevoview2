#include "../nmea/parser_types.h"
#include "bdgsa.h"
#include "parse.h"
#include "revo_os.h"

int
nmea_bdgsa_init(nmea_parser_s *parser)
{
	/* Declare what sentence type to parse */
	NMEA_PARSER_TYPE(parser, NMEA_BDGSA);
	NMEA_PARSER_PREFIX(parser, "BDGSA");
	return 0;
}

int
nmea_bdgsa_allocate_data(nmea_parser_s *parser)
{
	parser->data = revo_malloc(sizeof (nmea_bdgsa_s));
	if (NULL == parser->data) {
		return -1;
	}

	return 0;
}

int
nmea_bdgsa_set_default(nmea_parser_s *parser)
{
	memset(parser->data, 0, sizeof (nmea_bdgsa_s));
	return 0;
}

int
nmea_bdgsa_free_data(nmea_s *data)
{
	revo_free(data);
	return 0;
}

int
nmea_bdgsa_parse(nmea_parser_s *parser, char *value, int val_index)
{
	nmea_bdgsa_s *data = (nmea_bdgsa_s *) parser->data;

	switch (val_index) {
	case NMEA_BDGSA_OPMODE:
		/* Parse altitude */
		if( 'M' == *value )
			data->op_mode = 0; //manual
		else if( 'A' == *value )
			data->fix_status = 1; //auto
		else
			data->fix_status = -1; //invalid
		break;
	case NMEA_BDGSA_FIX:
		/* Parse altitude unit */
		data->fix_status = atoi(value);
		break;
	default:
		break;
	}

	return 0;
}
