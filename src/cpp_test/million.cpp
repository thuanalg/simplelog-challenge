#include "simplelog.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifndef UNIX_LINUX
	#include <Windows.h>
	DWORD WINAPI win32_thread_routine(LPVOID lpParam);
#else
	#include <unistd.h>
	#include <pthread.h>
	void* posix_thread_routine(void* lpParam);
#endif // !UNIX_LINUX

void dotest();
void* main_mtx = 0;
int off_process = 0;
int set_off_process(int val) {
	int ret = 0;
	spl_mutex_lock(main_mtx);
	do {
		off_process = val;
	} while (0);
	spl_mutex_unlock(main_mtx);
	return ret;
}

int get_off_process() {
	int ret = 0;
	spl_mutex_lock(main_mtx);
	do {
		ret = off_process;
	} while (0);
	spl_mutex_unlock(main_mtx);
	return ret;
}
//int number = 2;
int number = 20;
int number_thread = 20;
#define MY_NUMBER_THREAD		"--n_thread="
#define MY_OPT_MASTER			"--is_master="
#define MY_LOOP_COUNT			"--loop_count="
int is_master = 0;
int loop_count = 1000;

int main(int argc, char* argv[]) {
	char pathcfg[1024];
	char* path = (char*)"simplelog.cfg";
	char nowfmt[64];
	int n = 0, ret = 0, i = 0;
	if (argc < 2) {
		exit(1);
	}
	for (i = 1; i < argc; ++i) {
		if (strstr(argv[i], MY_NUMBER_THREAD)) {
			sscanf(argv[i], MY_NUMBER_THREAD"%d", &number_thread);
			continue;
		}
		if (strstr(argv[i], MY_OPT_MASTER)) {
			sscanf(argv[i], MY_OPT_MASTER"%d", &is_master);
			continue;
		}
		if (strstr(argv[i], MY_LOOP_COUNT)) {
			sscanf(argv[i], MY_LOOP_COUNT"%d", &loop_count);
			continue;
		}		
	}

	main_mtx = spl_mutex_create(0);
	spl_console_log("Main thread.\n");

	snprintf(pathcfg, 1024, path);
	n = strlen(pathcfg);
	for (i = 0; i < n; ++i) {
		if (pathcfg[i] == '\\') {
			pathcfg[i] = '/';
		}
	}
	// Init log with "pathcfg" path of file, after starting well, ready to use.
	ret = spl_init_log(pathcfg, is_master);
	if (ret) {
		spl_console_log("spl_init_log ret: %d", ret);
		exit(1);
	}
	n = 0;
	dotest();
	while (1) {
		FILE* fp = 0;
		
		spl_sleep(10);
		
		spllog(SPL_LOG_DEBUG, "%s", "Looping for waiting trigger.\n");

		if (is_master) {
			fp = fopen("trigger_master.txt", "r");
		}
		else {
			fp = fopen("trigger.txt", "r");
		}

		if (fp) {
			fclose(fp);
			break;
		}

	}
	spllog(SPL_LOG_INFO, "%s", "set_off_process.\n");
	set_off_process(1);
	spl_sleep(1000 *10);
	spl_console_log("Main close: spl_finish_log, is_master: %d.\n", is_master);
	spl_finish_log(is_master);
	return EXIT_SUCCESS;
}
void dotest() {
	int i = 0;
#ifndef UNIX_LINUX
	DWORD dwThreadId = 0;
	HANDLE hThread = 0;
	for (i = 0; i < number_thread; ++i) {
		hThread = CreateThread(NULL, 0, win32_thread_routine, 0, 0, &dwThreadId);
	}
#else
	pthread_t idd = 0;
	for (i = 0; i < number_thread; ++i) {
		int err = pthread_create(&idd, 0, posix_thread_routine, 0);
	}
#endif
}

//topic=sys,lib,exe,nayax,sksgn
#define spllogsys(__level__, __fmt__, ...)					spllogtopic(__level__, 0, __fmt__, ##__VA_ARGS__);
#define splloglib(__level__, __fmt__, ...)					spllogtopic(__level__, 1, __fmt__, ##__VA_ARGS__);
#define spllogexe(__level__, __fmt__, ...)					spllogtopic(__level__, 2, __fmt__, ##__VA_ARGS__);
#define spllognaxyax(__level__, __fmt__, ...)				spllogtopic(__level__, 3, __fmt__, ##__VA_ARGS__);
#define spllogsksgn(__level__, __fmt__, ...)				spllogtopic(__level__, 4, __fmt__, ##__VA_ARGS__);

#ifndef UNIX_LINUX
	DWORD WINAPI win32_thread_routine(LPVOID lpParam) {
#else
	void* posix_thread_routine(void* lpParam) {
#endif // !UNIX_LINUX
	int k = 0;
	int tpic = 0;
	while (1) {
		k = get_off_process();
		if (k) {
			break;
		}
		int count = 0;
		while(count < loop_count) {
			spllog(SPL_LOG_INFO, "test log: %llu", (LLU)time(0));
			tpic = (spl_milli_now() % 3);
			spllogsys(SPL_LOG_INFO, "test log: %llu, topic: %d.", (LLU)time(0), tpic);
			splloglib(SPL_LOG_INFO, "test log: %llu, topic: %d.", (LLU)time(0), tpic);
			spllogexe(SPL_LOG_INFO, "test log: %llu, topic: %d.", (LLU)time(0), tpic);
			spllognaxyax(SPL_LOG_INFO, "test log: %llu, topic: %d.", (LLU)time(0), tpic);
			spllogsksgn(SPL_LOG_INFO, "test log: %llu, topic: %d.", (LLU)time(0), tpic);
			//spl_sleep(1);
			count++;
		}
		if (is_master || 1) {
			spl_sleep(1000 * 3);
		}
	}
	return 0;
}
