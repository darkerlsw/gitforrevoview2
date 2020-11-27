#include "revo_typedef.h"
#include "revo_os.h"
#include "revo_gps_dev.h"
#include "revo_uart.h"
#include "revo_gps_config.h"
#include "revo_log.h"
#include "nmea.h"

static revo_bool g_is_init = REVO_FALSE;

/***************************************
  $GNTXT,GK9501*7C\r\n
  $GNTXT,HW:V1901_B05*1F\r\n
  $GNTXT,SW:V2.0_Jul 13 2020*37\r\n
  $GNTXT,GNSS_Mode:GB*37\r\n
  $GNTXT,NMEA_Format:V3.01(MTK)*25\r\n
****************************************/
static const revo_char *gps_dev_info_head[] = {//gk9501
	"GNTXT",
	"GNTXT,HW:",
	"GNTXT,SW:",
	"GNTXT,GNSS_Mode:",
	"GNTXT,NMEA_Format:",
	NULL //end of list, do not delete it !
};

static const revo_char *gps_init_code[] = {
	"PGKC115,1,0,1,0", //GPS + BeiDou
	"PGKC105,0", //Normal mode
	"PGKC101,5000", //report/5s
	"PGKC242,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0"
};

static const revo_char *gps_reboot_cmd[] = {
	"PGKC030,1,1"
};

static revo_char one_bit_char2str(revo_char a)
{
	if(a < 0 || a >= 16)
		return '0';

	if(a >= 10)
		a = a - 10 + 'A';
	else
		a = a + '0';

	return a;
}

static revo_s32 revo_gps_serialize_nmea_package(const revo_char *in, revo_u32 len, revo_char *out)
{
	revo_char *p = out;
	revo_char cs = 0;
	revo_u32 i;

	if(in == NULL || len == 0 || out == NULL)
	{
		REVO_LOGE(TAG, "%s() Error: invalid input params!", __func__);
		return REVO_FAIL;
	}

	if(len > NMEA_MAX_LENGTH - 7)
	{
		REVO_LOGE(TAG, "%s() Error: out buffer will be out of NMEA_MAX_LENGTH!", __func__);
		return REVO_FAIL;
	}

	*p++ = '$';
	for(i = 0; i < len; i++)
	{
		cs ^= *in;
		*p++= *in++;
	}

	*p++ = '*';
	*p++ = one_bit_char2str(cs >> 4);
	*p++ = one_bit_char2str(cs & 0x0f);
	*p++ = '\r';
	*p++ = '\n';
	*p++ = '\0';

	return REVO_SUCCESS;
}

revo_s32 revo_gps_devinfo_parse(revo_char *in, size_t len, revo_gps_dev_t *dev_info_p)
{
	revo_u32 idx;
	revo_char *p = (revo_char *)dev_info_p;

	for(idx = 0; gps_dev_info_head[idx] != NULL; idx++)
	{
		if(strncmp(in + 1, gps_dev_info_head[idx], strlen(gps_dev_info_head[idx])) == 0)
		{
			in[len - 5] = '\0';
			strcpy(p + idx*REVO_GPS_DEV_INFO_LENGTH, in + strlen(gps_dev_info_head[idx]) + 1);
			REVO_LOGI(TAG, "%s() get dev info, type:%d, $%s%s", __func__, idx, gps_dev_info_head[idx], p + idx*REVO_GPS_DEV_INFO_LENGTH);
			break;
		}
	}

	if(gps_dev_info_head[idx] == NULL)
	{
		REVO_LOGW(TAG, "%s() Warning: unknown sentence!", __func__);
		return REVO_FAIL;
	}
	
	return REVO_SUCCESS;
}

void revo_gps_init_dev_check(void)
{
	if(!g_is_init)
		revo_gps_reboot_dev();
}

revo_u32 revo_gps_reboot_dev(void)
{
	revo_u32 rc;
	revo_char out_buf[NMEA_MAX_LENGTH] = {0};

	g_is_init = REVO_FALSE;
	rc = revo_gps_serialize_nmea_package(gps_reboot_cmd[0], strlen(gps_reboot_cmd[0]), out_buf);
	if(rc == REVO_FAIL)
	{
		REVO_LOGE(TAG, "%s() Error: cannot serialize reboot cmd", __func__);
		return rc;
	}

	REVO_LOGI(TAG, "%s() send reboot cmd, %s, len = %d", __func__, out_buf, strlen(out_buf));
	revo_gps_uart_write(out_buf, strlen(out_buf));
	
	return REVO_SUCCESS;
}

revo_s32 revo_gps_init_dev(void)/*_gk9501*/
{
	revo_u32 rc;
    revo_char i;
    revo_char out_buf[NMEA_MAX_LENGTH] = {0};

    if(g_is_init == REVO_TRUE) 
		return REVO_SUCCESS;

	for (i=0; i< sizeof(gps_init_code)/sizeof(gps_init_code[0]); i++) {
		rc = revo_gps_serialize_nmea_package(gps_init_code[i], strlen(gps_init_code[i]), out_buf);
		if(rc == REVO_FAIL)
		{
			REVO_LOGE(TAG, "%s() Error: cannot serialize init str", __func__);
			return rc;
		}

		REVO_LOGI(TAG, "%s() send init data, %s", __func__, out_buf);
		revo_gps_uart_write(out_buf, strlen(out_buf));
	}
	
	g_is_init = REVO_TRUE;
	
	return REVO_SUCCESS;
}

