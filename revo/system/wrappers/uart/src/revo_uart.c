#include "os_api.h"
#include "revo_uart.h"

/* must be match to uart_baud_e */
revo_u32 const revo_uart_baud_tbl[] = {
	0x2A50,	/* UART_BAUD_1200 */
	0x1528,	/* UART_BAUD_2400 */
	0x0A94,	/* UART_BAUD_4800 */
	0x054A,	/* UART_BAUD_9600 */
	0x02A5,	/* UART_BAUD_19200 */
	0x0152,	/* UART_BAUD_38400 */
	0x00E2,	/* UART_BAUD_57600 */
	0x0071,	/* UART_BAUD_115200 */
	0x0038,	/* UART_BAUD_230400 */
	0x001C,	/* UART_BAUD_470800 */
	0x000E,	/* UART_BAUD_921600 */
	0x000D,	/* UART_BAUD_2000000 */
	0x0004,	/* UART_BAUD_3250000 */
	0x0003	/* UART_BAUD_8000000 */
};

extern void SIO_Close (uint32 port);
static void revo_uart_callback_com1(revo_u32 event);
static void revo_uart_callback_com2(revo_u32 event);

revo_uart_map_t revo_uart_callbacks_map_tbl[REVO_UART_COM_MAX] = {
	{REVO_UART_COM1, revo_uart_callback_com1, NULL},
	{REVO_UART_COM2, revo_uart_callback_com2, NULL}
};

static void revo_uart_callback_com1(revo_u32 event)
{
	revo_uart_callbacks_map_tbl[REVO_UART_COM1].custom_callback(event);
}

static void revo_uart_callback_com2(revo_u32 event)
{
	revo_uart_callbacks_map_tbl[REVO_UART_COM2].custom_callback(event);
}

static UartCallback revo_uart_callback_map(uart_com_idx_e com_idx, revo_uart_callback_t callback)
{
	revo_u32 i;
	revo_u32 com_num_in_tbl = sizeof(revo_uart_callbacks_map_tbl) / sizeof(revo_uart_map_t);

	for(i = 0; i < com_num_in_tbl; i++)
	{
		if(revo_uart_callbacks_map_tbl[i].com_idx == com_idx)
			break;
	}

	if(i == com_num_in_tbl)
	{
		revo_printf("[uart]%s() Error: com_idx is not registered in revo_uart_callbacks_map_tbl(static)! com_idx = %d", __func__, com_idx);
		return NULL;
	}

	revo_uart_callbacks_map_tbl[i].custom_callback = callback;
	
	return revo_uart_callbacks_map_tbl[i].system_callback;
}

static revo_s32 revo_uart_callback_unmap(uart_com_idx_e com_idx)
{
	revo_u32 i;
	revo_u32 com_num_in_tbl = sizeof(revo_uart_callbacks_map_tbl) / sizeof(revo_uart_map_t);

	for(i = 0; i < com_num_in_tbl; i++)
	{
		if(revo_uart_callbacks_map_tbl[i].com_idx == com_idx)
			break;
	}

	if(i == com_num_in_tbl)
	{
		revo_printf("[uart]%s() Error: com_idx is not registered in revo_uart_callbacks_map_tbl(static)! com_idx = %d", __func__, com_idx);
		return REVO_FAIL;
	}

	revo_uart_callbacks_map_tbl[i].custom_callback = NULL;
	
	return REVO_SUCCESS;
}

revo_u32 revo_uart_open(uart_com_idx_e com_idx, revo_uart_params_t *params, revo_uart_callback_t callback)
{
	revo_u32 ret;
	UART_INIT_PARA_T uart_st;
	UartCallback mapped_callback;


	uart_st.baud_rate     = revo_uart_baud_tbl[params->baud_rate];
	uart_st.parity        = params->parity; 
	uart_st.parity_enable = params->parity_enable;
	uart_st.byte_size     = params->byte_size;
	uart_st.stop_bits     = params->stop_bit;
	uart_st.flow_control  = params->flow_ctl;
	
	uart_st.tx_watermark  = 96; // 0~127B
	uart_st.rx_watermark  = 64; // 0~127B
	uart_st.ds_wakeup_en  = DS_WAKEUP_DISABLE;

	if(callback != NULL)
	{
		mapped_callback = revo_uart_callback_map(com_idx, callback);
		if(mapped_callback == NULL)
		{
			revo_printf("[uart]%s() Error: cannot map callback function to system!", __func__);
			return REVO_FAIL;
		}
	}

	UART_Close(com_idx);
	ret = UART_Initilize(com_idx, &uart_st, mapped_callback);
	if(ret != 0)
	{
		revo_printf("[uart]%s() Error: init uart fail! error code = %d", __func__, ret);
		if(callback != NULL)
			revo_uart_callback_unmap(com_idx);
		
		return REVO_FAIL;
	}
	
	UART_Tx_Int_Enable(com_idx, REVO_FALSE);
	if(callback == NULL)
		UART_Rx_Int_Enable(com_idx, REVO_FALSE);

	return REVO_SUCCESS;
}

revo_u32 revo_uart_close(uart_com_idx_e com_idx)
{
	revo_u32 ret;

	revo_uart_callback_unmap(com_idx);
	ret = UART_Close(com_idx);
	if(ret != 0)
	{
		revo_printf("[uart]%s() Error: close uart fail! error code = %d", __func__, ret);
	}

	return REVO_SUCCESS;
}

revo_u32 revo_uart_read(uart_com_idx_e com_idx, char *buf, revo_u32 len)
{
	return UART_ReadData(com_idx, buf, len);
}

revo_u32 revo_uart_write(uart_com_idx_e com_idx, char *buf, revo_u32 len)
{
	return UART_WriteData(com_idx, buf, len);
}

revo_u32 revo_uart_rx_fifo_cnt(uart_com_idx_e com_idx)
{
	return UART_GetRxFifoCnt(com_idx);
}

revo_u32 revo_uart_tx_fifo_cnt(uart_com_idx_e com_idx)
{
	return UART_GetTxFifoCnt(com_idx);
}

