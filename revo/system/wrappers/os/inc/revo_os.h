// Copyright (c) 2020 revoview, Inc. All rights reserved.
//
// This program is confidential and proprietary to revoview electronic communication enterprise, Inc.
// (Revoview), and may not be copied, reproduced, modified, disclosed to others,
// published or used, in whole or in part, without the express prior written
// permission of revoview.


#ifndef __REVO_OS_H__
#define __REVO_OS_H__

#include <stdarg.h>
#include "revo_typedef.h"
#include "priority_system.h"

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************
 *                Enumerations
 *******************************************************/
/**
 * @brief Enumerated list of wait type for sem
 */
typedef enum {
	REVO_NO_WAIT,
	REVO_INFINITE_WAIT
} revo_wait_mode_e;

typedef enum {
	REVO_TASK_NO_PREEMPT,
	REVO_TASK_PREEMPT
} revo_task_preempt_set_e;

typedef enum {
	REVO_TASK_NO_START,
	REVO_TASK_AUTO_START
} revo_task_auto_start_set_e;

typedef enum {
	REVO_TASK_PRIORITY_0 = 27,
	REVO_TASK_PRIORITY_1,
	REVO_TASK_PRIORITY_2,
	REVO_TASK_PRIORITY_3,
	REVO_TASK_PRIORITY_4
} revo_task_priority_set_e;
/*******************************************************
 *                Structures
 *******************************************************/
 /**
 * @brief message struct for revo sdk
 */
typedef void* revo_mutex;    /**< typedef revo_mutex */
typedef void* revo_sem;      /**< typedef revo_sem */
typedef void* revo_cond;     /**< typedef revo_cond */
typedef void* revo_timer_ptr;  
typedef void  (*revo_timer_fun)(revo_u32);



typedef struct {
	revo_char task_name;
	revo_char queue_name;
	void (*entry)(revo_u32, void*);
	revo_u32 argc;
	void *argv;
	revo_u32 stack_size;
	revo_u32 queue_num;
	revo_task_priority_set_e priority;
	revo_task_preempt_set_e preempt;
	revo_task_auto_start_set_e auto_start;
} revo_task_params_t;
/*******************************************************
 *                Function Definitions
 *******************************************************/
/**
 * @brief      create a mutex
 *
 * @return     mutex pointer
 *
 * @note
 *   It is strongly suggested to call this function only in system initialization
 *   time.
 */
revo_mutex revo_mutex_create(void);
/**
 * @brief      destroy a mutex
 *
 * @param[in]  mutex  mutex pointer
 *
 * @return     N/A
 *
 * @note
 *   It is strongly suggested to call this function only in system end
 *   time.
 */
void revo_mutex_destroy(revo_mutex mutex);
/**
 * @brief      lock a mutex
 *             Obtains an instance of the specified mutex. If the mutex is locked already
 *             before this call, the function cannot be immediately satisfied, and caller
 *             task will be suspended until other task give the ownership to caller task.
 *             After the function returns, caller task is the only task held this mutex.
 *
 * @param[in]  mutex  mutex pointer
 *
 * @return     N/A
 *
 * @note
 *             - Mutex is not a recursive lock. If caller task already held this mutex,
 *               then caller task will suspend forever because any task won't and can't
 *               give up mutex's ownership.
 *             - Mutex is a FIFO queue. If multiple tasks are waiting on a mutex, the
 *               first waiting task is selected to be the next owner.
 */
void revo_mutex_lock(revo_mutex mutex);
/**
 * @brief      unlock a mutex
 *             Give up mutex ownership. If any task is waiting on this mutex, the first
 *             waiting task is selected to be the next owner.
 *
 * @param[in]  mutex  mutex pointer
 *
 * @return     N/A
 *
 * @note       Mutex ownership is private to task. That means, give can be done at the same
 *             task of take, otherwise fatal error handling takes place.
 */
void revo_mutex_unlock(revo_mutex mutex);
/**
 * @brief      create a couting semaphore
 *             Give up mutex ownership. If any task is waiting on this mutex, the first
 *             waiting task is selected to be the next owner.
 *
 * @param[in]  initial_count  semaphore initial count
 *
 * @return     semaphore pointer
 *
 * @note       - It is strongly suggested to call this function only in system initialization
 *               time, and the related data allocated for the semaphore could not be freed
 *               once it's created.
 *             - Semaphore values can range from 0 through 4,294,967,294 (2^32 - 2).
 */
revo_sem revo_sem_create(revo_u32 initial_count);
/**
 * @brief      wait a semaphore be availiable
 *             Obtains an instance of the specified semaphore
 *
 * @param[in]  sem  semaphore pointer
 * @param[in]  wait_mode  wait mode, specify the behavior when the semaphore is
 *             not ready immediately, it can be one the the following values:
 *                 REVO_NO_WAIT - don't wait for other task gives ownership to it
 *                 REVO_INFINITE_WAIT - wait until ownership can be satisfied
 *
 * @return     N/A
 *
 * @note       - Semaphore is a FIFO queue. If multiple tasks is waiting on a semaphore,
 *               the first waiting task is selected to be the next owner.
 */
void revo_sem_wait(revo_sem sem, revo_wait_mode_e wait_mode);
/**
 * @brief      give a semaphore
 *             Give up 1 semaphore ownership. If any task is waiting on this semaphore,
 *             give the ownership to first waiting task.
 *
 * @param[in]  sem  semaphore pointer
 *
 * @return     N/A
 *
 */
void revo_sem_signal(revo_sem sem);
/**
 * @brief      malloc memory
 *
 * @param[in]  size size of memory
 *
 * @return     memory start pointer
 *
 */
void *revo_malloc(revo_u32 size);
/**
 * @brief      free memory
 *
 * @param[in]  ptr memory start pointer
 *
 * @return     N/A
 *
 */
void revo_free(void *ptr);
/**
 * @brief      create a new task
 *
 * @param[in]  params task params
 *
 * @return     task id
 *
 */
revo_u32 revo_task_create(revo_task_params_t *params);
/**
 * @brief       send message to specified task
 *
 * @param[in]   dst_task_id  destination task id which message want to send
 * @param[in]   event  event id
 * @param[in]   data   ipc data
 * @param[in]   data_len  ipc data length
 *
 * @return
 *              - REVO_SUCCESS
 *              - REVO_FAIL
 */
revo_s32 revo_task_msg_send(revo_char *dst_task_name, revo_u32 event, void *data, revo_u32 data_len);
/**
 * @brief       receive message
 *
 * @param[in]   rec_task_id  which task, if self, pls use revo_task_self() to get id
 * @param[out]  event  get event id
 * @param[out]  data  get ipc data
 * @param[in]   data  data len you want, will be used to check length
 *
 * @return      - REVO_SUCCESS
 *              - REVO_FAIL
 *
 * @note        - will block until msg arrives
 *
 */
revo_s32 revo_task_msg_recv(revo_char *rec_task_name, revo_u32 *event, void *data, revo_u32 data_len);
/**
 * @brief      get current task id
 *
 * @return     task id
 *
 */
revo_u32 revo_task_self(void);
/**
 * @brief     delay to run current task
 *            put the task into sleep for a specified duration which is expressed in seconds
 *
 * @param[in] ms  delay seconds
 *
 * @return    N/A
 *
 * @note      recommended place to call it is in task main function
 *
 */
void revo_task_delay(revo_u32 ms);
/**
 * @brief     same with revo_task_delay()
*/
void revo_task_sleep(revo_u32 ms);
/**
 * @brief     print info
*/
void revo_printf(const char *fmt, ...);
/**
 * @brief     same with snprintf
*/
revo_s32 revo_snprintf(char *str, const int len, const char *fmt, ...);
/**
 * @brief     same with vsnprintf
*/
revo_s32 revo_vsnprintf(char *str, const int len, const char *format, va_list ap);
/**
 * @brief     update time tick
 * @param[in] ms  N/A
 *
 * @return    time in ms since boot
 *
*/
revo_u32 revo_uptime_ms(void);

/**
*@brief  The function create a period timer with call back function.
*
*@param[in]   timer_name:  Name string of the timer
*@param[in]   timer_fun:   Timer callback function
*@param[in]   input:       Input value for timer callback function             
*@param[in]   timer_expire:  Specifies the timer expire value in milliseconds.
*@param[in]   auto_activate: Option to check if auto active the timer after create.
*
*@return      If successful, returns pointer to the control block of the timer, else return SCI_NULL.
*/
revo_timer_ptr revo_create_period_timer(const char  			*timer_name,
												revo_timer_fun   timer_fun,
												revo_u32        data,
												revo_u32        timer_expire,
												revo_u32        auto_activate
												); 

/**
*@brief       The function changed timer's callback function and expire time.
*
*@param[in]   timer_ptr:    Timer control block
*@param[in]   timer_func:   Timer callback function
*@param[in]   timer_expire: Specifies the expire value in milliseconds.
*
*@return   If successful, returns SCI_SUCCESS, else return SCI_ERROR.
*/
revo_u32 revo_change_timer(revo_timer_ptr  		timer_ptr, 
								  revo_timer_fun    timer_func,
								  revo_u32         timer_expire
								 ); 

/**
*@brief      The function activate a timer created before.
*
*@param[in]  timer_ptr:    Pointer to a previously created application timer.
*
*@return     If active successful, returns SCI_SUCCESS, else return SCI_ERROR.
*/
revo_u32 revo_active_timer(revo_timer_ptr timer_ptr);
 
/**
*@brief      The function deactive a timer created before.
*
*@param[in]  timer_ptr:   Pointer to a previously created application timer.
*
*@return     If successful, returns SCI_SUCCESS, else return SCI_ERROR.
*/
revo_u32 revo_deactive_timer(revo_timer_ptr timer_ptr);
 
/**
*@brief      The function checks if the timer is still active.
*
*@param[in]  timer_ptr:   Pointer to a previously created application timer.
*
*@return     If it is active, returns SCI_TRUE, else return SCI_FALSE. 
*/
revo_u8 revo_is_timer_active(revo_timer_ptr timer_ptr);
 
/**
*@brief      The function deletes a timer created before.
*
*@param[in]  timer_ptr:   Pointer to a previously created application timer. 
*
*@return     If successful, returns SCI_SUCCESS, else return SCI_ERROR.
*/
revo_u32 revo_delete_timer(revo_timer_ptr timer_ptr);


#ifdef __cplusplus
}
#endif
#endif
