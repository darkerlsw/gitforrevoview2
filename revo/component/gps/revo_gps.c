#include "revo_log.h"
#include "revo_uart.h"
#include "revo_gps.h"
#include "revo_gps_dev.h"
#include "revo_gps_config.h"

revo_u32 g_revo_gps_task_id;
revo_gps_ctx_t g_revo_gps_ctx = {0};

static revo_gps_ctx_t *revo_gps_get_ctx(void)
{
	return &g_revo_gps_ctx;
}

static revo_gps_nmea_parser_t *revo_gps_get_nmea_ctx(void)
{
	return &(g_revo_gps_ctx.nmea_info);
}

static revo_gps_dev_t *revo_gps_get_dev_ctx(void)
{
	return &(g_revo_gps_ctx.dev_info);
}

static revo_gps_app_info_t *revo_gps_get_app_ctx(void)
{
	return &(g_revo_gps_ctx.app_info);
}

static void revo_gps_ctx_init(void)
{
	revo_gps_ctx_t *ctx = revo_gps_get_ctx();

	memset(ctx, 0, sizeof(revo_gps_ctx_t));
}

static revo_bool revo_gps_fill_devinfo(revo_char *buf, revo_u32 len)
{
	revo_gps_dev_t *ctx = revo_gps_get_nmea_ctx();

	return revo_gps_devinfo_parse(buf, len, ctx);
}

static void revo_gps_fill_appinfo(revo_gps_nmea_type_e type)
{
	revo_gps_app_info_t *ctx = revo_gps_get_app_ctx();
	revo_gps_nmea_parser_t *nmea_ctx = revo_gps_get_nmea_ctx();
	revo_gps_nmea_parser_u *p;

	switch(type)
	{
		case REVO_GPS_TYPE_GGA:
			p = &revo_gps_get_nmea_ctx()->gga_s;
			if(p->gga_s.fix_status)
			{
				//time
				//memcpy(dctx->time, sctx->time, sizeof(struct tm));
				ctx->time = p->gga_s.time;
				//lat
				ctx->lat = p->gga_s.latitude.degrees + p->gga_s.latitude.minutes / 60.0;
				ctx->lat_cardinal = p->gga_s.latitude.cardinal;
				//lng
				ctx->lng = p->gga_s.longitude.degrees + p->gga_s.longitude.minutes / 60.0;
				ctx->lng_cardinal = p->gga_s.longitude.cardinal;
				ctx->hdop = p->gga_s.hdop;
				ctx->num_sat = p->gga_s.n_satellites;
			}
			break;
			
		case REVO_GPS_TYPE_GLL:
			break;

		case REVO_GPS_TYPE_GSA:
			break;
			
		case REVO_GPS_TYPE_GSV:
			break;
			
		case REVO_GPS_TYPE_RMC:
			break;
			
		case REVO_GPS_TYPE_BDGSA:
			break;

		case REVO_GPS_TYPE_GPGSA:
			break;

		default:
			break;
	}
}


static revo_s32 revo_gps_nmea_decode(revo_char *buf, revo_u32 len)
{
	nmea_s *data;
	revo_gps_nmea_parser_t *ctx = revo_gps_get_nmea_ctx();

	REVO_LOGI(TAG, "%s() buf = %s", __func__, buf);

	data = nmea_parse(buf, len, 1);
	if(data == NULL)
	{
		if(revo_gps_fill_devinfo(buf, len) == REVO_SUCCESS)
		{
			revo_gps_init_dev();
		}else{
			return REVO_FAIL;
		}
	}

	switch(data->type)
	{
		case NMEA_GLL:
			REVO_LOGI(TAG, "%s() get GLL", __func__);
			ctx->gll_s = *(nmea_gll_s *) data;
			revo_gps_fill_appinfo(REVO_GPS_TYPE_GLL);
			break;

		case NMEA_GGA:
			REVO_LOGI(TAG, "%s() get GGA", __func__);
			ctx->gga_s = *(nmea_gga_s *) data;
			revo_gps_fill_appinfo(REVO_GPS_TYPE_GGA);
			break;

		case NMEA_RMC:
			REVO_LOGI(TAG, "%s() get RMC", __func__);
			ctx->rmc_s = *(nmea_rmc_s *) data;
			revo_gps_fill_appinfo(REVO_GPS_TYPE_RMC);
			break;
			
		case NMEA_GSV:
			REVO_LOGI(TAG, "%s() get GSV", __func__);
			ctx->gsv_s = *(nmea_gsv_s *) data;
			revo_gps_fill_appinfo(REVO_GPS_TYPE_GSV);
			break;

		case NMEA_GPGSA:
			REVO_LOGI(TAG, "%s() get GPGSA", __func__);
			ctx->gpgsa_s = *(nmea_gpgsa_s *) data;
			revo_gps_fill_appinfo(REVO_GPS_TYPE_GPGSA);
			break;
			
		case NMEA_BDGSA:
			REVO_LOGI(TAG, "%s() get BDGSA", __func__);
			ctx->bdgsa_s = *(nmea_bdgsa_s *) data;
			revo_gps_fill_appinfo(REVO_GPS_TYPE_BDGSA);
			break;
			
		default:
			break;
	}
	nmea_free(data);
}

static void revo_gps_task(uint32 argc, void *argv)
{
#define REVO_GPS_REC_BYTES_MAX 256
	revo_u32 ret;
	revo_char data;
	revo_char ubuf[REVO_GPS_REC_BYTES_MAX] = {0};
	revo_char ubuf_offset = 0;
	revo_u32 time_cnt = 1;

	revo_gps_info_t gdata;
	double kk;
	char tmp[128] = 0;

	/* gps context init */
	revo_gps_ctx_init();

	/* gps parser init */
	nmea_init();

	/* gps uart init */	
	ret = revo_gps_uart_init();
	if(ret == REVO_FAIL)
	{
		REVO_LOGE(TAG, "%s() Error: uart init fail! Thread will be killed!", __func__);
		return;
	}

	/* gps dev reboot(always on) */
	ret = revo_gps_reboot_dev();
	if(ret == REVO_FAIL)
	{
		REVO_LOGE(TAG, "%s() Error: gps reboot fail! Thread will be killed!", __func__);
		return;
	}
	
	while(1)
	{
		while(revo_gps_uart_read_char(&data) == REVO_SUCCESS)
		{
			if(data == '$')
			{
				memset(ubuf, 0, REVO_GPS_REC_BYTES_MAX);
				ubuf_offset = 0;
			}

			if(ubuf_offset >= REVO_GPS_REC_BYTES_MAX)
			{
				memset(ubuf, 0, REVO_GPS_REC_BYTES_MAX);
				ubuf_offset = 0;
			}
			
			ubuf[ubuf_offset] = data;

			if(data == '\n')
			{
				if(ubuf[ubuf_offset - 1] == '\r' && ubuf[0] == '$')
				{
					revo_gps_nmea_decode(ubuf, ubuf_offset + 1);
				}else{
					memset(ubuf, 0, REVO_GPS_REC_BYTES_MAX);
					ubuf_offset = 0;
				}
			}

			ubuf_offset++;
		}

		if(time_cnt++ % 500 == 0)
		{
			revo_gps_init_dev_check();
			revo_gps_read(REVO_GPS_LONGITUDE, &gdata);
			revo_gps_read(REVO_GPS_LATITUDE, &gdata);
		}
		
		revo_task_sleep(10);
	}
}

static void revo_gps_task_create(void)
{
	revo_task_params_t task_params;

	task_params.task_name = REVO_GPS_TASK_NAME;
	task_params.queue_name = REVO_GPS_TASK_QNAME;
	task_params.entry = revo_gps_task;
	task_params.argc = 0;
	task_params.argv = 0;
	task_params.stack_size = REVO_GPS_TASK_STACK_SIZE;
	task_params.queue_num = REVO_GPS_TASK_QUEUE_NUM;
	task_params.priority = REVO_GPS_TASK_PRIORITY;
	task_params.preempt = REVO_TASK_PREEMPT;
	task_params.auto_start = REVO_TASK_AUTO_START;
	g_revo_gps_task_id = revo_task_create(&task_params);
}

void revo_gps_read(revo_gps_info_type_e type, revo_gps_info_t *data)
{
	revo_gps_app_info_t *ctx = revo_gps_get_app_ctx();

	memset(data, 0, sizeof(revo_gps_info_t));
	switch(type)
	{
		case REVO_GPS_LONGITUDE:
			data->ld.lng = ctx->lng;
			sprintf(data->str,"%lf", ctx->lng);
			REVO_LOGI(TAG, "%s() type:%d, lng:%s", __func__, REVO_GPS_LONGITUDE, data->str);
			break;

		case REVO_GPS_LATITUDE:
			data->ld.lat = ctx->lat;
			sprintf(data->str,"%lf", ctx->lat);
			REVO_LOGI(TAG, "%s() type:%d, lat:%s", __func__, REVO_GPS_LATITUDE, data->str);
			break;

		case REVO_GPS_LONGITUDE_D:
			data->ld.lng_cardinal = ctx->lng_cardinal;
			revo_snprintf(data->str, REVO_GPS_STR_INFO_LEN, "%c", ctx->lng_cardinal);
			break;

		case REVO_GPS_LATITUDE_D:
			data->ld.lat_cardinal = ctx->lat_cardinal;
			revo_snprintf(data->str, REVO_GPS_STR_INFO_LEN, "%c", ctx->lat_cardinal);
			break;

		case REVO_GPS_DATE:
			data->ld.time = ctx->time;
			revo_snprintf(data->str, REVO_GPS_STR_INFO_LEN, "%d-%d-%d", ctx->time.tm_hour, ctx->time.tm_min, ctx->time.tm_sec);
			break;

		case REVO_GPS_SAT_NUM:
			data->ld.num_sat = ctx->num_sat;
			revo_snprintf(data->str, REVO_GPS_STR_INFO_LEN, "%d", ctx->num_sat);
			break;
		
		default:
			break;
	}

	return;
}

void revo_gps_open(void)
{
	revo_gps_ctx_t *ctx = revo_gps_get_ctx();

	if(ctx->is_open == REVO_TRUE)
	{
		REVO_LOGE(TAG, "%s() Error: do not reopen the gps!", __func__);
		return;
	}

	revo_gps_task_create();
	
	ctx->is_open = REVO_TRUE;

	return;
}

