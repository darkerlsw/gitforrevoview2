#include "revo_os.h"
#include "os_api.h"

#define REVO_MUTEX_NAME  "REVO_%d"
static int cnt = 0;

/* MUTEX */
void *revo_mutex_create(void)
{
	revo_char str[10] = {0};
	
	sprintf(str, REVO_MUTEX_NAME, cnt++);
	
	return SCI_CreateMutex(str, SCI_INHERIT);
}

void revo_mutex_destroy(revo_mutex mutex)
{
	if (mutex == NULL)
	{
		SCI_TRACE_LOW("%s() revo_mutex_destroy mutex is null!", __func__);
		return;
	}
	
	SCI_DeleteMutex(mutex);
}

void revo_mutex_lock(revo_mutex mutex)
{
	revo_u32 status = 0;

	if (mutex == SCI_NULL)
	{
		SCI_TRACE_LOW("%s() mutex is null!", __func__);
		return;
	}

	status = SCI_GetMutex(mutex,SCI_WAIT_FOREVER);
	if(SCI_SUCCESS != status)
	{
		SCI_TRACE_LOW("%s() GetMutex failed!", __func__);
	}
}

void revo_mutex_unlock(revo_mutex mutex)
{
    if (mutex == SCI_NULL)
    {
        SCI_TRACE_LOW("%s() mutex is null!", __func__);
        return;
    }
    SCI_PutMutex(mutex);
}

/* SEMAPHORE */
revo_sem revo_sem_create(revo_u32 initial_count)
{
	return NULL;
}

void revo_sem_wait(revo_sem sem, revo_wait_mode_e wait_mode)
{
	return;
}

void revo_sem_signal(revo_sem sem)
{
	return;
}

/* TASK */
void revo_task_delay(revo_u32 ms)
{
	SCI_Sleep(ms);
}

void revo_task_sleep(revo_u32 ms)
{
	SCI_Sleep(ms);
}

revo_u32 revo_task_create(revo_task_params_t *params)
{
	return SCI_CreateAppThread(
					params->task_name, 
					params->queue_name, 
					params->entry, 
					params->argc, 
					params->argv, 
					params->stack_size, 
					params->queue_num, 
					params->priority, 
					params->preempt, 
					params->auto_start);
}

extern revo_u32 revo_get_thread_id(revo_char *name);


revo_s32 revo_task_msg_send(revo_char *dst_task_name, revo_u32 event, void *data, revo_u32 data_len)
{
	xSignalHeaderRec *p = NULL; 
	revo_u32 dst_task_id = revo_get_thread_id(dst_task_name);
	p = (xSignalHeaderRec *)SCI_ALLOC_APP(sizeof(xSignalHeaderRec) + data_len);
	
	if(p == NULL)
	{
		revo_printf("%s() Error: malloc fail!");
		return REVO_FAIL;
	}
	
	p->SignalCode = event;
	p->SignalSize = sizeof(xSignalHeaderRec) + data_len;
	p->Sender = SCI_IdentifyThread();
	if(data_len > 0 && data != NULL)
		memcpy((char *)(p+1), (revo_char *)data, data_len);
	SCI_SendSignal(p, dst_task_id);
	revo_printf("%s() dst_task_id = %d , event = %x", dst_task_id, event);
	
	return REVO_SUCCESS;	
}

revo_s32 revo_task_msg_recv(revo_char *rec_task_name, revo_u32 *event, void *data, revo_u32 data_len)
{
	xSignalHeaderRec *signal = NULL;
	revo_u32 rec_task_id = revo_get_thread_id(rec_task_name);
	revo_u32 len = 0;

	signal = SCI_GetSignal(rec_task_id);
	*event = signal->SignalCode;
	len = signal->SignalSize;

	if(data != NULL && data_len > 0)
	{
		/* check */
		if(len != (sizeof(xSignalHeaderRec) + data_len))
		{
			revo_printf("%s() Error: data_len is not match");
			return REVO_FAIL;
		}

		memcpy((revo_char *)data, (revo_char *)(signal+1), data_len);
	}

	SCI_Free(signal);
	
	return REVO_SUCCESS;
}

revo_u32 revo_task_self(void)
{
	return SCI_IdentifyThread();
}

/* MEMORY */
void *revo_malloc(revo_u32 size)
{
    return malloc(size);
}

void revo_free(void *ptr)
{
    free(ptr);
}

/* DEBUG */
static void _in_batches_printf(char *buf, char *tag)
{
    char tmp[101] = {0};
    char il = 0;
    int i = 0, len;

    len = strlen(buf); 
    while(len)
    {
        if(len > 100)
        {
            strncpy(tmp, buf, 100);
            tmp[100] = 0;
            len -= 100;
            buf += 100;
            if(!il)
                il = 1;
        }else{
            strncpy(tmp, buf, len);
            tmp[len] = 0;
            len = 0;
        }
        if(il)
            SCI_TRACE_LOW("%s[P%d]%s", tag, i++, tmp);
        else
            SCI_TRACE_LOW("%s%s", tag, tmp);
    }
}

void revo_printf(const char *fmt, ...)
{
#define REVO_LOG_BUF_SIZE 512
#define REVO_LOG_TAG "[REVO]"
    revo_u32 len;
    revo_char buf[REVO_LOG_BUF_SIZE];
    va_list args;

    memset(buf, 0, REVO_LOG_BUF_SIZE);
    va_start(args, fmt);
    len = vsnprintf(buf, REVO_LOG_BUF_SIZE, fmt, args);
    if(len >= REVO_LOG_BUF_SIZE)
        SCI_TRACE_LOW(REVO_LOG_TAG"ERROR:%s print buf outsize!", __func__);
    _in_batches_printf(buf, REVO_LOG_TAG);
    va_end(args);
}

/* STRINGS */
revo_s32 revo_snprintf(char *str, const int len, const char *fmt, ...)
{
    va_list args;
    revo_s32 ret;
	
    va_start(args, fmt);
    ret = vsnprintf(str, len, fmt, args);
    va_end(args);
	
    return (int)ret;
}

revo_s32 revo_vsnprintf(char *str, const int len, const char *format, va_list ap)
{
    return vsnprintf(str, len, format, ap);
}

/* TICK */
revo_u32 revo_uptime_ms(void)
{
    revo_u32 time_ms;
    time_ms = SCI_GetTickCount();
    return time_ms;
}

/*TIMER*/
revo_timer_ptr revo_create_period_timer(const char  *timer_name,
												revo_timer_fun   timer_fun,
												revo_u32       data,
												revo_u32      timer_expire,
												revo_u32      auto_activate
												) 
{
	return SCI_CreatePeriodTimer(timer_name, 
								 timer_fun,
								 data,
								 timer_expire,
								 auto_activate
								 );
}

revo_u32 revo_change_timer(revo_timer_ptr timer_ptr, 
								  revo_timer_fun timer_func,
								  revo_u32 timer_expire
								 ) 
{
	return SCI_ChangeTimer(timer_ptr, timer_func, timer_expire);
}

revo_u32 revo_active_timer(revo_timer_ptr timer_ptr) {
	return SCI_ActiveTimer(timer_ptr);
}

revo_u32 revo_deactive_timer(revo_timer_ptr timer_ptr) {
	return SCI_DeactiveTimer(timer_ptr);
}

revo_u8 revo_is_timer_active(revo_timer_ptr timer_ptr) {
	return SCI_IsTimerActive(timer_ptr);
}

revo_u32 revo_delete_timer(revo_timer_ptr timer_ptr) {
	return SCI_DeleteTimer(timer_ptr);
}
















