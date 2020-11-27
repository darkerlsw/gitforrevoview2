#include "../nmea/parser_types.h"
#include "gpgsv.h"
#include "parse.h"
#include "revo_os.h"

int
nmea_gpgsv_init(nmea_parser_s *parser)
{
	/* Declare what sentence type to parse */
	NMEA_PARSER_TYPE(parser, NMEA_GPGSV);
	NMEA_PARSER_PREFIX(parser, "GPGSV");
	return 0;
}

int
nmea_gpgsv_allocate_data(nmea_parser_s *parser)
{
	parser->data = revo_malloc(sizeof (nmea_gpgsv_s));
	if (NULL == parser->data) {
		return -1;
	}

	return 0;
}

int
nmea_gpgsv_set_default(nmea_parser_s *parser)
{
	memset(parser->data, 0, sizeof (nmea_gpgsv_s));
	return 0;
}

int
nmea_gpgsv_free_data(nmea_s *data)
{
	revo_free(data);
	return 0;
}

int
nmea_gpgsv_parse(nmea_parser_s *parser, char *value, int val_index)
{
	nmea_gpgsv_s *data = (nmea_gpgsv_s *) parser->data;

	switch (val_index) {
	case NMEA_GPGSV_SENTENCES:
		/* Parse sentences */
		data->sentences = atoi(value);
		break;

	case NMEA_GPGSV_SENTENCE_NUMBER:
		/* Parse latitude */
		data->sentence_number = atoi(value);
		break;

	case NMEA_GPGSV_SATELLITES:
		/* Parse cardinal direction */
		data->satellites = atoi(value);
		break;

	case NMEA_GPGSV_PRN:
		/* Parse longitude */
		data->prn = atoi(value);
		break;

	case NMEA_GPGSV_ELEVATION:
		/* Parse cardinal direction */
		data->elevation = atoi(value);
		break;

	case NMEA_GPGSV_AZIMUTH:
		/* Parse number of satellies */
		data->azimuth = atoi(value);
		break;

	case NMEA_GPGSV_SNR:
		/* Parse altitude */
		data->snr = atoi(value);
		break;

	default:
		break;
	}

	return 0;
}
