
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
 *		<2024-Sep-12>
 *		<2024-Dec-18>
 *		<2024-Dec-20>
 *		<2024-Dec-22>
 *		<2024-Dec-23>
 *		<2024-Dec-30>
 *		<2025-Jan-03>
 *		<2025-Jan-06>
 *		<2025-Jan-18>
 *		<2025-Feb-04>
 *		<2025-Apr-12>
 *		<2025-Apr-20>
 *		<2025-Apr-22>
 *		<2025-May-29>
 *		<2025-Jun-01>
 *		<2025-Jun-04>
 * Decription:
 *		The (only one) main file to implement simplelog-challenge.
 */
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/

#include "simplelog-challenge.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#ifndef UNIX_LINUX
#include <Windows.h>
#define SPC_YEAR_PADDING          0
#define SPC_MONTH_PADDING         0
#else
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h> /* For mode constants */
#include <fcntl.h> /* For O_* constants */
#include <errno.h>
#ifdef __MACH__
#include <mach/mach.h>
#include <mach/clock.h>
#include <mach/clock_types.h>
#endif
#define SPC_YEAR_PADDING          1900
#define SPC_MONTH_PADDING         1

#define SPC_LOG_UNIX__SHARED_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
#define SPC_LOG_UNIX_CREATE_MODE  (O_CREAT | O_RDWR | O_EXCL)
#define SPC_LOG_UNIX_OPEN_MODE    (O_RDWR | O_EXCL)
/* #define SPC_LOG_UNIX_PROT_FLAGS	\
	(PROT_READ | PROT_WRITE | PROT_EXEC) */
#define SPC_LOG_UNIX_PROT_FLAGS   (PROT_READ | PROT_WRITE)
#endif

/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/

#define SPC_FCLOSE(__fp__, __n)                                                                                             \
	{                                                                                                                   \
		if (__fp__) {                                                                                               \
			;                                                                                                   \
			SPC_LLU __tp__ = (SPC_LLU)(__fp__);                                                                 \
			;                                                                                                   \
			(__n) = fclose((FILE *)(__fp__));                                                                   \
			if (__n) {                                                                                          \
				spc_err("fclose, err: %d, fp: 0x%p.", __n, (void *)__tp__);                                 \
				/*spc_fclose_err(__n, __fp__); */                                                           \
			} else { /*spc_console_log("Close FILE 0x%p DONE.",                                                 \
				    (__fp__));*/                                                                            \
				;                                                                                           \
				(__fp__) = 0;                                                                               \
				;                                                                                           \
			}                                                                                                   \
		}                                                                                                           \
	}

#define SPC_FFLUSH(__fp__, __n)                                                                                             \
	{                                                                                                                   \
		if (__fp__) {                                                                                               \
			(__n) = fflush((FILE *)(__fp__));                                                                   \
			if (__n) {                                                                                          \
				spc_err("fflush, err: %d, fp: 0x%p.", __n, __fp__);                                         \
			}                                                                                                   \
		}                                                                                                           \
	}

#define SPC_FFOPEN(__fp, __path, __mode)                                                                                    \
	{                                                                                                                   \
		(__fp) = fopen((__path), (__mode));                                                                         \
		if (!(__fp))                                                                                                \
			spc_console_log("Open FILE error code: 0x%p, %s.\n", (__fp), (__fp) ? "DONE" : "FAILED");           \
	}

#define SPC_FFTELL(__fp) ftell((FILE *)(__fp))
#define SPC_FFSEEK(__fp, __a, __b) fseek((FILE *)(__fp), (__a), (__b))

#ifndef UNIX_LINUX
#define SPC_CloseHandle(__obj)                                                                                              \
	{                                                                                                                   \
		int bl = CloseHandle((__obj));                                                                              \
		if (!bl)                                                                                                    \
			spc_console_log("CloseHandle %s", bl ? "DONE" : "ERROR");                                           \
		(__obj) = 0;                                                                                                \
	}
#define spc_all(__fmt__, ...) spc_console_log("[A] -- " __fmt__, ##__VA_ARGS__)
#define spc_err(__fmt__, ...) spc_console_log("[E] errcode: %d, " __fmt__, (int)GetLastError(), ##__VA_ARGS__)
#else

#define spc_all(__fmt__, ...) spc_console_log("[A] -- " __fmt__, ##__VA_ARGS__)
#define spc_err(__fmt__, ...) spc_console_log("[E] errno: %d, errtext: %s: " __fmt__, errno, strerror(errno), ##__VA_ARGS__)


#define SPC_sem_wait(__obj) sem_wait((sem_t *)(__obj))
#define SPC_sem_post(__obj) sem_post((sem_t *)(__obj))
#define SPC_sem_destroy(__obj, __err)                                                                                       \
	{                                                                                                                   \
		(__err) = sem_destroy((sem_t *)(__obj));                                                                    \
		if ((__err))                                                                                                \
			spc_console_log("sem_destroy errcode: %d. %s\n", (__err), (__err) ? "FALIED" : "DONE")              \
	}
#define SPC_pthread_mutex_destroy(__obj, __err)                                                                             \
	{                                                                                                                   \
		(__err) = pthread_mutex_destroy((pthread_mutex_t *)(__obj));                                                \
		if ((__err))                                                                                                \
			spc_console_log("pthread_mutex_destroy errcode: %d. %s\n", (__err), (__err) ? "FALIED" : "DONE");   \
	}
#define SPC_pthread_mutex_lock(__obj, __err)                                                                                \
	{                                                                                                                   \
		(__err) = pthread_mutex_lock((pthread_mutex_t *)(__obj));                                                   \
		if ((__err))                                                                                                \
			spc_console_log("pthread_mutex_lock errcode: %d. %s\n", (__err), (__err) ? "FALIED" : "DONE");      \
	}
#define SPC_pthread_mutex_unlock(__obj, __err)                                                                              \
	{                                                                                                                   \
		(__err) = pthread_mutex_unlock((pthread_mutex_t *)(__obj));                                                 \
		if ((__err))                                                                                                \
			spc_console_log("pthread_mutex_unlock errcode: %d. %s\n", (__err), (__err) ? "FALIED" : "DONE");    \
	}

#define spc_shm_unlink(__name__, __err__)                                                                                   \
	{                                                                                                                   \
		__err__ = shm_unlink(__name__);                                                                             \
		if (__err__) {                                                                                              \
			spc_console_log("shm_unlink: err: %d, errno: %d, "                                                  \
					"text: %s, name: %s.",                                                              \
			    __err__, errno, strerror(errno), __name__);                                                     \
		}                                                                                                           \
	}
#endif
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
#define SPC_SEM_NAME_RW           "_SEM_RW"
#define SPC_SEM_NAME_OFF          "_SEM_OFF"

#define SPC_MTX_NAME_RW           "_MTX_RW"
#define SPC_MTX_NAME_OFF          "_MTX_OFF"
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/

/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/

#define SPCLOG_PATHFOLDR          "pathfoder="
#define SPCLOG_LEVEL              "level="
#define SPCLOG_BUFF_SIZE          "buffsize="
#define SPC_MAX_SZ_MSG            "max_sz_msg="
#define SPCLOG_ROT_SIZE           "rotation_size="
#define SPCLOG_TOPIC              "topic="
#define SPCLOG_NCPU               "ncpu="
#define SPCLOG_TRIGGER            "trigger="
#define SPCLOG_PROCESS_MODE       "process_mode="
#define SPCLOG_SHARED_KEY         "shared_key="
#define SPCLOG_MODE_STRAIGHT      "mode_straight="
#define SPCLOG_END_CFG            "end_configuring="
#ifndef UNIX_LINUX
#define SPC_FILE_NAME_FMT         "%s\\%s\\%s_%.8d.log"
#define SPC_FILE_NAME_FMT_TOPIC   "%s\\%s\\%s"
#else
#define SPC_FILE_NAME_FMT         "%s/%s/%s_%.8d.log"
#define SPC_FILE_NAME_FMT_TOPIC   "%s/%s/%s"
#endif

#define SPC_FMT_DATE_ADDING       "%.4d-%.2d-%.2d"
#define SPC_FMT_HOUR_ADDING       "%.2d:%.2d:%.2d"
#define SPC_FMT_DELT_ADDING       "%s %s.%.9u"
#define SPC_FMT_MILL_ADDING       "%s %s.%.9d"
#define SPC_FMT_DATE_ADDING_X     "\n[%.4d-%.2d-%.2d %.2d:%.2d:%.2d.%.9d] "
#define HHHHHHHHHHH               "%llu]\t"

#define SPC_TEXT_UNKNOWN          "U"
#define SPC_TEXT_DEBUG            "D"
#define SPC_TEXT_INFO             "I"
#define SPC_TEXT_WARN             "W"
#define SPC_TEXT_ERROR            "E"
#define SPC_TEXT_FATAL            "F"

#define SPC_CASTGEN(__t__) ((spc_gen_data_st *)__t__)
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
#ifndef UNIX_LINUX
// DLL_API_SIMPLE_LOG
static void
spc_LockSpinlock(volatile long *p);
// DLL_API_SIMPLE_LOG
static void
spc_UnlockSpinlock(volatile long *p);
#define SPC_LockSpinlock(__p__) spc_LockSpinlock((volatile long *)(__p__))
#define SPC_UnlockSpinlock(__p__) spc_UnlockSpinlock((volatile long *)(__p__))

/*static
	volatile long spc_rw_spin = 0;*/
#define SPC_THREAD_ROUTINE        LPTHREAD_START_ROUTINE
#else
typedef void *(*SPC_THREAD_ROUTINE)(void *);
#endif
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/

typedef enum __SPC_CHANGE_NAME_E__ {
	SPC_NO_CHANGE_NAME = 0,
	SPC_CHANGE_FILE_SIZE,
	SPC_CHANGE_DAY,
	SPC_CHANGE_MONTH,
	SPC_CHANGE_YEAR,

	SPC_CHANGE_FINISH
} SPC_CHANGE_NAME_E;

/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
static const char*
__spclog_pathfolder[] = {
	SPCLOG_PATHFOLDR, 
	SPCLOG_LEVEL, 
	SPCLOG_BUFF_SIZE, 
	SPC_MAX_SZ_MSG,
    SPCLOG_ROT_SIZE, 
	SPCLOG_TOPIC, 
	SPCLOG_NCPU, 
	SPCLOG_TRIGGER, 
	SPCLOG_PROCESS_MODE, 
	SPCLOG_SHARED_KEY, 
	SPCLOG_MODE_STRAIGHT,
    SPCLOG_END_CFG, 
	0};

static SPC_LOG_ST __spc_log_statiic__;
;

static char __spc_process_id[32];
static int __spc_process_id_len;

/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/

static int
spc_init_log_parse(char *buff, char *key, char *);

static int
spc_verify_folder(char *folder);
static int
spc_simple_log_thread(SPC_LOG_ST *t);
static int
spc_gen_file(SPC_LOG_ST *t, int *n, int limit, int *);
static int
spc_gen_topics(SPC_LOG_ST *t);
static int
spc_get_fname_now(char *name);
static int
spc_get_fname_now(char *name);
static int
spc_folder_sup(char *folder, 
	spc_local_time_st *lctime, char *year_month);

static int
spc_stdz_topics(char *buff, int *inoutlen, int *, char **);

#ifndef UNIX_LINUX
static DWORD WINAPI
spc_written_thread_routine(LPVOID lpParam);
#else
static void *
spc_written_thread_routine(void *);
#endif

#if 0
static int
spc_fclose_err(int t, void *fpp);


static int
spc_fflush_err(int t, void *fpp);
#endif

#ifndef UNIX_LINUX

static int
spc_win32_sync_create();
static DWORD WINAPI
spc_trigger_routine(void *arg);
#else
#ifdef __MACH__
static int
spc_osx_sync_create();
static int
spc_osx_sync_del();
#else
#ifdef SPC_USING_SPIN_LOCK
static int
spc_spinlock_init(pthread_spinlock_t *mtx, char shared);
#endif
#endif

static int
spc_mtx_init(void *mtx, char shared);
static void *
spc_trigger_routine(void *arg);
#endif

#ifndef UNIX_LINUX
static int
spc_create_thread(SPC_THREAD_ROUTINE f, void *arg, HANDLE *outhd);
#else
static int
spc_create_thread(SPC_THREAD_ROUTINE f, void *arg, pthread_t *outid);
#endif

static int
spc_create_memory(void **output, char *shared_key, int size_shared, char isCreating);
static int
spc_del_memory();
static int
spc_calculate_size();
static int
spc_init_segments();
static int
spc_allocate_topics();
static int
spc_gen_sync_tool();
static int
spc_clean_sync_tool();
static int
spc_set_off(int);
static int
spc_standardize_path(char *fname);
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
SPC_LOG_ST *
spc_control_obj()
{
	return (SPC_LOG_ST *)&__spc_log_statiic__;
}
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/

static const char spc_text_gb_c[SPC_LOG_PEAK] = {
    'A',
    'D',
    'I',
    'W',
    'E',
    'F',
};

/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
int
spc_local_time_now(spc_local_time_st *stt)
{
	int ret = 0;
#ifndef UNIX_LINUX
	SYSTEMTIME lt;
#else
	struct tm *lt, rlt;
	struct timespec nanosec;
#ifdef __MACH__
	clock_serv_t cclock;
	mach_timespec_t mts;
	kern_return_t result;
#endif
#endif
	do {
		if (!stt) {
			ret = SPC_LOG_ST_NAME_NULL_ERROR;
			break;
		}
#ifndef UNIX_LINUX
		LARGE_INTEGER counter;
		GetLocalTime(&lt);
		QueryPerformanceCounter(&counter);
		stt->year = (unsigned int)lt.wYear;
		stt->month = (unsigned char)lt.wMonth;
		stt->day = (unsigned char)lt.wDay;

		stt->hour = (unsigned char)lt.wHour;
		stt->minute = (unsigned char)lt.wMinute;
		stt->sec = (unsigned char)lt.wSecond;
		stt->nn = 
			(unsigned int)lt.wMilliseconds * SPC_MILLION + 
			counter.QuadPart % SPC_MILLION;
#else
		/* https://linux.die.net/man/3/localtime*/
		/* https://linux.die.net/man/3/clock_gettime*/
		time_t t = time(0);
		lt = localtime_r(&t, &rlt);
		if (!lt) {
			ret = SPC_LOG_TIME_NULL_ERROR;
			break;
		}
		lt = (struct tm *)&rlt;
		/* No need freeing,
		//https://stackoverflow.com/questions/35031647/do-i-need-to-free-the-returned-pointer-from-localtime-function
	      */
#ifdef __MACH__
		result = host_get_clock_service(
			mach_host_self(), REALTIME_CLOCK, &cclock);
		if (result != KERN_SUCCESS) {
			ret = SPC_LOG_MACH_CLOCK_SERVICE_ERROR;
			spc_err("host_get_clock_service");
			break;
		}
		result = clock_get_time(cclock, &mts);
		if (result != KERN_SUCCESS) {
			ret = SPC_LOG_MACH_GETTIME_ERROR;
			spc_err("clock_get_time");
			break;
		}
		mach_port_deallocate(mach_task_self(), cclock);
		nanosec.tv_sec = mts.tv_sec;
		nanosec.tv_nsec = mts.tv_nsec;
#else
		ret = clock_gettime(CLOCK_REALTIME, &nanosec);
		if (ret) {
			spc_err("clock_gettime");
			ret = SPC_LOG_TIME_NANO_NULL_ERROR;
			break;
		}
#endif
		stt->year = lt->tm_year;
		stt->month = lt->tm_mon;
		stt->day = lt->tm_mday;

		stt->hour = lt->tm_hour;
		stt->minute = lt->tm_min;
		stt->sec = lt->tm_sec;
		stt->nn = (nanosec.tv_nsec);

#endif
	} while (0);
	return ret;
}

/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
int
spc_set_off(int isoff)
{
	int ret = 0;
	SPC_LOG_ST *t = &__spc_log_statiic__;
	int shouldWait = 0;

	spc_mutex_lock(t->mtx_rw);
	do {
		t->off = isoff;
	} while (0);
	spc_mutex_unlock(t->mtx_rw);

	shouldWait = (!t->isProcessMode) ? 1 : (!!t->is_master);
	spc_rel_sem(t->sem_rwfile);

	if (isoff && shouldWait) {
		int errCode = 0;

#ifndef UNIX_LINUX
		errCode = (int)WaitForSingleObject(
			t->sem_off, INFINITE);
		if (errCode == WAIT_FAILED) {
			spc_err("WaitForSingleObject");
			ret = SPC_LOG_WIN32_SEM_WAIT;
		}
#else
		errCode = SPC_sem_wait(t->sem_off);
		if (errCode) {
			spc_err("SPC_sem_wait");
			ret = SPC_LOG_PX_SEM_WAIT;
		}
#endif

#ifdef SPC_SHOW_CONSOLE
		spc_console_log("------- errCode: %d\n", (int)errCode);
#endif

	}
	return ret;
}

/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
int
spc_init_log_parse(char *buff, char *key, char *isEnd)
{
	int ret = SPC_NO_ERROR;
	SPC_LOG_ST *t = &__spc_log_statiic__;
	do {
		if (strcmp(key, SPCLOG_PATHFOLDR) == 0) {
			if (!buff[0]) {
				ret = SPC_INIT_PATH_FOLDER_EMPTY_ERROR;
				break;
			}
			snprintf(__spc_log_statiic__.folder, 
				SPC_PATH_FOLDER, "%s", buff);
			break;
		}
		if (strcmp(key, SPCLOG_LEVEL) == 0) {
			int n = 0;
			int count = 0;
			count = sscanf(buff, "%d", &n);
			if (n < 0 || count < 1) {
				ret = SPC_LOG_LEVEL_ERROR;
				break;
			}
			/* spc_set_log_levwel(n); */
			t->llevel = n;
			break;
		}
		if (strcmp(key, SPCLOG_BUFF_SIZE) == 0) {
			int n = 0;
			int sz = 0;
			sz = sscanf(buff, "%d", &n);
			if (n < 1 || sz < 1) {
				ret = SPC_LOG_BUFF_SIZE_ERROR;
				break;
			}
			__spc_log_statiic__.buff_size = n;
			break;
		}
		if (strcmp(key, SPC_MAX_SZ_MSG) == 0) {
			int n = 0;
			int sz = 0;
			sz = sscanf(buff, "%d", &n);
			if (n < 1 || sz < 1) {
				ret = SPC_LOG_MAX_SZ_MSG_ERROR;
				break;
			}
			if (n < SPC_MEMO_PADDING) {
				n = SPC_MEMO_PADDING;
			}
			__spc_log_statiic__.max_sz_msg = n;
			break;
		}
		if (strcmp(key, SPCLOG_ROT_SIZE) == 0) {
			int n = 0;
			int sz = 0;
			sz = sscanf(buff, "%d", &n);
			if (n < 1 || sz < 1) {
				ret = SPC_LOG_ROT_SIZE_ERROR;
				break;
			}
			__spc_log_statiic__.file_limit_size = n;
#ifdef SPC_SHOW_CONSOLE
			spc_console_log(
				"__spc_log_statiic__.file_limit_size: %d.\n", 
				__spc_log_statiic__.file_limit_size);
#endif
			break;
		}
		if (strcmp(key, SPCLOG_TOPIC) == 0) {
			int n = 0, count = 0;
			char *p = 0;
			n = (int)strlen(buff);
			if (n < 1) {
				break;
			}
			ret = spc_stdz_topics(buff, &n, &count, &p);
			if (ret) {
				break;
			}
			__spc_log_statiic__.n_topic = count;
			__spc_log_statiic__.topics = p;
			break;
		}
		if (strcmp(key, SPCLOG_NCPU) == 0) {
			int sz = 0;
			int n = 0;
			sz = sscanf(buff, "%d", &n);
			if (sz < 1) {
				__spc_log_statiic__.ncpu = 1;
				break;
			}
			__spc_log_statiic__.ncpu = n;
			/*
			//if (__spc_log_statiic__.ncpu < 1) {
			//	__spc_log_statiic__.ncpu = 1;
			//}
			*/
			break;
		}
		if (strcmp(key, SPCLOG_TRIGGER) == 0) {
			int n = 0, sz = 0;
			sz = sscanf(buff, "%d", &n);
			if (sz < 1) {
				__spc_log_statiic__.trigger_thread = 0;
				break;
			}
			__spc_log_statiic__.trigger_thread = n;
			break;
		}
		if (strcmp(key, SPCLOG_PROCESS_MODE) == 0) {
			int n = 0, sz = 0;
			sz = sscanf(buff, "%d", &n);
			if (sz < 1) {
				__spc_log_statiic__.isProcessMode = 0;
				spc_console_log("SPCLOG_PROCESS_MODE error.");
				break;
			}
			__spc_log_statiic__.isProcessMode = n ? 1 : 0;
			break;
		}
		if (strcmp(key, SPCLOG_SHARED_KEY) == 0) {
			int n = 0;
			/*
			 char* p = 0;
			*/
			n = (int)strlen(buff);
			if (n < 1) {
				break;
			}
			snprintf(__spc_log_statiic__.shared_key, 
				SPC_SHARED_KEY_LEN, "%s", buff);
			break;
		}
		if (strcmp(key, SPCLOG_MODE_STRAIGHT) == 0) {
			int n = 0, sz = 0;
			sz = sscanf(buff, "%d", &n);
			t->mode_straight = n ? 1 : 0;
			spc_console_log("buff %s, t->mode_straight: %d, sz: %d.", 
				buff, t->mode_straight, sz);
			break;
		}
		if (strcmp(key, SPCLOG_END_CFG) == 0) {
#ifdef SPC_SHOW_CONSOLE
			spc_console_log("End configuration.\n");
#endif
			if (isEnd) {
				*isEnd = 1;
			}
			break;
		}
	} while (0);
	return ret;
}

/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/

int
spc_init_log_ext(SPC_INPUT_ARG *input)
{
	int ret = 0;
	do {
		memcpy(__spc_log_statiic__.id_name, 
			input->id_name, SPC_IDD_NAME);
		__spc_log_statiic__.is_master = input->is_master;
		/*
		__spc_log_statiic__.mode_straight = input->mode_straight;
		*/
		ret = spc_init_log(input->folder);
		;
		if (ret) {
			spc_console_log("Cannot initiate log.");
			break;
		}
	} while (0);
	return ret;
}
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
SPC_LLU
spc_update_processid()
{
	snprintf(__spc_process_id, 32, "[pid \t %llu]\t", spc_process_id());
	__spc_process_id_len = (int)strlen(__spc_process_id);
	return 0;
}
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/

int
spc_init_log(char *pathcfg)
{
	int ret = 0;
	FILE *fp = 0;
	char c = 0;
	int count = 0;
	char buf[1024];
	/*
	    void* obj = 0;
	*/
	char isEnd = 0;
	__spc_log_statiic__.ncpu = 1;
	do {
		snprintf(__spc_process_id, 
			32, "[pid \t %llu]\t", spc_process_id());

		__spc_process_id_len = (int)strlen(__spc_process_id);

		memset(buf, 0, sizeof(buf));
		/* // fp = fopen(pathcfg, "r");*/
		SPC_FFOPEN(fp, pathcfg, "r");
		if (!fp) {
			ret = 1;
			spc_console_log("Cannot open file error.");
			break;
		}
		while (1) {
			c = fgetc(fp);
			if (c == '\r' || c == '\n' || c == EOF) {
				int j = 0;
				char *node = 0;
				if (count < 1) {
					continue;
				}
				while (1) {
					char *pp = 0;
					node = (char *)__spclog_pathfolder[j];
					if (!node) {
						break;
					}
					pp = strstr(buf, node);
					if (!pp) {
						++j;
						continue;
					}
					if (pp != buf) {
						++j;
						continue;
					}
					/* // if (pp) */
					{
						char *p = 0;
						size_t k = 0;
						k = strlen(node);
						p = (buf + k);
#ifdef SPC_SHOW_CONSOLE
						spc_console_log("Find out the keyword"
								": [%s] value [%s].",
						    node, p);
#endif
						ret = spc_init_log_parse(p, node, &isEnd);
						break;
					}
					/*
					j++;
					*/
				}

				if (ret) {
					break;
				}
				count = 0;
				memset(buf, 0, sizeof(buf));

				if (isEnd) {
					break;
				}
				if (c == EOF) {
					break;
				}
				continue;
			}
			if (isEnd) {
				break;
			}
			if (c == EOF) {
				break;
			}
			buf[count++] = c;
		}
		if (ret) {
			break;
		}

		ret = spc_verify_folder(__spc_log_statiic__.folder);
		if (ret) {
			break;
		}
		ret = spc_gen_sync_tool();
		if (ret) {
			break;
		}
		if (__spc_log_statiic__.isProcessMode) {
			if (__spc_log_statiic__.is_master) {
				ret = spc_simple_log_thread(&__spc_log_statiic__);
				if (ret) {
					break;
				}
			}
		} else {
			ret = spc_simple_log_thread(&__spc_log_statiic__);
			if (ret) {
				break;
			}
		}
	} while (0);

	if (fp) {
		SPC_FCLOSE(fp, ret);
	}

	return ret;
}

/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
int
spc_mutex_lock(void *obj)
{
	int ret = 0;
#ifndef UNIX_LINUX
	DWORD err = 0;
#else
	int err = 0;
#endif
	do {
		if (!obj) {
			ret = SPC_LOG_MUTEX_NULL_ERROR;
			break;
		}
#ifndef UNIX_LINUX
#ifndef SPC_USING_SPIN_LOCK
		err = WaitForSingleObject(obj, INFINITE);
		if (err != WAIT_OBJECT_0) {
			spc_err("WaitForSingleObject");
			ret = SPC_LOG_WIN32_MTX_LOCK;
			break;
		}
#else
		SPC_LockSpinlock(obj);
#endif
#else
#ifndef SPC_USING_SPIN_LOCK
		SPC_pthread_mutex_lock((pthread_mutex_t *)obj, err);
		if (err) {
			ret = SPC_LOG_PX_MTX_LOCK;
			spc_err("SPC_pthread_mutex_lock");
		}
#else
		err = pthread_spin_lock((pthread_spinlock_t *)obj);
		if (err) {
			ret = SPC_LOG_PX_SPIN_LOCK;
			spc_err("pthread_spin_lock");
		}
#endif

#endif
	} while (0);
	return ret;
}
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
int
spc_mutex_unlock(void *obj)
{
	int ret = 0;
#ifndef UNIX_LINUX
	DWORD done = 0;
#else
	int err = 0;
#endif
	do {
		if (!obj) {
			ret = SPC_LOG_MUTEX_NULL_ERROR;
			break;
		}
#ifndef UNIX_LINUX
#ifndef SPC_USING_SPIN_LOCK
		done = ReleaseMutex(obj);
		if (!done) {
			ret = SPC_LOG_WIN32_MTX_UNLOCK;
			break;
		}
#else
		SPC_UnlockSpinlock(obj);
#endif
#else
#ifndef SPC_USING_SPIN_LOCK
		SPC_pthread_mutex_unlock((pthread_mutex_t *)obj, err);
		if (err) {
			spc_err("SPC_pthread_mutex_unlock");
			ret = SPC_LOG_PX_MTX_UNLOCK;
		}
#else
		err = pthread_spin_unlock((pthread_spinlock_t *)obj);
		if (err) {
			spc_err("pthread_spin_unlock");
			ret = SPC_LOG_PX_SPIN_UNLOCK;
		}
#endif
#endif
	} while (0);
	return ret;
}
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
int
spc_verify_folder(char *folder)
{
	int ret = 0;
	do {
#ifndef UNIX_LINUX
		/*
		https://learn.microsoft.com/en-us/windows/win32/fileio/retrieving-and-changing-file-attributes
		//
		https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-createdirectorya
		// ERROR_ALREADY_EXISTS, ERROR_PATH_NOT_FOUND
		*/
		BOOL result = CreateDirectoryA(folder, NULL);
		if (!result) {
			DWORD werr = GetLastError();
			if (werr == ERROR_ALREADY_EXISTS) {
				/*
				ret = 1;
				*/
				break;
			}
			spc_err("CreateDirectoryA");
			ret = SPC_LOG_FOLDER_ERROR;
		}
#else
		spc_console_log("folder: %s.", folder);
#endif
	} while (0);
	return ret;
}
/*https://learn.microsoft.com/en-us/windows/win32/api/sysinfoapi/nf-sysinfoapi-getsystemtime*/
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
int
spc_get_fname_now(char *name)
{
	int ret = 0;
	spc_local_time_st lt;
	spc_local_time_now(&lt);
	if (name) {
		if (__spc_log_statiic__.id_name[0]) {
			snprintf(name, SPC_FNAME_LEN, 
				"%.4d-%.2d-%.2d-%s", 
				lt.year + SPC_YEAR_PADDING,
			    (int)lt.month + SPC_MONTH_PADDING, 
				(int)lt.day, 
				(char *)__spc_log_statiic__.id_name);
		} else {
			snprintf(name, SPC_FNAME_LEN, 
				"%.4d-%.2d-%.2d-simplelog", 
				lt.year + SPC_YEAR_PADDING,
			    (int)lt.month + SPC_MONTH_PADDING, 
				(int)lt.day);
		}
	}
	return ret;
}

/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
#ifndef UNIX_LINUX
DWORD WINAPI
spc_written_thread_routine(LPVOID lpParam)
#else
void *
spc_written_thread_routine(void *lpParam)
#endif
{
	int k = 0;
	SPC_LOG_ST *t = (SPC_LOG_ST *)lpParam;
	int ret = 0, sz = 0, err = 0;
	int werr = 0;

	register char is_off = 0;
	register int i = 0, j = 0;

	char **main_src_thrd_buf = 0;
	char ***src_topic_thrd_buf = 0;

#ifndef UNIX_LINUX
	HANDLE trigger_handle_id = 0;
#else
	pthread_t trigger_handle_id = 0;
#endif
	char *only_buf = 0;
	spc_gen_data_st *only_cast = 0;
	spc_malloc((t->buff_size * t->ncpu), only_buf, char);
	/*
	//spc_malloc((t->buff_size * 3), only_buf, char);
	//spc_create_memory((void**)&only_buf,
		"thread_buff_123", (t->buff_size * t->ncpu), 1);
	*/
	only_cast = SPC_CASTGEN(only_buf);
	only_cast->total = (t->buff_size * t->ncpu);
	/*
	//only_cast->range = only_cast->total -
		sizeof(spc_gen_data_st);
	*/
	only_cast->pl = only_cast->pc = 0;

	spc_malloc(t->ncpu * sizeof(char *), main_src_thrd_buf, char *);
	for (i = 0; i < t->ncpu; ++i) {
		char *p = (char *)t->buf;
		main_src_thrd_buf[i] = p + t->buff_size * i;
	}

	/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/

	if (t->arr_topic) {
		spc_malloc(t->n_topic * sizeof(char *), src_topic_thrd_buf, char **);
		for (i = 0; i < t->n_topic; ++i) {
			char *p = (char *)t->arr_topic[i].buf;
			spc_malloc(t->ncpu * sizeof(char *), src_topic_thrd_buf[i], char *);
			for (j = 0; j < t->ncpu; ++j) {
				src_topic_thrd_buf[i][j] = p + t->buff_size * j;
			}
		}
	}

	/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
	if (t->trigger_thread > 0) {
		spc_create_thread(spc_trigger_routine, t, &trigger_handle_id);
	}
	do {
		if (is_off) {
			break;
		}
		if (!t) {
			exit(1);
		}
		if (!t->sem_rwfile) {
			exit(1);
		}
		/*
		//spc_console_log("Semaphore: 0x%p.\n", t->sem_rwfile);
		*/
		if (!t->mtx_rw) {
			exit(1);
		}
		while (1) {
			if (is_off) {
				break;
			}
#ifndef UNIX_LINUX
			werr = WaitForSingleObject(t->sem_rwfile, INFINITE);
			if (WAIT_OBJECT_0 != werr) {
				spc_err("WaitForSingleObject");
			}
#else
			werr = SPC_sem_wait(t->sem_rwfile);
			if (werr) {
				spc_err("SPC_sem_wait");
			}
#endif
			do {
				ret = spc_gen_file(t, &sz, t->file_limit_size, &(t->index));
				if (ret) {
					spc_console_log("--spc_gen_file, ret: %d --\n", ret);
					continue;
				}
				ret = spc_gen_topics(t);
				if (ret) {
					spc_console_log("--spc_gen_topics, ret: %d --\n", ret);
					continue;
				}

				/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
				if (!is_off) {
					spc_mutex_lock(t->mtx_rw);
					is_off = t->off;
					spc_mutex_unlock(t->mtx_rw);
				}
				/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
				for (i = 0; i < t->ncpu; ++i) {
					spc_mutex_lock(t->arr_mtx[i]);
					/*
					//do { */
					if (SPC_CASTGEN(main_src_thrd_buf[i])->pl > 0) 
					{
						memcpy(only_cast->data + only_cast->pl,
						    SPC_CASTGEN(main_src_thrd_buf[i])->data,
						    SPC_CASTGEN(main_src_thrd_buf[i])->pl);

						only_cast->pl += SPC_CASTGEN(
							main_src_thrd_buf[i])->pl;

						SPC_CASTGEN(main_src_thrd_buf[i])->pl = 0;
					}
					/* //} while (0); */
					spc_mutex_unlock(t->arr_mtx[i]);
				}

				if (only_cast->pl > 0) {
					k = (int)fwrite(only_cast->data, 
						1, only_cast->pl, t->fp);
					only_cast->pl = 0;
					sz += k;
					SPC_FFLUSH((t->fp), err);
				}
				/*only_cast->pl*/
				if (err) {
					ret = SPC_LOG_TOPIC_FLUSH;
					spc_err("fflush");
					break;
				}
				/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
				if (t->n_topic > 0) {
					char *src = 0;
					for (i = 0; i < t->n_topic; ++i) {
						for (j = 0; j < t->ncpu; ++j) {
							src = src_topic_thrd_buf[i][j];
							spc_mutex_lock(t->arr_mtx[j]);
							/*//do */
							if (SPC_CASTGEN(src)->pl > 0) {
								memcpy(
									only_cast->data + only_cast->pl,
								    SPC_CASTGEN(src)->data, 
									SPC_CASTGEN(src)->pl);

								only_cast->pl += SPC_CASTGEN(src)->pl;
								SPC_CASTGEN(src)->pl = 0;
							}
							/*//} while (0);*/
							spc_mutex_unlock(t->arr_mtx[j]);
						}

						if (only_cast->pl) {
							k = (int)fwrite( only_cast->data, 
								1, 
								only_cast->pl, 
								(FILE *)(t->arr_topic[i].fp));

							t->arr_topic[i].fizize += k;
							only_cast->pl = 0;
							SPC_FFLUSH((t->arr_topic[i].fp), err);
							if (err) {
								ret = SPC_LOG_TOPIC_FLUSH;
								break;
							}
						}
						/*only_cast->pl*/
					}
				}
				/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
				if (is_off) {
					break;
				}
			} while (0);
		}
		if (t->fp) {
			int werr = 0;
			SPC_FCLOSE(t->fp, werr);
			for (i = 0; i < t->n_topic; ++i) {
				if (t->arr_topic[i].fp) {
					SPC_FCLOSE(t->arr_topic[i].fp, werr);
				}
			}
			spc_mutex_lock(t->mtx_rw);
			/*
			if (t->buf) {
				spc_free(t->buf);
			}
			*/
			for (i = 0; i < t->n_topic; ++i) {
				if (t->arr_topic[i].buf) {
					t->arr_topic[i].buf = 0;
				}
			}
			spc_mutex_unlock(t->mtx_rw);
		}

	} while (0);
	/* //spc_free(thrd_buffer); */
	spc_free(main_src_thrd_buf);
	if (t->arr_topic) {
		for (i = 0; i < t->n_topic; ++i) {
			spc_free(src_topic_thrd_buf[i]);
		}
		spc_free(src_topic_thrd_buf);
	}
	spc_free(only_buf);
	/* //spc_del_memory((void *) only_buf); */
	/* Send a signal to the waiting thread. */
	if (t->trigger_thread) {
#ifndef UNIX_LINUX
		if (trigger_handle_id) {
			int rs = (int)TerminateThread(trigger_handle_id, 0);
			if (!rs) {
				spc_console_log("TerminateThread error.");
			} else {
				spc_console_log("TerminateThread OK.");
			}
		}
#else
		if (trigger_handle_id) {
			int rs = pthread_cancel(trigger_handle_id);
			if (rs) {
				spc_console_log("pthread_cancel error.");
			} else {
				spc_console_log("pthread_cancel OK.");
			}
		}
#endif
	}
	spc_rel_sem(__spc_log_statiic__.sem_rwfile);
	if (!t->isProcessMode) {
		spc_rel_sem(__spc_log_statiic__.sem_off);
	} else if (t->is_master) {
		spc_rel_sem(__spc_log_statiic__.sem_off);
	}
	return 0;
}
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
int
spc_simple_log_thread(SPC_LOG_ST *t)
{
	int ret = 0;
	do {
#ifndef UNIX_LINUX
		HANDLE hd = 0;
		DWORD thread_id = 0;
		hd = CreateThread(NULL, 0, 
			spc_written_thread_routine, t, 0, &thread_id);
		if (!hd) {
			ret = SPC_LOG_CREATE_THREAD_ERROR;
			break;
		}
#else
		pthread_t idd = 0;
		int err = 0;
		tzset();
		err = pthread_create(&idd, 0, 
			spc_written_thread_routine, t);
		if (err) {
			ret = SPC_LOG_CREATE_THREAD_ERROR;
			break;
		}
#endif
	} while (0);
	return ret;
}
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
char *
spc_fmt_now_ext(
    char *fmtt, int len, int lv, 
	const char *filename, const char *funcname, 
	int line, unsigned short *r, int *outlen)
{
	/*
	SPC_LOG_ST *t = &__spc_log_statiic__;
	*/
	char *p = fmtt;
	int ret = 0;
	spc_local_time_st stt;
	int n = 0;
	SPC_LLU threadiid = 0;

	threadiid = (SPC_LLU)spc_get_threadid();
	*outlen = 0;
	ret = spc_local_time_now(&stt);
	if (ret) {
		return p;
	}
#if 1
	*r = __spc_log_statiic__.mode_straight ? 
		(threadiid % __spc_log_statiic__.ncpu) : 
			(stt.nn % __spc_log_statiic__.ncpu);
#endif

#if 0
#ifndef __MODE_STRAIGHT__
	*r = (stt.nn % __spc_log_statiic__.ncpu);
#else
	*r = (threadiid % __spc_log_statiic__.ncpu);
#endif
#endif
	n = sprintf(fmtt, 
		SPC_FMT_DATE_ADDING_X "[%c] [tid\t%llu]\t", 
		stt.year + SPC_YEAR_PADDING,
	    stt.month + SPC_MONTH_PADDING, stt.day, 
		stt.hour, stt.minute, stt.sec, (int)stt.nn,
	    spc_text_gb_c[lv % SPC_LOG_PEAK], threadiid);

	if (n < 1) {
		ret = SPC_LOG_PRINTF_ERROR;
		return p;
	}

	memcpy(fmtt + n, __spc_process_id, __spc_process_id_len);
	n += __spc_process_id_len;

	*outlen = n;

	/*
	 *outlen += snprintf(fmtt + n, len - n,
	 "[%s:%s:%d] [r: %d]\t", filename, funcname, line, (int)*r);
	 */
	*outlen += snprintf(fmtt + n, SPC_RL_BUF - n, 
		"[%s:%s:%d] ", filename, funcname, line);
	if (*outlen > len) {
		spc_malloc((*outlen + 1), p, char);
		if (!p) {
			exit(1);
		}
		memcpy(p, fmtt, n);
		*outlen = n;
		/*
		 *outlen += sprintf(p + n,
		 "[%s:%s:%d] [r: %d]\t", filename,
		 funcname, line, (int)*r);
		 */
		*outlen += snprintf(fmtt + n, SPC_RL_BUF - n, 
			"[%s:%s:%d] ", filename, funcname, line);
	}

	return p;
}

/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
int
spc_fmmt_now(char *fmtt, int len)
{
	int ret = 0;
	spc_local_time_st stt;
	char buff[20], buff1[20];
	int n = 0;
	do {
		if (!fmtt) {
			ret = (int)SPC_LOG_FMT_NULL_ERROR;
			break;
		}
		memset(&stt, 0, sizeof(stt));
		ret = spc_local_time_now(&stt);
		if (ret) {
			break;
		}

		memset(buff, 0, 20);
		memset(buff1, 0, 20);

		n = snprintf(
		    buff, 20, SPC_FMT_DATE_ADDING, 
			stt.year + SPC_YEAR_PADDING, 
			stt.month + SPC_MONTH_PADDING, stt.day);
		if (n < 1) {
			ret = SPC_LOG_PRINTF_ERROR;
			break;
		}
		n = snprintf(buff1, 20, 
			SPC_FMT_HOUR_ADDING, stt.hour, stt.minute, stt.sec);
		n = snprintf(fmtt, len, 
			SPC_FMT_MILL_ADDING, buff, buff1, (int)stt.nn);

	} while (0);
	return ret;
}

/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/

int
spc_gen_file(SPC_LOG_ST *t, int *sz, int limit, int *index)
{
	int ret = 0;
	spc_local_time_st lt, *plt = 0;
	;
	/*
	//int renew = SPC_NO_CHANGE_NAME;
	*/
	char path[SPC_FULLPATH_LEN + 1];
	char fmt_file_name[SPC_FNAME_LEN];
	int ferr = 0;
	char yearmonth[16];

	do {
		t->renew = SPC_NO_CHANGE_NAME;
		ret = spc_local_time_now(&lt);
		if (ret) {
			spc_console_log("spc_local_time_now: ret: %d.\n", ret);
			break;
		}

		memcpy(&(t->lc_time_now), &lt, sizeof(spc_local_time_st));
		plt = &(t->lc_time_now);
		if (!t->fp) {
			memset(path, 0, sizeof(path));
			memset(fmt_file_name, 0, sizeof(fmt_file_name));
			spc_get_fname_now(fmt_file_name);
			ret = spc_folder_sup(
				t->folder, &(t->lc_time_now), yearmonth);
			if (ret) {
				spc_console_log("spc_folder_sup: ret: %d.\n", ret);
				break;
			}
			do {
				int err = 0;
				int cszize = 0;
				snprintf(
				    path, SPC_FULLPATH_LEN, 
					SPC_FILE_NAME_FMT, t->folder, 
					yearmonth, fmt_file_name, *index);

				snprintf(t->path_template, 
					SPC_TEMPLATE_LEN, 
					SPC_FILE_NAME_FMT_TOPIC, t->folder, yearmonth,
				    fmt_file_name);

				spc_standardize_path(path);
				spc_standardize_path(t->path_template);

				SPC_FFOPEN(t->fp, path, "a+");
				if (!t->fp) {
					ret = SPC_LOG_OPEN_FILE_ERROR;
					break;
				}
				SPC_FFSEEK(t->fp, 0, SEEK_END);
				cszize = (int)SPC_FFTELL(t->fp);
				if (cszize < limit) {
					*sz = cszize;
					break;
				}
				SPC_FCLOSE(t->fp, err);
				if (err) {
					ret = SPC_LOG_CLOSE_FILE_ERROR;
				}
				(*index)++;

			} while (1);
			if (ret) {
				break;
			}
			break;
		}
		if (ret) {
			break;
		}
		do {
			if (*sz > limit) {
				(*index)++;
				t->renew = SPC_CHANGE_FILE_SIZE;
				break;
			}
			if (lt.year > plt->year) {
				(*index) = 0;
				t->renew = SPC_CHANGE_YEAR;
				break;
			}
			if (lt.month > plt->month) {
				t->renew = SPC_CHANGE_MONTH;
				(*index) = 0;
				break;
			}
			if (lt.day > plt->day) {
				(*index) = 0;
				t->renew = SPC_CHANGE_DAY;
				break;
			}
			t->renew = SPC_NO_CHANGE_NAME;
		} while (0);
		if (!t->renew) {
			break;
		}
		memcpy(&(t->lc_time_now), &lt, sizeof(spc_local_time_st));
		spc_get_fname_now(fmt_file_name);
		ret = spc_folder_sup(t->folder, &(t->lc_time_now), yearmonth);
		if (ret) {
			spc_console_log("spc_folder_sup: ret: %d.\n", ret);
			break;
		}
		snprintf(path, SPC_FULLPATH_LEN, 
			SPC_FILE_NAME_FMT, t->folder, 
			yearmonth, fmt_file_name, *index);

		snprintf(t->path_template, 
			SPC_TEMPLATE_LEN, 
			SPC_FILE_NAME_FMT_TOPIC, 
			t->folder, yearmonth, fmt_file_name);

		SPC_FCLOSE(t->fp, ferr);
		if (ferr) {
			ret = SPC_LOG_CLOSE_FILE_ERROR;
			break;
		}
		spc_standardize_path(path);
		spc_standardize_path(t->path_template);
		/*
		//t->fp = fopen(path, "a+");
		*/
		SPC_FFOPEN(t->fp, path, "a+");
		if (sz) {
			*sz = 0;
		}
		if (!t->fp) {
			ret = SPC_LOG_OPEN1_FILE_ERROR;
			break;
		}
	} while (0);
	return ret;
}

/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
SPC_LLU
spc_get_threadid()
{
#ifndef UNIX_LINUX
	return (SPC_LLU)GetCurrentThreadId();
#else
	return (SPC_LLU)pthread_self();
#endif
}
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
int
spc_rel_sem(void *sem)
{
	int ret = 0;
#ifndef UNIX_LINUX
	BOOL rs = 0;
#else
	/* int err = 0, val = 0; */
	int err = 0;
#endif
	do {
		if (!sem) {
			ret = SPC_LOG_SEM_NULL_ERROR;
			break;
		}
#ifndef UNIX_LINUX
		
		rs = ReleaseSemaphore(sem, 1, 0);
		if(!rs) {
			DWORD dwerr = GetLastError();
			if (dwerr == ERROR_TOO_MANY_POSTS) {
				break;
			}
			spc_err("ReleaseSemaphore");
			ret = SPC_LOG_WIN32_SEM_REL;
		}
#else
		/*
		err = sem_getvalue((sem_t*)sem, &val);
		if (!err) {
			if (val < 1) {
				SPC_sem_post(sem);
			}
		}
		*/
		err = SPC_sem_post(sem);
		if(err) {
			ret = SPC_LOG_PX_SEM_REL;
			spc_err("SPC_sem_post");
		}
#endif
	} while (0);
	return ret;
}

/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
int
spc_finish_log()
{
	int ret = 0;
	SPC_LOG_ST *t = &__spc_log_statiic__;
	ret = spc_set_off(1);
	if (ret) {
		spc_console_log("spc_set_off ret: %d", ret);
	}
	spc_all("Prepare spc_clean_sync_tool!");
	ret = spc_clean_sync_tool();
	if (ret) {
		spc_console_log("spc_clean_sync_tool ret: %d", ret);
	}
	memset(t, 0, sizeof(SPC_LOG_ST));
	return ret;
}

/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
/* https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-createdirectorya
 */
int
spc_folder_sup(char *folder, spc_local_time_st *lctime, char *year_month)
{
	int ret = 0;
	char path[SPC_PATH_FOLDER];
	int tyear = 0, tmonth = 0;
	;
#ifndef UNIX_LINUX
	int result = 0;
#else
	int err = 0;
	struct stat buf;
#endif
	memset(path, 0, sizeof(path));
	do {
		if (!folder) {
			ret = SPC_LOG_CHECK_FOLDER_NULL_ERROR;
			break;
		}
		if (!lctime) {
			ret = SPC_LOG_CHECK_FOLDER_NULL_ERROR;
			break;
		}
		if (!year_month) {
			ret = SPC_LOG_CHECK_FOLDER_NULL_ERROR;
			break;
		}

		tyear = lctime->year + SPC_YEAR_PADDING;
		tyear %= SPC_RANGE_YEAR;

		tmonth = lctime->month + SPC_MONTH_PADDING;
		tmonth %= SPC_RANGE_MONTH;

		snprintf(path, SPC_PATH_FOLDER, "%s", folder);

#ifndef UNIX_LINUX
		result = CreateDirectoryA(path, 0);
		if (!result) {
			DWORD xerr = GetLastError();
			if (xerr != ERROR_ALREADY_EXISTS) {
				ret = SPC_LOG_CHECK_FOLDER_ERROR;
				spc_err("CreateDirectoryA");
				break;
			}
		}

		snprintf(path, SPC_PATH_FOLDER, "%s/%.4u", folder, tyear);
		result = CreateDirectoryA(path, 0);
		if (!result) {
			DWORD xerr = GetLastError();
			if (xerr != ERROR_ALREADY_EXISTS) {
				ret = SPC_LOG_CHECK_FOLDER_YEAR_ERROR;
				spc_err("CreateDirectoryA");
				break;
			}
		}
		snprintf(path, SPC_PATH_FOLDER, 
			"%s/%.4d/%.2d", folder, tyear, tmonth);

		result = CreateDirectoryA(path, 0);
		if (!result) {
			DWORD xerr = GetLastError();
			if (xerr != ERROR_ALREADY_EXISTS) {
				ret = SPC_LOG_CHECK_FILE_YEAR_ERROR;
				spc_err("CreateDirectoryA");
				break;
			}
		}
#else
		/*https://linux.die.net/man/3/mkdir
		//https://linux.die.net/man/2/stat*/
		memset(&buf, 0, sizeof(buf));
		stat(path, &buf);
		if (!S_ISDIR(buf.st_mode)) {
			err = mkdir(path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
			memset(&buf, 0, sizeof(buf));
			stat(path, &buf);
			if (!S_ISDIR(buf.st_mode)) {
				ret = SPC_LOG_CHECK_FOLDER_ERROR;
				spc_err("mkdir, path: %s", path);
				break;
			}
		}

		memset(&buf, 0, sizeof(buf));
		snprintf(path, SPC_PATH_FOLDER, "%s/%.4u", folder, tyear);
		err = stat(path, &buf);
		if (!S_ISDIR(buf.st_mode)) {
			err = mkdir(path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
			if (err) {
				ret = SPC_LOG_CHECK_FOLDER_YEAR_ERROR;
				spc_err("mkdir, path: %s", path);
				break;
			}
		}
		memset(&buf, 0, sizeof(buf));
		snprintf(path, SPC_PATH_FOLDER, 
			"%s/%.4d/%.2d", folder, tyear, tmonth);

		err = stat(path, &buf);
		if (!S_ISDIR(buf.st_mode)) {
			err = mkdir(path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
			if (err) {
				spc_err("mkdir, path: %s", path);
				ret = SPC_LOG_CHECK_FILE_YEAR_ERROR;
				break;
			}
		}
#endif
		snprintf(year_month, 10, "%.4d\\%.2d", tyear, tmonth);

	} while (0);
	return ret;
}
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
void
spc_sleep(unsigned int sec)
{
#ifndef UNIX_LINUX
	Sleep(((DWORD)(sec)) * 1000);
#else
	sleep(sec);
#endif
}
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
void
spc_milli_sleep(unsigned int mill_sec)
{
#ifndef UNIX_LINUX
	Sleep(((DWORD)(mill_sec)));
#else
	usleep(mill_sec * 1000);
#endif
}
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
int
spc_standardize_path(char *fname)
{
	int ret = 0;
	int i = 0;
	while (fname[i]) {
		if (fname[i] == '\\') {
			fname[i] = '/';
		}
		++i;
	}
	return ret;
}
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
int
spc_stdz_topics(char *buff, int *inoutlen, int *ntopics, char **pchar)
{
	int ret = 0, i = 0;
	int count = 1;
	int n = 0;
	char *p = 0, *tmp = 0, *commas = 0;
	do {
		if (!inoutlen) {
			ret = SPC_LOG_TOPIC_NULL;
			break;
		}
		if (!pchar) {
			ret = SPC_LOG_TOPIC_NULL;
			break;
		}
		if (!buff) {
			ret = SPC_LOG_TOPIC_NULL;
			break;
		}
		if (!ntopics) {
			ret = SPC_LOG_TOPIC_NULL;
			break;
		}
		n = *inoutlen;
		i = n - 1;
		while (i > -1) {
			if (buff[i] != ',') {
				break;
			}
			buff[i] = 0;
			--i;
		}
		n = (int)strlen(buff);
		if (n < 1) {
			ret = SPC_LOG_TOPIC_EMPTY;
			break;
		}
		spc_malloc(n + 1, p, char);
		if (!p) {
			ret = SPC_LOG_TOPIC_NULL;
			break;
		}
		memcpy(p, buff, n);
		tmp = buff;
		while (1) {
			commas = strstr(tmp, ",");
			if (!commas) {
				break;
			}
			tmp = ++commas;
			count++;
		}
		(*pchar) = p;
		*ntopics = count;
		spc_console_log("Topic.\n");

	} while (0);
	/*
	//if (p) {
	//	spc_free(p);
	//}
	*/
	return ret;
}
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
int
spc_gen_topics(SPC_LOG_ST *t)
{
	int ret = 0;
	char path[SPC_FULLPATH_LEN + 1];
	/*
	//int renew = 0;
	*/
	SPC_LLU cszize = 0;
	do {
		int i = 0;
		if (t->n_topic < 1) {
			/*
			//ret = SPC_LOG_TOPIC_ZERO;
			*/
			break;
		}
		for (i = 0; i < t->n_topic; ++i) {
			if (t->arr_topic[i].fp) {
				continue;
			}
			do {
				int err = 0;
				snprintf(path, SPC_FULLPATH_LEN, 
					"%s-%s-%.7d.log", 
					t->path_template, t->arr_topic[i].topic,
				    t->arr_topic[i].index);

				SPC_FFOPEN(t->arr_topic[i].fp, path, "a+");
				if (!t->arr_topic[i].fp) {
					ret = SPC_LOG_TOPIC_FOPEN;
					break;
				}
				SPC_FFSEEK(t->arr_topic[i].fp, 0, SEEK_END);
				cszize = (SPC_LLU)SPC_FFTELL(t->arr_topic[i].fp);
				if (cszize < (SPC_LLU)t->file_limit_size) {
					t->arr_topic[i].fizize = (int)cszize;
					break;
				}
				t->arr_topic[i].fizize = (int)cszize;
				SPC_FCLOSE(t->arr_topic[i].fp, err);
				if (err) {
					ret = SPC_LOG_CLOSE_FILE_ERROR;
					break;
				}
				t->arr_topic[i].index++;
			} while (1);
		}
		if (ret) {
			break;
		}
		/*--------------*/
		if (ret) {
			break;
		}
		/*--------------*/
		if (t->renew > SPC_CHANGE_FILE_SIZE) {
			for (i = 0; i < t->n_topic; ++i) {
				do {
					int err = 0;
					SPC_FCLOSE(t->arr_topic[i].fp, err);
					if (err) {
						ret = SPC_LOG_CLOSE_FILE_ERROR;
						break;
					}
					t->arr_topic[i].index = 0;
					t->arr_topic[i].fizize = 0;
					snprintf(path, 
						SPC_FULLPATH_LEN, 
						"%s-%s-%.7d.log", 
						t->path_template,
					    t->arr_topic[i].topic, 
						t->arr_topic[i].index);
					/*
					//t->arr_topic[i].fp = fopen(path,
					"a+");
					*/
					SPC_FFOPEN(t->arr_topic[i].fp, path, "a+");
					if (!t->arr_topic[i].fp) {
						ret = SPC_LOG_TOPIC_FOPEN;
						break;
					}
				} while (0);
				if (ret) {
					break;
				}
			}
			break;
		}
		for (i = 0; i < t->n_topic;) {
			if ((t->arr_topic[i].fizize) < t->file_limit_size) {
				++i;
				continue;
			}
			do {
				int err = 0;

				SPC_FCLOSE(t->arr_topic[i].fp, err);
				if (err) {
					ret = SPC_LOG_CLOSE_FILE_ERROR;
					break;
				}

				t->arr_topic[i].fizize = 0;

				snprintf(path, SPC_FULLPATH_LEN, 
					"%s-%s-%.7d.log", t->path_template, 
					t->arr_topic[i].topic,
				    t->arr_topic[i].index);
				/*
				//t->arr_topic[i].fp = fopen(path, "a+");
				*/
				SPC_FFOPEN(t->arr_topic[i].fp, path, "a+");
				if (!t->arr_topic[i].fp) {
					ret = SPC_LOG_TOPIC_FOPEN;
					break;
				}
				SPC_FFSEEK(t->arr_topic[i].fp, 0, SEEK_END);
				cszize = (SPC_LLU)SPC_FFTELL(t->arr_topic[i].fp);
				if (cszize < (SPC_LLU)t->file_limit_size) {
					break;
				}
				SPC_FCLOSE(t->arr_topic[i].fp, err);
				if (err) {
					ret = SPC_LOG_CLOSE_FILE_ERROR;
					break;
				}
				t->arr_topic[i].index++;
			} while (1);
			++i;
			if (ret) {
				break;
			}
		}
	} while (0);
	return ret;
}

/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
SPC_LLU
spc_milli_now()
{
	SPC_LLU ret = 0;
	time_t t0 = time(0);
	do {
#ifndef UNIX_LINUX
		SYSTEMTIME lt;
		GetLocalTime(&lt);
		ret = t0 * 1000 + lt.wMilliseconds;
#else
		int err = 0;
		struct timespec nanosec;
		err = clock_gettime(CLOCK_REALTIME, &nanosec);
		if (err) {
			break;
		}
		ret = t0 * 1000 + (nanosec.tv_nsec / SPC_MILLION);
#endif
	} while (0);
	return ret;
}
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
#if 0
int
spc_fclose_err(int terr, void *ffp)
{
	spc_err("fpp: 0x%p, terr: %d", ffp, terr);
	return 0;
}

/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
int
spc_fflush_err(int terr, void *ffp)
{
	spc_err("fpp: 0x%p, terr: %d", ffp, terr);
	return 0;
}
#endif
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
#ifndef UNIX_LINUX
void
spc_LockSpinlock(volatile long *p)
{
	while (InterlockedCompareExchange(p, 1, 0) != 0) {
	}
}

void
spc_UnlockSpinlock(volatile long *p)
{
	InterlockedExchange(p, 0);
}
#else
#endif
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/

#ifndef UNIX_LINUX
DWORD WINAPI
spc_trigger_routine(void *arg)
#else
void *
spc_trigger_routine(void *arg)
#endif
{
	SPC_LOG_ST *t = (SPC_LOG_ST *)arg;
	while (1) {
		spc_rel_sem(t->sem_rwfile);
		spc_milli_sleep(t->trigger_thread);
	}
	return 0;
}
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
#ifndef UNIX_LINUX
int
spc_create_thread(SPC_THREAD_ROUTINE f, void *arg, HANDLE *outhd)
#else
int
spc_create_thread(SPC_THREAD_ROUTINE f, void *arg, pthread_t *outid)
#endif
{
	int ret = 0;
	/*
	 */
#ifndef UNIX_LINUX
	DWORD dwThreadId = 0;
	HANDLE hThread = 0;
	hThread = CreateThread(NULL, 0, f, arg, 0, &dwThreadId);
	if (!hThread) {
		ret = SPC_LOG_THREAD_W32_CREATE;
		spc_err("CreateThread");
	}
	*outhd = hThread;
#else
	pthread_t tidd = 0;
	ret = pthread_create(&tidd, 0, f, arg);
	if (ret) {
		spc_err("pthread_create");
	}
	*outid = tidd;
#endif
	return ret;
}
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
int
spc_del_memory()
{
	int ret = 0;
	SPC_LOG_ST *t = &__spc_log_statiic__;
	do {
		
#ifndef UNIX_LINUX
		int isWell = 0;
		if (t->is_master) {
			isWell = (int)UnmapViewOfFile((void *)t->buf);
			if (!isWell) {
				spc_err("UnmapViewOfFile");
				ret = SPC_LOG_SHM_WIN_UNMAP;
			}
		}
		isWell = (int)CloseHandle((HANDLE)t->hd);
		if (!isWell) {
			ret = SPC_LOG_WIN_SHM_CLOSE;
		}
#else
		int i = 0;
		if (t->is_master) {
#ifdef SPC_USING_SPIN_LOCK
			/* Clean spinlock */
			/* https://linux.die.net/man/3/pthread_spin_destroy */
			ret = pthread_spin_destroy((pthread_spinlock_t *)t->mtx_rw);
			if (ret) {
				spc_err("pthread_spin_destroy/mtx_rw");
			}
			for (i = 0; i < t->ncpu; ++i) {
				ret = pthread_spin_destroy(
					(pthread_spinlock_t *)t->arr_mtx[i]);
				if (ret) {
					spc_err("pthread_spin_destroy/arr_mtx");
				}
			}
#else
			/* Clean Mutex */
			/* https://linux.die.net/man/3/pthread_mutex_destroy */
			ret = pthread_mutex_destroy(
				(pthread_mutex_t *)t->mtx_rw);
			if (ret) {
				spc_err("pthread_mutex_destroy/arr_mtx");
			}
			for (i = 0; i < t->ncpu; ++i) {
				ret = pthread_mutex_destroy(
					(pthread_mutex_t *)t->arr_mtx[i]);
				if (ret) {
					spc_err("pthread_mutex_destroy/arr_mtx");
				}
			}
#endif
#ifdef __MACH__
			/* Clear semaphore of MAC OSX. */
			ret = spc_osx_sync_del();
#else
			/* Clean Semaphore */
			/* https://linux.die.net/man/3/sem_destroy */
			ret = sem_destroy((sem_t *)t->sem_rwfile);
			if (ret) {
				spc_err("sem_destroy/sem_rwfile");
			}
			ret = sem_destroy((sem_t *)t->sem_off);
			if (ret) {
				spc_err("sem_destroy/sem_off");
			}
#endif
		}
		ret = munmap((void *)t->buf, (size_t)t->map_mem_size);
		if (ret) {
			ret = SPC_LOG_SHM_UNIX_UNMAP;
			spc_err("munmap");
		}
		if (t->is_master) {
			/* https://linux.die.net/man/3/shm_unlink */
			spc_shm_unlink(t->shared_key, ret);
		}
#endif
	} while (0);
	return ret;
}
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/

int
spc_create_memory(void **output, 
	char *shared_key, 
	int size_shared, char isCreating)
{
	int ret = 0;
	char *p = 0;
	SPC_LOG_ST *t = &__spc_log_statiic__;

	do {
#ifndef UNIX_LINUX
		HANDLE hMapFile = 0;
		char *p = 0;
		if (!output) {
			ret = SPC_LOG_SHM_CREATE_NULL;
			break;
		}
		if (isCreating) {
			hMapFile =
			    CreateFileMappingA(
					INVALID_HANDLE_VALUE, 
					NULL, PAGE_READWRITE, 
					0, size_shared, shared_key);

			if (!hMapFile) {
				spc_err("CreateFileMappingA");
				ret = SPC_LOG_SHM_WIN32_CREATE;
				break;
			}
			spc_console_log("hMapFile: 0x%p", hMapFile);
		} else {
			hMapFile = OpenFileMappingA(
				FILE_MAP_ALL_ACCESS, 0, shared_key);
			if (!hMapFile) {
				ret = SPC_LOG_SHM_WIN32_OPEN;
				spc_err("OpenFileMappingA");
				break;
			}
		}
		if (ret) {
			break;
		}
		p = (char *)MapViewOfFile(hMapFile, 
			FILE_MAP_ALL_ACCESS, 0, 0, size_shared);
		if (!p) {
			ret = SPC_LOG_SHM_WIN32_MAPVIEW;
			spc_err("MapViewOfFile");
			break;
		}
#else
		int hMapFile = 0;
		int err = 0;
		if (isCreating) {
			int retry = 0;
			while (1) {
				hMapFile = shm_open(shared_key, 
					SPC_LOG_UNIX_CREATE_MODE, 
					SPC_LOG_UNIX__SHARED_MODE);

				if (hMapFile < 0) {
					if (retry) {
						spc_err("shm_open, key: %s", shared_key);
						ret = SPC_LOG_SHM_UNIX_CREATE;
					}
					if (!retry) {
						/* shm_unlink(shared_key); */
						spc_shm_unlink(shared_key, err);
						retry++;
						continue;
					}
					break;
				}
				break;
			}
		} else {
			hMapFile = shm_open(shared_key, 
				SPC_LOG_UNIX_OPEN_MODE, 
				SPC_LOG_UNIX__SHARED_MODE);

			if (hMapFile < 0) {
				spc_err("shm_open, key: %s", shared_key);
				ret = SPC_LOG_SHM_UNIX_OPEN;
				break;
			}
		}
		if (ret) {
			break;
		}
		if (t->isProcessMode) {
			if (t->is_master) {
				err = ftruncate(hMapFile, size_shared);
				if (err) {
					spc_err("ftruncate, key: %s", shared_key);
					ret = SPC_LOG_SHM_UNIX_TRUNC;
					break;
				}
			}
		}

		p = (char *)mmap(0, size_shared, 
			SPC_LOG_UNIX_PROT_FLAGS, MAP_SHARED, hMapFile, 0);
		/* p = (char*)mmap(0, size_shared,
			PROT_WRITE, MAP_SHARED, hMapFile, 0); //PROT_WRITE */
		if (p == MAP_FAILED || p == 0) {
			ret = SPC_LOG_SHM_UNIX_MAP_FAILED;
			spc_err(", mmap: %d, size_shared: %d", 
				hMapFile, (int)size_shared) p = 0;
			break;
		}
#endif
		t->hd = hMapFile;
		if (t->isProcessMode && t->is_master) {
			memset(p, 0, size_shared);
		}
		else if (!t->isProcessMode) {
			memset(p, 0, size_shared);
		}
		*output = (void *)p;

	} while (0);
	return ret = 0;
}
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
/* (SPC_LOG_ST*)&__spc_log_statiic__,
volatile long, pthread_spinlock_t, pthread_mutex_t */

int
spc_calculate_size()
{
	int ret = 0;
	int k = 0;
	int n = 0;
	int mtxsize = 0;
	int semsize = 0;
	int size_arr_mtx = 0;
	char *buff = 0;

#ifndef UNIX_LINUX
#ifdef SPC_USING_SPIN_LOCK
	int i = 0;
	char *p = 0;
	int step_size = 0;
#else
#endif
#else
	int i = 0;
	char *p = 0;
	int step_size = 0;
#endif
	SPC_LOG_ST *t = &__spc_log_statiic__;
	size_arr_mtx = t->ncpu * sizeof(void *);
	/*k: For buffer.*/
	k = t->buff_size * t->ncpu * (t->n_topic + 1);
	/*k = t->buff_size * t->ncpu +
		t->buff_size * t->ncpu * t->n_topic;*/
	do {
		if (!t->arr_mtx) {
			spc_malloc(size_arr_mtx, t->arr_mtx, void *);
			if (!t->arr_mtx) {
				ret = SPC_LOG_ARR_MTX_NULL;
				spc_err("SPC_LOG_ARR_MTX_NULL.");
				break;
			}
		}
#ifndef UNIX_LINUX
#ifdef SPC_USING_SPIN_LOCK
		step_size = sizeof(volatile long);
		mtxsize = (1 + t->ncpu) * step_size;
		semsize = 0;
#else
		/*t->mtx_rw: is NamedMutex*/
		mtxsize = 0;
		/*semsize*/
		semsize = 0;
#endif
#else
#ifdef __MACH__
#ifdef SPC_USING_SPIN_LOCK
		step_size = sizeof(pthread_spinlock_t);
#error "not yet implemented."
		/*
			-
		   https://developer.apple.com/documentation/os/os_unfair_lock_t
		   : iOS 10.0+ iPadOS 10.0+ Mac Catalyst 13.1+ macOS 10.12+
				tvOS 10.0+
				visionOS 1.0+
				watchOS 3.0+
			-
		   https://developer.apple.com/library/archive/documentation/System/Conceptual/ManPages_iPhoneOS/man3/OSSpinLockTry.3.html
				- May 26, 2004   Darwin
		*/
#else
		step_size = sizeof(pthread_mutex_t);
#endif
		mtxsize = (1 + t->ncpu) * step_size;
		/*1: For semrw.*/
		/*1: For semOnOff.*/
		semsize = 0;
#else
#ifdef SPC_USING_SPIN_LOCK
		step_size = sizeof(pthread_spinlock_t);
#else
		step_size = sizeof(pthread_mutex_t);
#endif
		mtxsize = (1 + t->ncpu) * step_size;
		/*1: For semrw.*/
		/*1: For semOnOff.*/
		semsize = 2 * sizeof(sem_t);
#endif
#endif
		/*k: For buffer.*/
		/*mtxsize: mutex size.*/
		/*semsize: sem size.*/
		/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
		spc_console_log(
			"buf size: %d, mtxsize: %d, semsize: %d", 
			(int)k, (int)mtxsize, (int)semsize);

		n = k + mtxsize + semsize;
		t->map_mem_size = n;
		/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
		/* // int spc_create_memory(void** output,
			char* shared_key, int size_shared, char isCreating) { */
		if (t->isProcessMode) {
			spc_create_memory((void **)&buff, 
			t->shared_key, n, t->is_master);
		} else {
			spc_malloc(n, buff, char);
		}
		if (!buff) {
			ret = SPC_LOG_BUFF_MALLOC_ERROR;
			spc_err("SPC_LOG_BUFF_MALLOC_ERROR.");
			break;
		}
		t->buf = (spc_gen_data_st *)buff;

#ifndef UNIX_LINUX
#ifdef SPC_USING_SPIN_LOCK

		t->mtx_rw = (void *)(buff + k);

		step_size = sizeof(volatile long);
		p = (buff + k) + step_size;
		for (i = 0; i < t->ncpu; ++i) {
			t->arr_mtx[i] = (void *)(p + i * step_size);
		}
#else
		/*t->mtx_rw: is NamedMutex*/
		/*t->mtx_rw: is NamedSem*/
#endif
		ret = spc_win32_sync_create();
#else
		t->mtx_rw = (void *)(buff + k);
#ifdef SPC_USING_SPIN_LOCK
		if (t->mtx_rw) {
			pthread_spinlock_t *mtx = 0;
			mtx = (pthread_spinlock_t *)t->mtx_rw;
			ret = spc_spinlock_init(mtx, t->isProcessMode);
		}
		if (ret) {
			break;
		}
		step_size = sizeof(pthread_spinlock_t);
		p = buff + k + step_size;
		for (i = 0; i < t->ncpu; ++i) {
			pthread_spinlock_t *mtx = 0;
			t->arr_mtx[i] = (void *)(p + i * step_size);
			mtx = (pthread_spinlock_t *)t->arr_mtx[i];
			ret = spc_spinlock_init(mtx, t->isProcessMode);
			if (ret) {
				break;
			}
		}
#else
		if (t->mtx_rw) {
			pthread_mutex_t *mtx = (pthread_mutex_t *)t->mtx_rw;
			ret = spc_mtx_init(mtx, t->isProcessMode);
		}
		step_size = sizeof(pthread_mutex_t);
		p = buff + k + step_size;
		for (i = 0; i < t->ncpu; ++i) {
			/*pthread_mutex_t* mtx = 0;*/
			t->arr_mtx[i] = (void *)(p + i * step_size);
			/*mtx = (pthread_mutex_t*)t->arr_mtx[i];*/
			ret = spc_mtx_init(t->arr_mtx[i], t->isProcessMode);
			if (ret) {
				break;
			}
		}
#endif
		if (ret) {
			break;
		}
#ifdef __MACH__
		/* Semaphore UNIX_LINUX MAC OSX */
		ret = spc_osx_sync_create();
		if (ret) {
			break;
		}
#else
		/*Semaphore UNIX_LINUX*/
		p = buff + k + mtxsize;
		t->sem_rwfile = (void *)p;
		t->sem_off = (void *)(p + sizeof(sem_t));
		/*
		 * https://linux.die.net/man/3/sem_init
		 * #include <semaphore.h>
		 * int sem_init(sem_t *sem, int pshared, unsigned int value);
		 */
		if (t->isProcessMode) {
			if (t->is_master) {
				int err = 0;
				err = sem_init((sem_t *)t->sem_rwfile, (int)t->isProcessMode, 0);

				if (err) {
					ret = SPC_LOG_SEM_INIT_UNIX;
					spc_err("sem_init");
					break;
				}
				err = sem_init((sem_t *)t->sem_off, (int)t->isProcessMode, 0);

				if (err) {
					ret = SPC_LOG_SEM_INIT_UNIX;
					spc_err("sem_init");
					break;
				}
			}
		} else {
			int err = 0;
			err = sem_init((sem_t *)t->sem_rwfile, (int)t->isProcessMode, 0);

			if (err) {
				ret = SPC_LOG_SEM_INIT_UNIX;
				spc_err("sem_init");
				break;
			}
			err = sem_init((sem_t *)t->sem_off, (int)t->isProcessMode, 0);

			if (err) {
				ret = SPC_LOG_SEM_INIT_UNIX;
				spc_err("sem_init");
				break;
			}
		}
#endif
#endif
	} while (0);

	return ret;
}
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/

#ifndef UNIX_LINUX

#define WIN32_MUTEX_RW	"Global\\%s_%s"
#define WIN32_MUTEX_BUF "Global\\%s_%s_%0.2d"

static int
spc_win32_sync_create_mutex()
{
	int ret = 0;
	SPC_LOG_ST *t = &__spc_log_statiic__;
	char nameobj[SPC_SHARED_NAME_LEN];
	HANDLE hd = 0;
	int i = 0;
	do {
		if (!t->isProcessMode) {
			hd = CreateMutexA(0, 0, 0);
			if (!hd) {
				ret = SPC_LOG_MTX_WIN32_CREATED_ERROR;
				spc_err("CreateMutexA");
				break;
			}
			t->mtx_rw = hd;

			for (i = 0; i < t->ncpu; ++i) {
				hd = CreateMutexA(0, 0, 0);
				if (!hd) {
					ret = SPC_LOG_MTX_WIN32_CREATED_ERROR;
					spc_err("CreateMutexA");
					break;
				}
				t->arr_mtx[i] = hd;
			}
			break;
		}

		if (t->isProcessMode && t->is_master) {
			snprintf(nameobj, 
				SPC_SHARED_NAME_LEN, 
				WIN32_MUTEX_RW, 
				SPC_MTX_NAME_OFF, t->shared_key);

			hd = CreateMutexA(0, 0, nameobj);
			if (!hd) {
				ret = SPC_LOG_MTX_WIN32_CREATED_ERROR;
				spc_err("CreateMutexA");
				break;
			}
			t->mtx_rw = hd;

			for (i = 0; i < t->ncpu; ++i) {
				snprintf(nameobj, 
					SPC_SHARED_NAME_LEN, 
					WIN32_MUTEX_BUF, 
					SPC_MTX_NAME_OFF, t->shared_key, i);

				hd = CreateMutexA(0, 0, nameobj);
				if (!hd) {
					ret = SPC_LOG_MTX_WIN32_CREATED_ERROR;
					spc_err("CreateMutexA");
					break;
				}
				t->arr_mtx[i] = hd;
			}
			break;
		}
		/*(t->isProcessMode && !t->is_master) */
		snprintf(nameobj, SPC_SHARED_NAME_LEN, 
			WIN32_MUTEX_RW, 
			SPC_MTX_NAME_OFF, t->shared_key);

		hd = OpenMutexA(MUTEX_ALL_ACCESS, 0, nameobj);
		if (!hd) {
			ret = SPC_LOG_MTX_WIN32_OPEN_ERROR;
			spc_err("OpenMutexA");
			break;
		}
		t->mtx_rw = hd;

		for (i = 0; i < t->ncpu; ++i) {
			snprintf(nameobj, SPC_SHARED_NAME_LEN, 
				WIN32_MUTEX_BUF, 
				SPC_MTX_NAME_OFF, t->shared_key, i);

			hd = OpenMutexA(MUTEX_ALL_ACCESS, 0, nameobj);
			if (!hd) {
				ret = SPC_LOG_MTX_WIN32_OPEN_ERROR;
				spc_err("OpenMutexA");
				break;
			}
			t->arr_mtx[i] = hd;
		}
	} while (0);
	return ret;
}

#define WIN32_SEM_RW	"Global\\%s_%s"
#define WIN32_SEM_OFF	"Global\\%s_%s"

static int
spc_win32_sync_create_sem()
{
	int ret = 0;

	SPC_LOG_ST *t = &__spc_log_statiic__;
	char nameobj[SPC_SHARED_NAME_LEN];
	HANDLE hd = 0;
	do {
		if (!t->isProcessMode) {
			hd = CreateSemaphoreA(0, 0, 1, 0);
			if (!hd) {
				ret = SPC_LOG_SEM_WIN32_CREATED_ERROR;
				spc_err("CreateSemaphoreA");
				break;
			}
			t->sem_rwfile = hd;
			hd = CreateSemaphoreA(0, 0, 1, 0);
			if (!hd) {
				ret = SPC_LOG_SEM_WIN32_CREATED_ERROR;
				spc_err("CreateSemaphoreA");
				break;
			}
			t->sem_off = hd;
			break;
		}

		if (t->isProcessMode && t->is_master) 
		{
			snprintf(nameobj, SPC_SHARED_NAME_LEN, 
				WIN32_SEM_RW, SPC_SEM_NAME_RW, t->shared_key);

			hd = CreateSemaphoreA(0, 0, 1, nameobj);
			if (!hd) {
				ret = SPC_LOG_SEM_WIN32_CREATED_ERROR;
				spc_err("CreateSemaphoreA");
				break;
			}
			t->sem_rwfile = hd;
			snprintf(nameobj, SPC_SHARED_NAME_LEN, 
				WIN32_SEM_OFF, SPC_SEM_NAME_OFF, t->shared_key);

			hd = CreateSemaphoreA(0, 0, 1, nameobj);
			if (!hd) {
				ret = SPC_LOG_SEM_WIN32_CREATED_ERROR;
				spc_err("CreateSemaphoreA");
				break;
			}
			t->sem_off = hd;
			break;
		}

		snprintf(nameobj, SPC_SHARED_NAME_LEN, 
			WIN32_SEM_RW, SPC_SEM_NAME_RW, t->shared_key);

		hd = OpenSemaphoreA(SEMAPHORE_ALL_ACCESS, 0, nameobj);
		if (!hd) {
			ret = SPC_LOG_SEM_WIN32_CREATED_ERROR;
			spc_err("CreateSemaphoreA");
			break;
		}
		t->sem_rwfile = hd;
		snprintf(nameobj, SPC_SHARED_NAME_LEN, 
			WIN32_SEM_OFF, 
			SPC_SEM_NAME_OFF, t->shared_key);

		hd = OpenSemaphoreA(SEMAPHORE_ALL_ACCESS, 0, nameobj);
		if (!hd) {
			ret = SPC_LOG_SEM_WIN32_CREATED_ERROR;
			spc_err("CreateSemaphoreA");
			break;
		}
		t->sem_off = hd;

	} while (0);

	return ret;
}

int
spc_win32_sync_create()
{
	int ret = 0;
	SPC_LOG_ST *t = &__spc_log_statiic__;
	char nameobj[SPC_SHARED_NAME_LEN];
	do {
#ifdef SPC_USING_SPIN_LOCK
#else
		if (!t->arr_mtx) {
			int n = t->ncpu * sizeof(void *);
			spc_malloc(n, t->arr_mtx, void *);
			if (!t->arr_mtx) {
				ret = SPC_LOG_ARR_MTX_NULL;
				spc_err("SPC_LOG_ARR_MTX_NULL");
				break;
			}
		}
		ret = spc_win32_sync_create_mutex();

#endif
		if (ret) {
			break;
		}

		ret = spc_win32_sync_create_sem();
	} while (0);
	return ret;
}
#else
#ifdef __MACH__
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
int
spc_osx_sync_del()
{
	int ret = 0;
	SPC_LOG_ST *t = &__spc_log_statiic__;
	char nameobj[SPC_SHARED_NAME_LEN];
	do {
		if (!t->sem_rwfile) {
			spc_console_log("Already cleared.");
			break;
		}
		if ((t->isProcessMode && t->is_master) || !t->isProcessMode) {
			snprintf(nameobj, SPC_SHARED_NAME_LEN, 
				"%s_%s", SPC_SEM_NAME_RW, t->shared_key);

			if (sem_close((sem_t *)t->sem_rwfile) == -1) {
				spc_err("sem_close");
				ret = SPC_LOG_OSX_SEM_CLOSE;
			}
			if (sem_unlink(nameobj) == -1) {
				spc_err("sem_unlink");
				ret = SPC_LOG_OSX_SEM_UNLINK;
			}
			t->sem_rwfile = 0;

			snprintf(nameobj, SPC_SHARED_NAME_LEN, 
				"%s_%s", SPC_SEM_NAME_OFF, t->shared_key);

			if (sem_close((sem_t *)t->sem_off) == -1) {
				spc_err("sem_close");
				ret = SPC_LOG_OSX_SEM_CLOSE;
			}
			if (sem_unlink(nameobj) == -1) {
				spc_err("sem_unlink");
				ret = SPC_LOG_OSX_SEM_UNLINK;
			}
			t->sem_off = 0;
		} else {
			snprintf(nameobj, SPC_SHARED_NAME_LEN, 
				"%s_%s", SPC_SEM_NAME_RW, t->shared_key);

			if (sem_close((sem_t *)t->sem_rwfile) == -1) {
				spc_err("sem_close");
				ret = SPC_LOG_OSX_SEM_CLOSE;
			}
			t->sem_rwfile = 0;

			snprintf(nameobj, SPC_SHARED_NAME_LEN, 
				"%s_%s", SPC_SEM_NAME_OFF, t->shared_key);

			if (sem_close((sem_t *)t->sem_off) == -1) {
				spc_err("sem_close");
				ret = SPC_LOG_OSX_SEM_CLOSE;
			}
			t->sem_off = 0;
		}
	} while (0);
	return ret;
}
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
static sem_t *
spc_osx_sync_sem_create(char *nameobj, int *pret)
{
	sem_t *hd = 0;
	int retry = 0;
	int ret = 0;

	do {
		ret = 0;
		hd = sem_open(nameobj, 
			SPC_LOG_UNIX_CREATE_MODE, 
			SPC_LOG_UNIX__SHARED_MODE, 1);
		if (hd != SEM_FAILED) {
			break;
		}
		ret = SPC_LOG_SEM_OSX_CREATED_ERROR;
		spc_err("sem_open, name: %s.", nameobj);
		if (retry) {
			break;
		}
		ret = sem_unlink(nameobj);
		if (ret) {
			spc_err("sem_unlink, name: %s.", nameobj);
			ret = SPC_LOG_SEM_OSX_UNLINK_ERROR;
			break;
		}
		retry++;
		continue;
	} while (1);

	if (pret) {
		*pret = ret;
	}

	return hd;
}

int
spc_osx_sync_create()
{
	int ret = 0;
	SPC_LOG_ST *t = &__spc_log_statiic__;
	char nameobj[SPC_SHARED_NAME_LEN];
	do {
#ifdef SPC_USING_SPIN_LOCK
#else
#endif

		if ((t->isProcessMode && t->is_master) || !t->isProcessMode) {
			sem_t *hd = 0;
			snprintf(nameobj, SPC_SHARED_NAME_LEN, 
				"%s_%s", SPC_SEM_NAME_RW, t->shared_key);
			hd = spc_osx_sync_sem_create(nameobj, &ret);

			if (ret) {
				break;
			}
			t->sem_rwfile = hd;

			snprintf(nameobj, SPC_SHARED_NAME_LEN, 
				"%s_%s", SPC_SEM_NAME_OFF, t->shared_key);
			hd = spc_osx_sync_sem_create(nameobj, &ret);
			if (ret) {
				break;
			}
			t->sem_off = hd;
			sem_wait(hd);
		} else {
			sem_t *hd = 0;
			snprintf(nameobj, SPC_SHARED_NAME_LEN, 
				"%s_%s", SPC_SEM_NAME_RW, t->shared_key);
			hd = sem_open(nameobj, SPC_LOG_UNIX_OPEN_MODE);
			if (hd == SEM_FAILED) {
				spc_err("sem_open");
				ret = SPC_LOG_SEM_OSX_CREATED_ERROR;
				break;
			}
			t->sem_rwfile = hd;

			snprintf(nameobj, 
				SPC_SHARED_NAME_LEN, "%s_%s", 
				SPC_SEM_NAME_OFF, t->shared_key);
			hd = sem_open(nameobj, SPC_LOG_UNIX_OPEN_MODE);
			if (hd == SEM_FAILED) {
				spc_err("sem_open");
				ret = SPC_LOG_SEM_OSX_CREATED_ERROR;
				break;
			}
			t->sem_off = hd;
		}
	} while (0);
	return ret;
}
#else
#ifdef SPC_USING_SPIN_LOCK
/*Apply for LINUX-Unix but not MACH*/
int
spc_spinlock_init(pthread_spinlock_t *mtx, char shared)
{
	int ret = 0;
	int err = 0;
	SPC_LOG_ST *t = &__spc_log_statiic__;
	do {
		if (!shared) {
			err = pthread_spin_init(mtx, PTHREAD_PROCESS_PRIVATE);
			if (err) {
				ret = SPC_LOG_SPINLOCK_INIT_PRIVATE;
				spc_err("pthread_spin_init");
			}
			break;
		}

		if (t->is_master) {
			err = pthread_spin_init(mtx, PTHREAD_PROCESS_SHARED);
			if (err) {
				ret = SPC_LOG_SPINLOCK_INIT_SHARED;
				spc_err("pthread_spin_init");
			}
		}

	} while (0);
	return ret;
}
#endif
#endif
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
int
spc_mtx_init(void *obj, char shared)
{
	int ret = 0;
	int err = 0;
	SPC_LOG_ST *t = &__spc_log_statiic__;
	pthread_mutex_t *mtx = (pthread_mutex_t *)obj;
	pthread_mutexattr_t psharedm;

	do {
		if (t->isProcessMode && !t->is_master) {
			break;
		}

		if (!shared) {
			err = pthread_mutex_init(mtx, 0);
			if (err) {
				ret = SPC_LOG_MTX_INIT_ERR;
				spc_err("pthread_mutex_init");
				break;
			}
			break;
		}

		err = pthread_mutexattr_init(&psharedm);
		if (err) {
			ret = SPC_LOG_MTX_ATT_SHARED_MODE;
			spc_err("pthread_mutexattr_setpshared");
			break;
		}
		err = pthread_mutexattr_setpshared(
			&psharedm, PTHREAD_PROCESS_SHARED);

		if (err) {
			ret = SPC_LOG_MTX_ATT_SHARED_MODE_SET;
			spc_err("pthread_mutexattr_setpshared");
			break;
		}
		err = pthread_mutex_init(mtx, &psharedm);
		if (err) {
			ret = SPC_LOG_SHM_UNIX_INIT_MUTEX;
			spc_err("pthread_mutex_init");
			break;
		}
	} while (0);

	return ret;
}
#endif

/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
static void
spc_fmt_segment(spc_gen_data_st *sgment)
{
	SPC_LOG_ST *t = &__spc_log_statiic__;
	sgment->total = t->buff_size;
	sgment->pl = 0;
	sgment->pc = 0;
}
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
int
spc_init_segments()
{
	int ret = 0;
	char *p = 0;
	char *seg = 0;
	int i = 0;
	int k = 0;
	int step = 0;
	spc_gen_data_st *sgment = 0;
	SPC_LOG_ST *t = &__spc_log_statiic__;
	p = (char *)t->buf;
	if (!t->range) {
		t->range = t->buff_size;
		t->range -= sizeof(spc_gen_data_st);
		t->range -= (t->max_sz_msg + SPC_RL_BUF);

		t->krange = t->range + t->max_sz_msg;
	}
	for (i = 0; i < t->ncpu; ++i) {
		seg = p + i * t->buff_size;
		sgment = (spc_gen_data_st *)seg;
		spc_fmt_segment(sgment);
	}
	step = t->buff_size * t->ncpu;
	for (k = 0; k < t->n_topic; ++k) {
		p += step;
		t->arr_topic[k].buf = (spc_gen_data_st *)p;
		for (i = 0; i < t->ncpu; ++i) {
			seg = p + i * t->buff_size;
			sgment = (spc_gen_data_st *)seg;
			spc_fmt_segment(sgment);
		}
	}
	return ret;
}
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/

int
spc_allocate_topics()
{
	int ret = 0;
	int i = 0;
	char *p0 = 0;
	char *p1 = 0;
	int n = 0;
	int szitopics = 0;
	SPC_LOG_ST *t = &__spc_log_statiic__;
	do {
		if (!t->n_topic) {
			break;
		}
		szitopics = sizeof(SPC_TOPIC_ST) * t->n_topic;
		spc_malloc(szitopics, t->arr_topic, SPC_TOPIC_ST);

		if (!t->arr_topic) {
			ret = SPC_LOG_TOPIC_MEMORY;
			break;
		}
		p0 = t->topics;
		for (i = 0; i < t->n_topic; ++i) {
			p1 = strstr(p0, ",");
			if (!p1) {
				char *tpp = t->arr_topic[i].topic;
				snprintf(tpp, SPC_TOPIC_SIZE, "%s", p0);
				continue;
			}
			n = (int)(p1 - p0);
			if (n > 0) {
				int szz = 0;
				char *tpp = 0;

				szz = SPC_MIN_AB(SPC_TOPIC_SIZE, n + 1);
				tpp = t->arr_topic[i].topic;
				snprintf(tpp, szz, "%s", p0);
			}
			p1++;
			p0 = p1;
		}
	} while (0);
	return ret;
}
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
int
spc_gen_sync_tool()
{
	int ret = 0;
	do {
		ret = spc_allocate_topics();
		if (ret) {
			break;
		}
		ret = spc_calculate_size();
		if (ret) {
			break;
		}
		ret = spc_init_segments();
		if (ret) {
			break;
		}
	} while (0);
	return ret;
}
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/

int
spc_clean_sync_tool()
{
	int ret = 0;
	SPC_LOG_ST *t = &__spc_log_statiic__;
	do {
		if (t->n_topic > 0) {
			spc_free(t->topics);
			spc_free(t->arr_topic);
		}
#ifndef UNIX_LINUX
#ifdef SPC_USING_SPIN_LOCK
#else
		int i = 0;
		SPC_CloseHandle(t->mtx_rw);
		for (i = 0; i < t->ncpu; ++i) {
			SPC_CloseHandle(t->arr_mtx[i]);
		}
#endif
		SPC_CloseHandle(t->sem_rwfile);
		SPC_CloseHandle(t->sem_off);
#else
#ifdef __MACH__
		/* Clear semaphore of MAC OSX. */
		ret = spc_osx_sync_del();
#endif
#endif
		if (t->isProcessMode) {
			ret = spc_del_memory();
		} else {
			spc_free(t->buf);
		}
		spc_free(t->arr_mtx);
	} while (0);
	return ret;
}
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
SPC_LLU
spc_process_id()
{
	/*SPC_LLU ret = 0;*/
#ifndef UNIX_LINUX
	return (SPC_LLU)GetCurrentProcessId();
#else
	return (SPC_LLU)getpid();
#endif
	/*return ret;*/
}
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
static const char *__spc_err_text__[SPC_END_ERROR + 1];

void
spl_err_txt_init()
{
	__spc_err_text__[SPC_NO_ERROR] = "SPC_NO_ERROR";
	__spc_err_text__[SPC_INIT_PATH_FOLDER_EMPTY_ERROR] = "SPC_INIT_PATH_FOLDER_EMPTY_ERROR";
	__spc_err_text__[SPC_LOG_LEVEL_ERROR] = "SPC_LOG_LEVEL_ERROR";
	__spc_err_text__[SPC_ERROR_CREATE_MUTEX] = "SPC_ERROR_CREATE_MUTEX";
	__spc_err_text__[SPC_ERROR_CREATE_SEM] = "SPC_ERROR_CREATE_SEM";
	__spc_err_text__[SPC_LOG_BUFF_SIZE_ERROR] = "SPC_LOG_BUFF_SIZE_ERROR";
	__spc_err_text__[SPC_LOG_MAX_SZ_MSG_ERROR] = "SPC_LOG_MAX_SZ_MSG_ERROR";
	__spc_err_text__[SPC_LOG_BUFF_MALLOC_ERROR] = "SPC_LOG_BUFF_MALLOC_ERROR";
	__spc_err_text__[SPC_LOG_FOLDER_ERROR] = "SPC_LOG_FOLDER_ERROR";
	__spc_err_text__[SPC_LOG_CREATE_THREAD_ERROR] = "SPC_LOG_CREATE_THREAD_ERROR";
	__spc_err_text__[SPC_LOG_FMT_NULL_ERROR] = "SPC_LOG_FMT_NULL_ERROR";
	__spc_err_text__[SPC_LOG_MEM_GEN_FILE_ERROR] = "SPC_LOG_MEM_GEN_FILE_ERROR";
	__spc_err_text__[SPC_LOG_MEM_MALLOC_ERROR] = "SPC_LOG_MEM_MALLOC_ERROR";
	__spc_err_text__[SPC_LOG_OPEN_FILE_ERROR] = "SPC_LOG_OPEN_FILE_ERROR";
	__spc_err_text__[SPC_LOG_OPEN1_FILE_ERROR] = "SPC_LOG_OPEN1_FILE_ERROR";
	__spc_err_text__[SPC_LOG_CLOSE_FILE_ERROR] = "SPC_LOG_CLOSE_FILE_ERROR";
	__spc_err_text__[SPC_LOG_SEM_NULL_ERROR] = "SPC_LOG_SEM_NULL_ERROR";
	__spc_err_text__[SPC_LOG_SEM_WIN32_CREATED_ERROR] = "SPC_LOG_SEM_WIN32_CREATED_ERROR";
	__spc_err_text__[SPC_LOG_MTX_WIN32_CREATED_ERROR] = "SPC_LOG_MTX_WIN32_CREATED_ERROR";
	__spc_err_text__[SPC_LOG_ROT_SIZE_ERROR] = "SPC_LOG_ROT_SIZE_ERROR";
	__spc_err_text__[SPC_LOG_TOPIC_EMPTY] = "SPC_LOG_TOPIC_EMPTY";
	__spc_err_text__[SPC_LOG_TOPIC_NULL] = "SPC_LOG_TOPIC_NULL";
	__spc_err_text__[SPC_LOG_MEM_FILE_MALLOC_ERROR] = "SPC_LOG_MEM_FILE_MALLOC_ERROR";
	__spc_err_text__[SPC_LOG_CHECK_FOLDER_ERROR] = "SPC_LOG_CHECK_FOLDER_ERROR";
	__spc_err_text__[SPC_LOG_CHECK_FOLDER_YEAR_ERROR] = "SPC_LOG_CHECK_FOLDER_YEAR_ERROR";
	__spc_err_text__[SPC_LOG_CHECK_FILE_YEAR_ERROR] = "SPC_LOG_CHECK_FILE_YEAR_ERROR";
	__spc_err_text__[SPC_LOG_CHECK_FOLDER_NULL_ERROR] = "SPC_LOG_CHECK_FOLDER_NULL_ERROR";
	__spc_err_text__[SPC_LOG_MUTEX_NULL_ERROR] = "SPC_LOG_MUTEX_NULL_ERROR";
	__spc_err_text__[SPC_LOG_ST_NAME_NULL_ERROR] = "SPC_LOG_ST_NAME_NULL_ERROR";
	__spc_err_text__[SPC_LOG_TIME_NULL_ERROR] = "SPC_LOG_TIME_NULL_ERROR";
	__spc_err_text__[SPC_LOG_TIME_NANO_NULL_ERROR] = "SPC_LOG_TIME_NANO_NULL_ERROR";
	__spc_err_text__[SPC_LOG_STAT_FOLDER_ERROR] = "SPC_LOG_STAT_FOLDER_ERROR";
	__spc_err_text__[SPC_LOG_PRINTF_ERROR] = "SPC_LOG_PRINTF_ERROR";
	__spc_err_text__[SPC_LOG_TOPIC_ZERO] = "SPC_LOG_TOPIC_ZERO";
	__spc_err_text__[SPC_LOG_TOPIC_MEMORY] = "SPC_LOG_TOPIC_MEMORY";
	__spc_err_text__[SPC_LOG_TOPIC_FOPEN] = "SPC_LOG_TOPIC_FOPEN";
	__spc_err_text__[SPC_LOG_TOPIC_FLUSH] = "SPC_LOG_TOPIC_FLUSH";
	__spc_err_text__[SPC_LOG_TOPIC_BUFF_MEM] = "SPC_LOG_TOPIC_BUFF_MEM";
	__spc_err_text__[SPC_LOG_ALOCK_NUM] = "SPC_LOG_ALOCK_NUM";
	__spc_err_text__[SPC_LOG_ALOCK_NULL] = "SPC_LOG_ALOCK_NULL";
	__spc_err_text__[SPC_LOG_SHM_CREATE_NULL] = "SPC_LOG_SHM_CREATE_NULL";
	__spc_err_text__[SPC_LOG_SHM_WIN_UNMAP] = "SPC_LOG_SHM_WIN_UNMAP";
	__spc_err_text__[SPC_LOG_SHM_UNIX_CREATE] = "SPC_LOG_SHM_UNIX_CREATE";
	__spc_err_text__[SPC_LOG_SHM_UNIX_OPEN] = "SPC_LOG_SHM_UNIX_OPEN";
	__spc_err_text__[SPC_LOG_SHM_UNIX_TRUNC] = "SPC_LOG_SHM_UNIX_TRUNC";
	__spc_err_text__[SPC_LOG_SHM_UNIX_MAP_FAILED] = "SPC_LOG_SHM_UNIX_MAP_FAILED";
	__spc_err_text__[SPC_LOG_WIN_SHM_CLOSE] = "SPC_LOG_WIN_SHM_CLOSE";
	__spc_err_text__[SPC_LOG_SHM_UNIX_UNMAP] = "SPC_LOG_SHM_UNIX_UNMAP";
	__spc_err_text__[SPC_LOG_VAR_NULL] = "SPC_LOG_VAR_NULL";
	__spc_err_text__[SPC_LOG_ARR_MTX_NULL] = "SPC_LOG_ARR_MTX_NULL";
	__spc_err_text__[SPC_LOG_ARR_BUFF_NULL] = "SPC_LOG_ARR_BUFF_NULL";
	__spc_err_text__[SPC_LOG_MTX_ATT_SHARED_MODE] = "SPC_LOG_MTX_ATT_SHARED_MODE";
	__spc_err_text__[SPC_LOG_MTX_ATT_SHARED_MODE_SET] = "SPC_LOG_MTX_ATT_SHARED_MODE_SET";
	__spc_err_text__[SPC_LOG_MTX_INIT_ERR] = "SPC_LOG_MTX_INIT_ERR";
	__spc_err_text__[SPC_LOG_SHM_UNIX_INIT_MUTEX] = "SPC_LOG_SHM_UNIX_INIT_MUTEX";
	__spc_err_text__[SPC_LOG_SPINLOCK_INIT_SHARED] = "SPC_LOG_SPINLOCK_INIT_SHARED";
	__spc_err_text__[SPC_LOG_SPINLOCK_INIT_PRIVATE] = "SPC_LOG_SPINLOCK_INIT_PRIVATE";
	__spc_err_text__[SPC_LOG_SEM_INIT_UNIX] = "SPC_LOG_SEM_INIT_UNIX";
	__spc_err_text__[SPC_LOG_THREAD_W32_CREATE] = "SPC_LOG_THREAD_W32_CREATE";
	__spc_err_text__[SPC_LOG_THREAD_PX_CREATE] = "SPC_LOG_THREAD_PX_CREATE";
	__spc_err_text__[SPC_LOG_MACH_GETTIME_ERROR] = "SPC_LOG_MACH_GETTIME_ERROR";
	__spc_err_text__[SPC_LOG_MACH_CLOCK_SERVICE_ERROR] = "SPC_LOG_MACH_CLOCK_SERVICE_ERROR";
	__spc_err_text__[SPC_LOG_OSX_SEM_CLOSE] = "SPC_LOG_OSX_SEM_CLOSE";
	__spc_err_text__[SPC_LOG_OSX_SEM_UNLINK] = "SPC_LOG_OSX_SEM_UNLINK";
	__spc_err_text__[SPC_LOG_SEM_OSX_CREATED_ERROR] = "SPC_LOG_SEM_OSX_CREATED_ERROR";
	__spc_err_text__[SPC_LOG_SEM_INIT_OSX] = "SPC_LOG_SEM_INIT_OSX";
	__spc_err_text__[SPC_LOG_SEM_OSX_UNLINK_ERROR] = "SPC_LOG_SEM_OSX_UNLINK_ERROR";
	__spc_err_text__[SPC_LOG_WIN32_MTX_LOCK] = "SPC_LOG_WIN32_MTX_LOCK";
	__spc_err_text__[SPC_LOG_WIN32_MTX_UNLOCK] = "SPC_LOG_WIN32_MTX_UNLOCK";
	__spc_err_text__[SPC_LOG_WIN32_SEM_WAIT] = "SPC_LOG_WIN32_SEM_WAIT";
	__spc_err_text__[SPC_LOG_WIN32_SEM_REL] = "SPC_LOG_WIN32_SEM_REL";
	__spc_err_text__[SPC_LOG_PX_SEM_WAIT] = "SPC_LOG_PX_SEM_WAIT";
	__spc_err_text__[SPC_LOG_PX_SEM_REL] = "SPC_LOG_PX_SEM_REL";
	__spc_err_text__[SPC_LOG_PX_MTX_LOCK] = "SPC_LOG_PX_MTX_LOCK";
	__spc_err_text__[SPC_LOG_PX_MTX_UNLOCK] = "SPC_LOG_PX_MTX_UNLOCK";
	__spc_err_text__[SPC_LOG_PX_SPIN_LOCK] = "SPC_LOG_PX_SPIN_LOCK";
	__spc_err_text__[SPC_LOG_PX_SPIN_UNLOCK] = "SPC_LOG_PX_SPIN_UNLOCK";
	__spc_err_text__[SPC_LOG_MTX_WIN32_OPEN_ERROR] = "SPC_LOG_MTX_WIN32_OPEN_ERROR";
	__spc_err_text__[SPC_LOG_SEM_WIN32_OPEN_ERROR] = "SPC_LOG_SEM_WIN32_OPEN_ERROR";
	__spc_err_text__[SPC_LOG_SHM_WIN32_CREATE] = "SPC_LOG_SHM_WIN32_CREATE";
	__spc_err_text__[SPC_LOG_SHM_WIN32_OPEN] = "SPC_LOG_SHM_WIN32_OPEN";
	__spc_err_text__[SPC_LOG_SHM_WIN32_MAPVIEW] = "SPC_LOG_SHM_WIN32_MAPVIEW";

	__spc_err_text__[SPC_END_ERROR] = "SPC_END_ERROR";
}
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
const char *
spc_err_txt(int i)
{
	if (i < 0) {
		return "UNKNOW - less than 0.";
	}
	if (i > SPC_END_ERROR) {
		return "UNKNOW - greater than SPC_END_ERROR.";
	}
	return __spc_err_text__[i];
}
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/

#ifndef UNIX_LINUX
#else
#endif

#ifndef UNIX_LINUX
#ifdef SPC_USING_SPIN_LOCK
#else
#endif
#else
#ifdef SPC_USING_SPIN_LOCK
#else
#endif
#endif
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
