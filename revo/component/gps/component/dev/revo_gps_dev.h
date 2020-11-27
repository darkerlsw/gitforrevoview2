#ifndef __REVO_GPS_DEV_H__
#define __REVO_GPS_DEV_H__

#define REVO_GPS_DEV_INFO_LENGTH 25

typedef enum {
	REVO_GPS_IC_NAME,
	REVO_GPS_HW_VER,
	REVO_GPS_SW_VER,
	REVO_GPS_GNSS_M,
	REVO_GPS_NMEA_FMT,
	REVO_GPS_DEV_INFO_END
} revo_gps_dev_info_e;

typedef struct {
	revo_char ic_name[REVO_GPS_DEV_INFO_LENGTH];
	revo_char hw_version[REVO_GPS_DEV_INFO_LENGTH];
	revo_char sw_version[REVO_GPS_DEV_INFO_LENGTH];
	revo_char gnss_mode[REVO_GPS_DEV_INFO_LENGTH];
	revo_char nmea_format[REVO_GPS_DEV_INFO_LENGTH];
} revo_gps_dev_t;

revo_s32 revo_gps_devinfo_parse(revo_char *in, size_t len, revo_gps_dev_t *dev_info_p);
revo_u32 revo_gps_reboot_dev(void);
revo_s32 revo_gps_init_dev(void);
void revo_gps_init_dev_check(void);

#endif

