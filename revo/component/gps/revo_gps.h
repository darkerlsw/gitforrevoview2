#ifndef __REVO_GPS_H__
#define __REVO_GPS_H__

#include "revo_os.h"
#include "revo_gps_uart.h"
#include "revo_gps_dev.h"
#include "nmea.h"
#include "gll.h"
#include "gga.h"
#include "rmc.h"
#include "gsv.h"
#include "gpgll.h"
#include "gpgga.h"
#include "gprmc.h"
#include "gpgsv.h"
#include "gpgsa.h"
#include "bdgsa.h"

typedef enum {
	REVO_GPS_TYPE_GGA,
	REVO_GPS_TYPE_GLL,
	REVO_GPS_TYPE_GSA,
	REVO_GPS_TYPE_GSV,
	REVO_GPS_TYPE_RMC,
	REVO_GPS_TYPE_BDGSA,
	REVO_GPS_TYPE_GPGSA
} revo_gps_nmea_type_e;

typedef enum {
	REVO_GPS_LONGITUDE,
	REVO_GPS_LATITUDE,
	REVO_GPS_LONGITUDE_D,
	REVO_GPS_LATITUDE_D,
	REVO_GPS_DATE,
	REVO_GPS_SAT_NUM,
} revo_gps_info_type_e;

typedef union {
	struct tm time;
	float lng;
	float lat;
	revo_char lng_cardinal;
	revo_char lat_cardinal;
	revo_u32 num_sat;
} revo_gps_app_info_u;

/* union for user to get info */
typedef struct {
#define REVO_GPS_STR_INFO_LEN 128
	//str
	revo_char str[REVO_GPS_STR_INFO_LEN];
	//logic data
	revo_gps_app_info_u ld;
} revo_gps_info_t;

typedef struct {	
	//COMMON
	nmea_gll_s gll_s;
	nmea_gga_s gga_s;
	nmea_rmc_s rmc_s;
	nmea_gsv_s gsv_s;

	//GPS
	nmea_gpgll_s gpgll_s;
	nmea_gpgga_s gpgga_s;
	nmea_gprmc_s gprmc_s;
	nmea_gpgsv_s gpgsv_s;
	nmea_gpgsa_s gpgsa_s;
	//BD
	nmea_bdgsa_s bdgsa_s;
} revo_gps_nmea_parser_t;

typedef union {	
	//COMMON
	nmea_gll_s gll_s;
	nmea_gga_s gga_s;
	nmea_rmc_s rmc_s;
	nmea_gsv_s gsv_s;

	//GPS
	nmea_gpgll_s gpgll_s;
	nmea_gpgga_s gpgga_s;
	nmea_gprmc_s gprmc_s;
	nmea_gpgsv_s gpgsv_s;
	nmea_gpgsa_s gpgsa_s;
	//BD
	nmea_bdgsa_s bdgsa_s;
} revo_gps_nmea_parser_u;

/* inner gps app info */
typedef struct {
	struct tm time;
	float lng;
	float lat;
	revo_char lng_cardinal;
	revo_char lat_cardinal;
	revo_u32 num_sat;

	//precision
	double hdop;
} revo_gps_app_info_t;

typedef struct {
	revo_bool is_open;
	revo_gps_dev_t dev_info;
	revo_gps_nmea_parser_t nmea_info;
	revo_gps_app_info_t app_info;
} revo_gps_ctx_t;

void revo_gps_open(void);
void revo_gps_read(revo_gps_info_type_e type, revo_gps_info_t *data);

#endif

