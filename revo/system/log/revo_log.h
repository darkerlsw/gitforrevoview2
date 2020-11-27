#ifndef __REVO_LOG_H__
#define __REVO_LOG_H__

#include <stdarg.h>
#include "revo_log_config.h"

typedef enum {
	REVO_LOG_LEVEL_ERROR,
	REVO_LOG_LEVEL_WARNING,
	REVO_LOG_LEVEL_INFO
} revo_log_level_e;

revo_u32 revo_log_init(void);
void revo_log(const char *tag, const char *fmt, ...);

#define REVO_LOGI(tag, fmt, ...) \
do{ \
	if(REVO_LOG_LEVEL >= REVO_LOG_LEVEL_INFO) \
		revo_log(tag, fmt, ##__VA_ARGS__); \
}while(0)

#define REVO_LOGW(tag, fmt, ...) \
do{ \
	if(REVO_LOG_LEVEL >= REVO_LOG_LEVEL_WARNING) \
		revo_log(tag, fmt, ##__VA_ARGS__); \
}while(0)

#define REVO_LOGE(tag, fmt, ...) \
do{ \
	if(REVO_LOG_LEVEL >= REVO_LOG_LEVEL_ERROR) \
		revo_log(tag, fmt, ##__VA_ARGS__); \
}while(0)

#endif
