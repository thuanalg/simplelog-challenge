/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
/* Email:
 *		<nguyenthaithuanalg@gmail.com> - Nguyễn Thái Thuận
 * Mobile:
 *		<+084.799.324.179>
 * Skype:
 *		<nguyenthaithuanalg>
 * Date:
 *		<2024-July-14>
 * The lasted modified date:
 *		<2024-Sep-14>
 *		<2024-Dec-18>
 *		<2024-Dec-20>
 *		<2024-Dec-22>
 *		<2024-Dec-23>
 *		<2024-Dec-30>
 *		<2025-Jan-06>
 *		<2025-Jan-08>
 *		<2025-Jan-10>
 *		<2025-Jan-18>
 *		<2025-Feb-04>
 *		<2025-Apr-12>
 *		<2025-Apr-20>
 *		<2025-Apr-22>
 *		<2025-May-29>
 *		<2025-Jun-01>
 *		<2025-Jun-04>
 * Decription:
 *		The (only) main header file to export 4 APIs: [spc_init_log_ext,
 *spclog, spclogtopic, spc_finish_log].
 */
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
#ifndef ___SPC_SIMEPLE_LOG__
#define ___SPC_SIMEPLE_LOG__            
#include <stdio.h>
#include <string.h>
/*strrchr*/

#if 0
#ifndef UNIX_LINUX
#define UNIX_LINUX                      
#endif
#endif

#if 0
#ifndef SPC_USING_SPIN_LOCK
#define SPC_USING_SPIN_LOCK             
#endif /* !SPC_USING_SPIN_LOCK */
#endif

#if 0
#define __UNIX_LINUX_CPP11_AND_NEWERS__ 
#endif

#ifndef __UNIX_LINUX_CPP11_AND_NEWERS__
#else
#include <string>
#endif
#ifdef __cplusplus
extern "C" {
#endif

#ifndef SPC_LLU
#define SPC_LLU                         unsigned long long
#endif

#define SPC_LOG_BASE                    0
#define SPC_LOG_DEBUG                   1
#define SPC_LOG_INFO                    2
#define SPC_LOG_WARNING                 3
#define SPC_LOG_ERROR                   4
#define SPC_LOG_FATAL                   5
#define SPC_LOG_PEAK                    6

#if 0
#define SPC_RL_BUF                      50
#endif

#define SPC_RL_BUF                      256
#define SPC_PATH_FOLDER                 (256 + 16)
#define SPC_IDD_NAME                    32
#define SPC_MILLION                     1000000
#define SPC_RANGE_YEAR                  10000
#define SPC_RANGE_MONTH                 13
#define SPC_RANGE_DAY                   32
#define SPC_FNAME_LEN                   (SPC_IDD_NAME + 32)
#define SPC_TOPIC_SIZE                  32
#define SPC_MEMO_PADDING                1024
#define SPC_SHARED_KEY_LEN              32
#define SPC_SHARED_NAME_LEN             64
#define SPC_TEMPLATE_LEN                (SPC_PATH_FOLDER + SPC_FNAME_LEN + 32)
#define SPC_FULLPATH_LEN                (SPC_TEMPLATE_LEN + 32 + 16)

#ifndef UNIX_LINUX
#ifndef __SPC_STATIC_LOG__
#ifdef EXPORT_DLL_API_SPC_LOG
#define DLL_API_SPC_LOG                 __declspec(dllexport)
#else
#define DLL_API_SPC_LOG                 __declspec(dllimport)
#endif
#else
#define DLL_API_SPC_LOG                 
#endif
#else
#define DLL_API_SPC_LOG                 
#endif /*! UNIX_LINUX */

typedef enum __SPC_LOG_ERR_CODE__ {
	SPC_NO_ERROR,
	SPC_INIT_PATH_FOLDER_EMPTY_ERROR,
	SPC_LOG_LEVEL_ERROR,
	SPC_ERROR_CREATE_MUTEX,
	SPC_ERROR_CREATE_SEM,
	SPC_LOG_BUFF_SIZE_ERROR,
	SPC_LOG_MAX_SZ_MSG_ERROR,
	SPC_LOG_BUFF_MALLOC_ERROR,
	SPC_LOG_FOLDER_ERROR,
	SPC_LOG_CREATE_THREAD_ERROR,
	SPC_LOG_FMT_NULL_ERROR,
	SPC_LOG_MEM_GEN_FILE_ERROR,
	SPC_LOG_MEM_MALLOC_ERROR,
	SPC_LOG_OPEN_FILE_ERROR,
	SPC_LOG_OPEN1_FILE_ERROR,
	SPC_LOG_CLOSE_FILE_ERROR,
	SPC_LOG_SEM_NULL_ERROR,
	SPC_LOG_SEM_WIN32_CREATED_ERROR,
	SPC_LOG_MTX_WIN32_CREATED_ERROR,
	SPC_LOG_ROT_SIZE_ERROR,
	SPC_LOG_TOPIC_EMPTY,
	SPC_LOG_TOPIC_NULL,
	SPC_LOG_MEM_FILE_MALLOC_ERROR,
	SPC_LOG_CHECK_FOLDER_ERROR,
	SPC_LOG_CHECK_FOLDER_YEAR_ERROR,
	SPC_LOG_CHECK_FILE_YEAR_ERROR,
	SPC_LOG_CHECK_FOLDER_NULL_ERROR,
	SPC_LOG_MUTEX_NULL_ERROR,
	SPC_LOG_ST_NAME_NULL_ERROR,
	SPC_LOG_TIME_NULL_ERROR,
	SPC_LOG_TIME_NANO_NULL_ERROR,
	SPC_LOG_STAT_FOLDER_ERROR,
	SPC_LOG_PRINTF_ERROR,
	SPC_LOG_TOPIC_ZERO,
	SPC_LOG_TOPIC_MEMORY,
	SPC_LOG_TOPIC_FOPEN,
	SPC_LOG_TOPIC_FLUSH,
	SPC_LOG_TOPIC_BUFF_MEM,
	SPC_LOG_ALOCK_NUM,
	SPC_LOG_ALOCK_NULL,
	SPC_LOG_SHM_CREATE_NULL,
	SPC_LOG_SHM_WIN_UNMAP,
	SPC_LOG_SHM_UNIX_CREATE,
	SPC_LOG_SHM_UNIX_OPEN,
	SPC_LOG_SHM_UNIX_TRUNC,
	SPC_LOG_SHM_UNIX_MAP_FAILED,
	SPC_LOG_WIN_SHM_CLOSE,
	SPC_LOG_SHM_UNIX_UNMAP,
	SPC_LOG_VAR_NULL,
	SPC_LOG_ARR_MTX_NULL,
	SPC_LOG_ARR_BUFF_NULL,
	SPC_LOG_MTX_ATT_SHARED_MODE,
	SPC_LOG_MTX_ATT_SHARED_MODE_SET,
	SPC_LOG_MTX_INIT_ERR,
	SPC_LOG_SHM_UNIX_INIT_MUTEX,
	SPC_LOG_SPINLOCK_INIT_SHARED,
	SPC_LOG_SPINLOCK_INIT_PRIVATE,
	SPC_LOG_SEM_INIT_UNIX,
	SPC_LOG_THREAD_W32_CREATE,
	SPC_LOG_THREAD_PX_CREATE,
	SPC_LOG_MACH_GETTIME_ERROR,
	SPC_LOG_MACH_CLOCK_SERVICE_ERROR,
	SPC_LOG_OSX_SEM_CLOSE,
	SPC_LOG_OSX_SEM_UNLINK,
	SPC_LOG_SEM_OSX_CREATED_ERROR,
	SPC_LOG_SEM_INIT_OSX,
	SPC_LOG_SEM_OSX_UNLINK_ERROR,
	SPC_LOG_WIN32_MTX_LOCK,
	SPC_LOG_WIN32_MTX_UNLOCK,
	SPC_LOG_WIN32_SEM_WAIT,
	SPC_LOG_WIN32_SEM_REL,
	SPC_LOG_PX_SEM_WAIT, 
	SPC_LOG_PX_SEM_REL,
	SPC_LOG_PX_MTX_LOCK,
	SPC_LOG_PX_MTX_UNLOCK,
	SPC_LOG_PX_SPIN_LOCK,
	SPC_LOG_PX_SPIN_UNLOCK,


	SPC_END_ERROR,
} SPC_LOG_ERR_CODE;
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
typedef struct __SPC_CALLBACL_DATA__ {
	int total;
	int eventid;
	int range;
	int pc;
	int pl;
	char data[0];
} SPC_CALLBACL_DATA;
typedef int (*SPC_CALLBACL_FUNCTION)(void *);
typedef struct __SPC_GENERIC_DATA__ {
	int total; /*Total size*/
	/*int range;					Total size*/
	int pc; /*Point to the current*/
	int pl; /*Point to the last*/
	char data[0]; /*Generic data */
} spc_gen_data_st;

#define spc_uchar                       unsigned char
#define spc_uint                        unsigned int

typedef struct __spc_local_time_st__ {
	spc_uint year;
	spc_uchar month;
	spc_uchar day;
	spc_uchar hour;
	spc_uchar minute;
	spc_uchar sec;
	spc_uint nn; /*Nanosecond*/
} spc_local_time_st;

typedef struct __SPC_TOPIC_ST__ {
	int index; /*Index of a topic*/
	char topic[SPC_TOPIC_SIZE]; /*Name of topic*/
	spc_gen_data_st *buf; /*Buff for writing*/
	int fizize; /*Size of file.*/
	void *fp; /*File stream.*/
} SPC_TOPIC_ST;

typedef struct __SPC_LOG_ST__ {
	int llevel; /*Level of log.*/
	int file_limit_size; /*Limitation of each log file. No nead SYNC.*/
	int buff_size; /*Buffer size for each buffer. No nead SYNC.*/
	int range; /*The limitation of usage buffer.*/
	int krange; /*The limitation of usage buffer.*/
	int max_sz_msg; /*If the size of the message is less than the number, it
			   is safe to write. If not, it may be truncated.*/
	int index; /*Index of default log, not in a topic. No nead SYNC.*/
	char folder[SPC_PATH_FOLDER]; /*Path of genera folder. No nead SYNC.*/
	char off; /*Must be sync*/
	void *mtx_rw; /*mtx: Need to close handle*/
	void *sem_rwfile; /*sem_rwfile: Need to close handle*/
	void *sem_off; /*sem_off: Need to close handle*/
	spc_local_time_st lc_time_now; /*Current time.*/
	FILE *fp; /*fp: Need to close*/
	spc_gen_data_st *buf; /*buf: Must be synchoronized. Must be freed.*/
	char *topics; /*topics: topics string. Must be freed */
	int n_topic; /*Number of topics, SPC_TOPIC_ST.*/
	SPC_TOPIC_ST *arr_topic; /*List od topics: SPC_TOPIC_ST. Must be freed*/
	int renew; /*In a thread of logger, NO NEED SYNC.*/
	char path_template[SPC_TEMPLATE_LEN]; /*In a thread of logger, NO NEED
						 SYNC.*/
	int ncpu; /*Number of CPU.*/
	int trigger_thread; /*Use trigger thread or not.*/
	void **arr_mtx; /*List of lock: Spinlock or Mutex. Must be freed.*/
	char mode_straight; /* The order of records in same thread is arranged.*/
	/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
#ifndef UNIX_LINUX
	void *
#else
	int
#endif
	    hd; /* Handle of shared memory.*/
	char shared_key[SPC_SHARED_KEY_LEN]; /* Name of shared key.
		MUST be different between applications, special with MAC OSX,
		even process_mode=0.*/
	char id_name[SPC_IDD_NAME]; /*To avoid duplicating of file name.*/
	char isProcessMode; /*For cross processes mode.*/
	int map_mem_size; /*Total mapped memory.*/
	char is_master;
	SPC_CALLBACL_FUNCTION fn;
	SPC_CALLBACL_DATA *obj;

} SPC_LOG_ST;

typedef struct __SPC_INPUT_ARG__ {
	char folder[SPC_PATH_FOLDER];
	char id_name[SPC_IDD_NAME];
	char is_master;
	/*
	char mode_straight;
	*/
	SPC_CALLBACL_FUNCTION fn;
	SPC_CALLBACL_DATA *obj;
} SPC_INPUT_ARG;
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/

#define __FILLE__(__p__)                                                                                                    \
	do {                                                                                                                \
		__p__ = strrchr(__FILE__, '/');                                                                             \
		if (__p__) {                                                                                                \
			++__p__;                                                                                            \
			break;                                                                                              \
		}                                                                                                           \
		__p__ = strrchr(__FILE__, '\\');                                                                            \
		if (__p__) {                                                                                                \
			++__p__;                                                                                            \
			break;                                                                                              \
		}                                                                                                           \
		__p__ = __FILE__;                                                                                           \
	} while (0);

#ifndef __UNIX_LINUX_CPP11_AND_NEWERS__
#define spc_console_log(___fmttt___, ...)                                                                                   \
	{                                                                                                                   \
		char buf[1024];                                                                                             \
		const char *pfn = 0;                                                                                        \
		__FILLE__(pfn);                                                                                             \
		spc_fmmt_now(buf, 1024);                                                                                    \
		fprintf(stdout,                                                                                             \
		    "[%s] [%s:%s:%d] [thid: %llu] "___fmttt___                                                              \
		    "\n",                                                                                                   \
		    (char *)buf, (char *)pfn, (char *)__FUNCTION__, (int)__LINE__, spc_get_threadid(), ##__VA_ARGS__);      \
	}
#else
#define spc_console_log(___fmttt___, ...)                                                                                   \
	{                                                                                                                   \
		std::string __c11fmt__ = "[%s] [%s:%s:%d] [thid: %llu] ";                                                   \
		__c11fmt__ += ___fmttt___;                                                                                  \
		__c11fmt__ += "\n";                                                                                         \
		;                                                                                                           \
		char buf[1024];                                                                                             \
		const char *pfn = 0;                                                                                        \
		__FILLE__(pfn);                                                                                             \
		spc_fmmt_now(buf, 1024);                                                                                    \
		fprintf(stdout, __c11fmt__.c_str(), (char *)buf, (char *)pfn, (char *)__FUNCTION__, (int)__LINE__,          \
		    spc_get_threadid(), ##__VA_ARGS__);                                                                     \
	}
#endif

#define spc_malloc(__nn__, __obj__, __type__)                                                                               \
	{                                                                                                                   \
		(__obj__) = (__type__ *)malloc(__nn__);                                                                     \
		if (__obj__) { /*spc_console_log("Malloc: 0x%p\n",                                                          \
				  (__obj__));*/                                                                             \
			;                                                                                                   \
			memset((void *)(__obj__), 0, (__nn__));                                                             \
		} else {                                                                                                    \
			spc_console_log("Malloc: error.\n");                                                                \
		}                                                                                                           \
	}

#define spc_free(__obj__)                                                                                                   \
	{ /*spc_console_log("Free: 0x%p.\n", (__obj__));*/                                                                  \
		;                                                                                                           \
		free(__obj__);                                                                                              \
		;                                                                                                           \
		(__obj__) = 0;                                                                                              \
	}

/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
#define SPC_MIN_AB(a, b) ((a) < (b)) ? (a) : (b)
#define SPC_MAX_AB(a, b) ((a) > (b)) ? (a) : (b)
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/

#define SPC_KEYBUF(__t__, __i__) ((spc_gen_data_st *)((char *)__t__->buf + (__t__->buff_size * __i__)))
#define __spc_log_buf_level__(__lv__, ___fmttt___, ...)                                                                     \
	{                                                                                                                   \
		;                                                                                                           \
		;                                                                                                           \
		SPC_LOG_ST *__t__ = spc_control_obj();                                                                      \
		if (__t__->llevel <= (__lv__) && ___fmttt___[0]) {                                                          \
			;                                                                                                   \
			;                                                                                                   \
			int __outlen__ = 0;                                                                                 \
			;                                                                                                   \
			const char *__pfn__ = 0; /*char __isOof = 0;*/                                                      \
			;                                                                                                   \
			;                                                                                                   \
			unsigned short __r__ = 0;                                                                           \
			;                                                                                                   \
			char __tnow__[SPC_RL_BUF];                                                                          \
			char *__pprefmt__ = 0;                                                                              \
			;                                                                                                   \
			;                                                                                                   \
			;                                                                                                   \
			__FILLE__(__pfn__);                                                                                 \
			__pprefmt__ = spc_fmt_now_ext(                                                                      \
			    __tnow__, SPC_RL_BUF, __lv__, __pfn__, __FUNCTION__, __LINE__, &__r__, &__outlen__);            \
			;                                                                                                   \
			{                                                                                                   \
				do {                                                                                        \
					;                                                                                   \
					int __len__ = 0;                                                                    \
					;                                                                                   \
                                                                                                                            \
					;                                                                                   \
					;                                                                                   \
					;                                                                                   \
					spc_mutex_lock(__t__->arr_mtx[__r__]);                                              \
					;                                                                                   \
					if (__t__->range > SPC_KEYBUF(__t__, __r__)->pl) {                                  \
						;                                                                           \
						memcpy(SPC_KEYBUF(__t__, __r__)->data + SPC_KEYBUF(__t__, __r__)->pl,       \
						    __pprefmt__, __outlen__);                                               \
						;                                                                           \
						SPC_KEYBUF(__t__, __r__)->pl += __outlen__;                                 \
						;                                                                           \
						__len__ =                                                                   \
						    snprintf(SPC_KEYBUF(__t__, __r__)->data + SPC_KEYBUF(__t__, __r__)->pl, \
							__t__->krange - SPC_KEYBUF(__t__, __r__)->pl, ___fmttt___,          \
							##__VA_ARGS__);                                                     \
						;                                                                           \
						if (__len__ > 0) {                                                          \
							; /*spc_console_log("len:                                           \
							     %d", len);*/                                                   \
							;                                                                   \
							;                                                                   \
							__outlen__ = SPC_MIN_AB(                                            \
							    __len__, __t__->krange - SPC_KEYBUF(__t__, __r__)->pl);         \
							;                                                                   \
							; /*spc_console_log("outlen:                                        \
							     %d", outlen);*/                                                \
							;                                                                   \
							SPC_KEYBUF(__t__, __r__)->pl += __outlen__;                         \
							;                                                                   \
						};                                                                          \
					}                                                                                   \
                                                                                                                            \
					spc_mutex_unlock(__t__->arr_mtx[__r__]);                                            \
                                                                                                                            \
					if (__len__ > 0)                                                                    \
						break;                                                                      \
					;                                                                                   \
					__r__++;                                                                            \
					__r__ %= __t__->ncpu;                                                               \
					;                                                                                   \
					;                                                                                   \
					continue;                                                                           \
				} while (1);                                                                                \
				if (!__t__->trigger_thread)                                                                 \
					spc_rel_sem(__t__->sem_rwfile);                                                     \
				if (__pprefmt__ != __tnow__) {                                                              \
					spc_free(__pprefmt__);                                                              \
				}                                                                                           \
			}                                                                                                   \
		}                                                                                                           \
	}

#define SPC_ST_LOGBUFTOPIC(__t__, __i__) (&(__t__->arr_topic[__i__]))->buf
#define SPC_ST_LOGBUFTOPIC_RANGE(__t__, __i__, __r__)                                                                       \
	((spc_gen_data_st *)((char *)SPC_ST_LOGBUFTOPIC(__t__, __i__) + __t__->buff_size * __r__))

#define __spc_log_buf_topic_level__(__lv__, __tpic__, ___fmttt___, ...)                                                     \
	{                                                                                                                   \
		;                                                                                                           \
		SPC_LOG_ST *__t__ = spc_control_obj();                                                                      \
		;                                                                                                           \
		if (__t__->llevel <= (__lv__) && ___fmttt___[0] && __t__->arr_topic) {                                      \
			;                                                                                                   \
			short __tpp__ = 0;                                                                                  \
			int __len__ = 0;                                                                                    \
			unsigned short __r__ = 0;                                                                           \
			;                                                                                                   \
			const char *__pfn__ = 0;                                                                            \
			;                                                                                                   \
			;                                                                                                   \
			int __outlen__ = 0;                                                                                 \
			;                                                                                                   \
			char *__pprefmt__ = 0;                                                                              \
			;                                                                                                   \
			char __tnow__[SPC_RL_BUF];                                                                          \
			;                                                                                                   \
			__tpp__ = __tpic__ % __t__->n_topic;                                                                \
			;                                                                                                   \
			;                                                                                                   \
			;                                                                                                   \
			__FILLE__(__pfn__);                                                                                 \
			;                                                                                                   \
			;                                                                                                   \
			__pprefmt__ = spc_fmt_now_ext(                                                                      \
			    __tnow__, SPC_RL_BUF, __lv__, __pfn__, __FUNCTION__, __LINE__, &__r__, &__outlen__);            \
			;                                                                                                   \
			do {                                                                                                \
				;                                                                                           \
				;                                                                                           \
				spc_mutex_lock(__t__->arr_mtx[__r__]);                                                      \
				/*do                                                                                        \
				{*/                                                                                         \
				/*if(t->arr_topic){*/;                                                                      \
				;                                                                                           \
				;                                                                                           \
				;                                                                                           \
				if (__t__->range > SPC_ST_LOGBUFTOPIC_RANGE(__t__, __tpp__, __r__)->pl) {                   \
					;                                                                                   \
					memcpy(SPC_ST_LOGBUFTOPIC_RANGE(__t__, __tpp__, __r__)->data +                      \
						   SPC_ST_LOGBUFTOPIC_RANGE(__t__, __tpp__, __r__)->pl,                     \
					    __pprefmt__, __outlen__);                                                       \
					;                                                                                   \
					SPC_ST_LOGBUFTOPIC_RANGE(__t__, __tpp__, __r__)->pl += __outlen__;                  \
					;                                                                                   \
					;                                                                                   \
					__len__ = snprintf(SPC_ST_LOGBUFTOPIC_RANGE(__t__, __tpp__, __r__)->data +          \
							       SPC_ST_LOGBUFTOPIC_RANGE(__t__, __tpp__, __r__)->pl,         \
					    __t__->krange - SPC_ST_LOGBUFTOPIC_RANGE(__t__, __tpp__, __r__)->pl,            \
					    ___fmttt___, ##__VA_ARGS__);                                                    \
					;                                                                                   \
					;                                                                                   \
					if (__len__ > 0) {                                                                  \
						; /*spc_console_log("len: %d",                                              \
						     len);*/                                                                \
						;                                                                           \
						;                                                                           \
						__outlen__ = SPC_MIN_AB(__len__,                                            \
						    __t__->krange - SPC_ST_LOGBUFTOPIC_RANGE(__t__, __tpp__, __r__)->pl);   \
						; /*spc_console_log("outlen:                                                \
						     %d", outlen);*/                                                        \
						;                                                                           \
						;                                                                           \
						;                                                                           \
						SPC_ST_LOGBUFTOPIC_RANGE(__t__, __tpp__, __r__)->pl += __outlen__;          \
					}                                                                                   \
				}                                                                                           \
				/*}*/                                                                                       \
				/*}                                                                                         \
				while(0);*/                                                                                 \
				spc_mutex_unlock(__t__->arr_mtx[__r__]);                                                    \
				if (__len__ > 0)                                                                            \
					break;                                                                              \
				;                                                                                           \
				;                                                                                           \
				__r__++;                                                                                    \
				__r__ %= __t__->ncpu;                                                                       \
				;                                                                                           \
				continue;                                                                                   \
			} while (1);                                                                                        \
			if (!__t__->trigger_thread)                                                                         \
				spc_rel_sem(__t__->sem_rwfile);                                                             \
			;                                                                                                   \
			if (__pprefmt__ != __tnow__) {                                                                      \
				spc_free(__pprefmt__);                                                                      \
			}                                                                                                   \
		}                                                                                                           \
	}

/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/

/* Please demo with spc_init_log */
DLL_API_SPC_LOG int
spc_init_log(char *path);

/* Please demo with spc_init_log */
DLL_API_SPC_LOG int
spc_init_log_ext(SPC_INPUT_ARG *input);

/*
 * Export name:	spclog
 * Sample:		spclog(SPC_LOG_INFO, "Hello spclog: %llu", time(0));
 */
#define spclog                          __spc_log_buf_level__

/*
 * Export name:	spclogtopic
 * Sample:		spclogtopic(SPC_LOG_INFO, 0, "Hello spclog: %llu",
 * time(0));
 */
#define spclogtopic                     __spc_log_buf_topic_level__

/* Please demo with spc_finish_log */
DLL_API_SPC_LOG int
spc_finish_log();

/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/

DLL_API_SPC_LOG char *
spc_fmt_now_ext(char *fmtt, int len, int lv, const char *filename, const char *funcname, int line, unsigned short *r, int *);

DLL_API_SPC_LOG int
spc_fmmt_now(char *fmtt, int len);

DLL_API_SPC_LOG int
spc_mutex_lock(void *mtx);

DLL_API_SPC_LOG int
spc_mutex_unlock(void *mtx);

DLL_API_SPC_LOG SPC_LLU
spc_get_threadid();

DLL_API_SPC_LOG int
spc_rel_sem(void *sem);

DLL_API_SPC_LOG void
spc_sleep(unsigned int);

DLL_API_SPC_LOG void
spc_milli_sleep(unsigned int);

DLL_API_SPC_LOG SPC_LLU
spc_milli_now();

DLL_API_SPC_LOG
SPC_LOG_ST *
spc_control_obj();

DLL_API_SPC_LOG
SPC_LLU
spc_process_id();

DLL_API_SPC_LOG
SPC_LLU
spc_update_processid();

DLL_API_SPC_LOG const char *
spc_err_txt(int i);
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/

#ifdef __cplusplus
}
#endif
#endif
