#ifndef _REVO_KEYPAD_CONFIG_H
#define _REVO_KEYPAD_CONFIG_H

#ifdef __cplusplus
extern "C"
{
#endif

/* task */
#define REVO_KEYPAD_TASK_NAME           "REVO_KEYPAD_TASK"
#define REVO_KEYPAD_TASK_QNAME          "REVO_KEYPAD_QUEUE"
#define REVO_KEYPAD_TASK_STACK_SIZE     16*1024
#define REVO_KEYPAD_TASK_QUEUE_NUM      50
#define REVO_KEYPAD_TASK_PRIORITY       REVO_TASK_PRIORITY_0



#ifdef __cplusplus
}
#endif

#endif