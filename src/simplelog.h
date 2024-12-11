/*
//================================================================================================================
// Email:														
//		<nguyenthaithuanalg@gmail.com> - Nguyễn Thái Thuận
// Date:														
//		<2024-July-14>
// The lasted modified date:									
//		<2024-Aug-22>
// Decription:													
//		The (only) main header file to export 3 APIs: [spl_init_log, spllog, spllogtopic, spl_finish_log].
//===============================================================================================================
*/
#ifndef ___SIMPLE_LOG__
#define ___SIMPLE_LOG__
#include <stdio.h>
#include "simplelog_config.h"

#ifdef __cplusplus
extern "C" {
#endif

#define LLU				unsigned long long

#define					SPL_LOG_BASE					0
#define					SPL_LOG_DEBUG					1
#define					SPL_LOG_INFO					2
#define					SPL_LOG_WARNING					3
#define					SPL_LOG_ERROR					4
#define					SPL_LOG_FATAL					5
#define					SPL_LOG_PEAK					6

#ifndef  UNIX_LINUX
	#ifndef __SIMPLE_STATIC_LOG__
		#ifdef EXPORT_DLL_API_SIMPLE_LOG
			#define DLL_API_SIMPLE_LOG		__declspec(dllexport)
		#else
			#define DLL_API_SIMPLE_LOG		__declspec(dllimport)
		#endif
	#else
	#define DLL_API_SIMPLE_LOG
	#endif
#else
	#define DLL_API_SIMPLE_LOG
#endif /*! UNIX_LINUX */ 


	typedef enum __SPL_LOG_ERR_CODE__ {
		SPL_NO_ERROR = 0,
		SPL_INIT_PATH_FOLDER_EMPTY_ERROR,
		SPL_LOG_LEVEL_ERROR,
		SPL_ERROR_CREATE_MUTEX,
		SPL_ERROR_CREATE_SEM,
		SPL_LOG_BUFF_SIZE_ERROR,
		SPL_LOG_FOLDER_ERROR,
		SPL_LOG_CREATE_THREAD_ERROR,
		SPL_LOG_FMT_NULL_ERROR,
		SPL_LOG_MEM_GEN_FILE_ERROR,
		SPL_LOG_MEM_MALLOC_ERROR,
		SPL_LOG_OPEN_FILE_ERROR,
		SPL_LOG_OPEN1_FILE_ERROR,
		SPL_LOG_CLOSE_FILE_ERROR,
		SPL_LOG_SEM_NULL_ERROR,
		SPL_LOG_ROT_SIZE_ERROR,
		SPL_LOG_TOPIC_EMPTY,
		SPL_LOG_TOPIC_NULL,
		SPL_LOG_MEM_FILE_MALLOC_ERROR,
		SPL_LOG_CHECK_FOLDER_ERROR,
		SPL_LOG_CHECK_FOLDER_YEAR_ERROR,
		SPL_LOG_CHECK_FILE_YEAR_ERROR,
		SPL_LOG_CHECK_FOLDER_NULL_ERROR,
		SPL_LOG_MUTEX_NULL_ERROR,
		SPL_LOG_ST_NAME_NULL_ERROR,
		SPL_LOG_TIME_NULL_ERROR,
		SPL_LOG_TIME_NANO_NULL_ERROR, 
		SPL_LOG_STAT_FOLDER_ERROR,
		SPL_LOG_PRINTF_ERROR,
		SPL_LOG_TOPIC_ZERO,
		SPL_LOG_TOPIC_MEMORY,
		SPL_LOG_TOPIC_FOPEN,
		SPL_LOG_TOPIC_FLUSH,
		SPL_LOG_TOPIC_BUFF_MEM,
		SPL_LOG_SHM_OUT_NULL,
		SPL_LOG_SHM_MEM_ERROR,
		SPL_LOG_SHM_KEY_NULL,
		SPL_LOG_SHM_MAPPING_NULL,
		SPL_LOG_SHM_MAP_VIEW_NULL,
		SPL_LOG_SHM_MODE,
		SPL_LOG_SHM_UNIX_CREATE,
		SPL_LOG_SHM_UNIX_OPEN,
		SPL_LOG_SHM_UNIX_TRUNC,
		SPL_LOG_SHM_UNIX_MAP_FAILED,
		SPL_LOG_SHM_UNIX_ATT_INIT_MUTEX,
		SPL_LOG_SHM_UNIX_ATT_INIT_MUTEX_SET,
		SPL_LOG_SHM_UNIX_INIT_MUTEX,
		SPL_LOG_SHM_UNIX_MUTEX_SHM_NULL,
		SPL_LOG_SHM_UNIX_SEM_SHM_NULL,
		SPL_LOG_SHM_UNIX_SEM_SHM_INIT,


		SPL_END_ERROR,
	} SPL_LOG_ERR_CODE;



#define spl_console_log(___fmttt___, ...)		{char buf[1024];spl_fmmt_now(buf, 1024);\
fprintf(stdout, "[%s] [%s:%d] [pid: %llu, thid: %llu] "___fmttt___"\n" , buf, __FUNCTION__, __LINE__, spl_process_id(), spl_get_threadid(), ##__VA_ARGS__);}





#define __spl_log_buf__(___fmttt___, ...)	{int *__ppl = 0; char tnow[40]; int range=0; char* __p = 0; void *__mtx__ =  spl_get_mtx(); LLU thrid = spl_get_threadid();\
int len = 0; LLU prid = spl_process_id(); spl_fmt_now(tnow, 40);\
spl_mutex_lock(__mtx__);\
__p = spl_get_buf(&range, &__ppl); if (__p && __ppl) { len = snprintf((__p + (*__ppl)), range, \
"[%s] [pid: %llu, tid: %llu] [%s:%d] "___fmttt___"\n\n", \
tnow, prid, thrid, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
if(len > 0) (*__ppl) += (len -1);}\
spl_mutex_unlock(__mtx__); spl_rel_sem(spl_get_sem_rwfile());}


#define __spl_log_buf_level__(__lv__, ___fmttt___, ...)	{if(spl_get_log_levwel() <= (__lv__) )\
{int *__ppl = 0; char tnow[40]; int range=0; char* __p = 0; void *__mtx__ =  spl_get_mtx(); LLU thrid = spl_get_threadid();\
int len = 0; const char *lv_text = spl_get_text(__lv__);LLU prid = spl_process_id(); spl_fmt_now(tnow, 40);\
spl_mutex_lock(__mtx__);\
__p = spl_get_buf(&range, &__ppl); if (__p && __ppl) { len = snprintf((__p + (*__ppl)), range, \
"[%s] [%s] [pid: %llu, tid:%llu]\t[%s:%d] "___fmttt___"\n\n", \
tnow, lv_text, prid, thrid, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
if(len > 0) (*__ppl) += (len -1);}\
spl_mutex_unlock(__mtx__); spl_rel_sem(spl_get_sem_rwfile());}\
}



//#define __spl_log_buf_level__(__lv__, ___fmttt___, ...)	{if(spl_get_log_levwel() <= (__lv__) )\
//{int *__ppl = 0; char tnow[40]; int range=0; char* __p = 0; void *__mtx__ =  spl_get_mtx(); LLU thrid = spl_get_threadid();\
//int len = 0; const char *lv_text = spl_get_text(__lv__);spl_fmt_now(tnow, 40);\
//spl_mutex_lock(__mtx__);\
//__p = spl_get_buf(&range, &__ppl); if (__p && __ppl) { len = snprintf((__p + (*__ppl)), range, \
//"[%s] [%s] [tid:\t%llu]\t[%s:%d]\t"___fmttt___"\n\n", \
//tnow, lv_text, thrid, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
//if(len > 0) (*__ppl) += (len -1);}\
//spl_mutex_unlock(__mtx__); spl_rel_sem(spl_get_sem_rwfile());}\
//}



#define __spl_log_buf_topic__(__tpic, ___fmttt___, ...)	{int *__ppl = 0; char tnow[40]; int range=0; char* __p = 0; void *__mtx__ =  spl_get_mtx(); LLU thrid = spl_get_threadid();\
int len = 0; LLU prid = spl_process_id();;spl_fmt_now(tnow, 40);\
spl_mutex_lock(__mtx__);\
__p = spl_get_buf_topic(&range, &__ppl, (__tpic)); if (__p && __ppl) { len = snprintf((__p + (*__ppl)), range, \
"[%s] [pid: %llu, tid: %llu] [%s:%d] "___fmttt___"\n\n", \
tnow, prid, thrid, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
if(len > 0) (*__ppl) += (len -1);}\
spl_mutex_unlock(__mtx__); spl_rel_sem(spl_get_sem_rwfile());}





#define __spl_log_buf_topic_level__(__lv__, __tpic, ___fmttt___, ...)	{if(spl_get_log_levwel() <= (__lv__) )\
{int *__ppl = 0; char tnow[40]; int range=0; char* __p = 0; void *__mtx__ =  spl_get_mtx(); LLU thrid = spl_get_threadid();\
int len = 0; const char *lv_text = spl_get_text(__lv__);LLU prid = spl_process_id();;spl_fmt_now(tnow, 40);\
spl_mutex_lock(__mtx__);\
__p = spl_get_buf_topic(&range, &__ppl, (__tpic)); if (__p && __ppl) { len = snprintf((__p + (*__ppl)), range, \
"[%s] [%s] [pid: %llu, tid: %llu]\t[%s:%d] "___fmttt___"\n\n", \
tnow, lv_text, prid, thrid, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
if(len > 0) (*__ppl) += (len -1);}\
spl_mutex_unlock(__mtx__); spl_rel_sem(spl_get_sem_rwfile());}\
}




//#define __spl_log_buf_topic_level__(__lv__, __tpic, ___fmttt___, ...)	{ if(spl_get_log_levwel() <= (__lv__) ) \
//{int *__ppl = 0; char tnow[40]; int range=0; char* __p = 0; void *__mtx__ =  spl_get_mtx(); LLU thrid = spl_get_threadid();\
//int len = 0; const char *lv_text = spl_get_text(__lv__);spl_fmt_now(tnow, 40);\
//spl_mutex_lock(__mtx__);\
//__p = spl_get_buf_topic(&range, &__ppl, (__tpic)); if (__p && __ppl) { len = snprintf((__p + (*__ppl)), range, \
//"[%s] [%s] [tid:\t%llu]\t[%s:%d]\t"___fmttt___"\n\n", \
//tnow, lv_text, thrid, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
//if(len > 0) (*__ppl) += (len -1);}\
//spl_mutex_unlock(__mtx__); spl_rel_sem(spl_get_sem_rwfile());}\
//}



//#define spllog(__lv__, __fmtt__, ...) { if(spl_get_log_levwel() <= (__lv__) ) {__spl_log_buf__("[%s] -->> "__fmtt__, spl_get_text(__lv__), ##__VA_ARGS__);};}
#define spllog			__spl_log_buf_level__
		
//#define spllogtopic(__lv__, __tpic, __fmtt__, ...) \
//{ if(spl_get_log_levwel() <= (__lv__) ) {__spl_log_buf_topic__((__tpic), "[%s] -->> "__fmtt__, spl_get_text(__lv__), ##__VA_ARGS__);};}

#define spllogtopic		__spl_log_buf_topic_level__

	
DLL_API_SIMPLE_LOG int									
	spl_set_log_levwel(int val);
DLL_API_SIMPLE_LOG int									
	spl_get_log_levwel();
DLL_API_SIMPLE_LOG int									
	spl_init_log(char *path, int creating);
DLL_API_SIMPLE_LOG int									
	spl_finish_log(int);
DLL_API_SIMPLE_LOG int									
	spl_fmt_now(char* fmtt, int len);
DLL_API_SIMPLE_LOG int									
	spl_fmmt_now(char* fmtt, int len);
DLL_API_SIMPLE_LOG int									
	spl_mutex_lock(void* mtx);
DLL_API_SIMPLE_LOG int									
	spl_mutex_unlock(void* mtx);
DLL_API_SIMPLE_LOG int									
	spl_set_off(int, int);
DLL_API_SIMPLE_LOG int									
	spl_get_off();
DLL_API_SIMPLE_LOG void*								
	spl_get_mtx();
DLL_API_SIMPLE_LOG void*								
	spl_get_sem_rwfile();
DLL_API_SIMPLE_LOG LLU									
	spl_get_threadid();
DLL_API_SIMPLE_LOG int									
	spl_rel_sem(void* sem);
DLL_API_SIMPLE_LOG const char*							
	spl_get_text(int lev);
DLL_API_SIMPLE_LOG char *								
	spl_get_buf(int* n, int** ppl);
DLL_API_SIMPLE_LOG char*
	spl_get_buf_topic(int* n, int** ppl, int );
DLL_API_SIMPLE_LOG 
	void* spl_mutex_create(char *);
DLL_API_SIMPLE_LOG
	void spl_sleep(unsigned  int);
DLL_API_SIMPLE_LOG
	int spl_standardize_path(char* fname);
DLL_API_SIMPLE_LOG
	LLU spl_milli_now();
DLL_API_SIMPLE_LOG
	LLU spl_process_id();
#ifdef __cplusplus
}
#endif
#endif