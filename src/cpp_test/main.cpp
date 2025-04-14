#include "simplelog-challenge.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifndef UNIX_LINUX

#include <Windows.h>
DWORD WINAPI
win32_thread_routine(LPVOID lpParam);
#else
#include <unistd.h>
#include <pthread.h>
void *
posix_thread_routine(void *lpParam);
#endif // !UNIX_LINUX

void
dotest();
int num_threads = 10;
int loop_count = 1000 * 1000;
int ismaster = 0;
int topicindex = 0;

#define TNUMBEER_OF_THREADS "--nthread="
#define TCONFIG_FILE "--cfg="
#define TLOOP_COUNT "--loopcount="
#define TMASTER_MODE "--is_master="
#define TTOPIC_INDEX "--topic_index="

void
dotest()
{
	int i = 0;
#ifndef UNIX_LINUX

	DWORD *dwpThreadId = 0, dwEvent = 0;
	HANDLE *hpThread = 0;

	dwpThreadId = (DWORD *)malloc(num_threads * sizeof(DWORD));
	if (!dwpThreadId) {
		exit(1);
	}
	memset(dwpThreadId, 0, num_threads * sizeof(DWORD));

	hpThread = (HANDLE *)malloc(num_threads * sizeof(HANDLE));
	if (!hpThread) {
		exit(1);
	}
	memset(hpThread, 0, num_threads * sizeof(HANDLE));

	for (i = 0; i < num_threads; ++i) {
		hpThread[i] = CreateThread(NULL, 0, win32_thread_routine, 0, 0, (dwpThreadId + i));
	}
	// https://learn.microsoft.com/en-us/windows/win32/api/synchapi/nf-synchapi-waitformultipleobjects
	// https://learn.microsoft.com/en-us/windows/win32/sync/waiting-for-multiple-objects
	dwEvent = WaitForMultipleObjects(num_threads, // number of objects in array
	    hpThread, // array of objects
	    TRUE, // wait for any object
	    INFINITE); // five-second wait
	free(dwpThreadId);
	free(hpThread);
#else
	// https://man7.org/linux/man-pages/man3/pthread_create.3.html
	pthread_t *pidds = 0;
	pidds = (pthread_t *)malloc(num_threads * sizeof(pthread_t));
	if (!pidds) {
		exit(1);
	}
	for (i = 0; i < num_threads; ++i) {
		int err = pthread_create(pidds + i, 0, posix_thread_routine, 0);
	}
	for (int i = 0; i < num_threads; i++) {
		int s = pthread_join(pidds[i], 0);
		if (s != 0) {
			spc_console_log("pthread_join error.\n");
		}
	}
	free(pidds);

#endif
}

// topic=sys,lib,exe,nayax,sksgn
#define spllogsys(__level__, __fmt__, ...) spllogtopic(__level__, 0, __fmt__, ##__VA_ARGS__);
#define splloglib(__level__, __fmt__, ...) spllogtopic(__level__, 1, __fmt__, ##__VA_ARGS__);
#define spllogexe(__level__, __fmt__, ...) spllogtopic(__level__, 2, __fmt__, ##__VA_ARGS__);
#define spllognaxyax(__level__, __fmt__, ...) spllogtopic(__level__, 3, __fmt__, ##__VA_ARGS__);
#define spllogsksgn(__level__, __fmt__, ...) spllogtopic(__level__, 4, __fmt__, ##__VA_ARGS__);
// https://github.com/gabime/spdlog, 10 thread
#ifndef UNIX_LINUX
DWORD WINAPI
win32_thread_routine(LPVOID lpParam)
{
#else
void *
posix_thread_routine(void *lpParam)
{
#endif // !UNIX_LINUX
	int count = 0;
	/*
	char aa[10000];
	memset(aa, 'A', sizeof(aa));
	aa[sizeof(aa) - 1] = 0;
	*/
	while (count < loop_count) {
/*You can mix any topic togther. No problem.*/
#define TEXT_PERFORMMANCE_TEXT "Log:%d"
		if (topicindex < 1) {
			spllog(SPC_LOG_INFO, TEXT_PERFORMMANCE_TEXT, count);
			/*spllog(SPC_LOG_INFO, TEXT_PERFORMMANCE_TEXT"\t%s", count, aa);*/
		} else {
			spllogtopic(SPC_LOG_INFO, topicindex - 1, TEXT_PERFORMMANCE_TEXT, count);
			/*spllogtopic(SPC_LOG_INFO, topicindex - 1, TEXT_PERFORMMANCE_TEXT"\t%s", count, aa); */
		}
		++count;
	}
	return 0;
}

int
main(int argc, char *argv[])
{
	int ret = 0, i = 0;
	SPC_INPUT_ARG input;
	int count = 2;
	for (i = 1; i < argc; ++i) {
		if (strstr(argv[i], TNUMBEER_OF_THREADS) == argv[i]) {
			ret = sscanf(argv[i], TNUMBEER_OF_THREADS "%d", &num_threads);
			continue;
		}
		if (strstr(argv[i], TLOOP_COUNT) == argv[i]) {
			ret = sscanf(argv[i], TLOOP_COUNT "%d", &loop_count);
			continue;
		}
		if (strstr(argv[i], TMASTER_MODE) == argv[i]) {
			ret = sscanf(argv[i], TMASTER_MODE "%d", &ismaster);
			continue;
		}
		if (strstr(argv[i], TTOPIC_INDEX) == argv[i]) {
			ret = sscanf(argv[i], TTOPIC_INDEX "%d", &topicindex);
			continue;
		}
	}
	memset(&input, 0, sizeof(input));
	snprintf(input.id_name, SPC_IDD_NAME, "testlog");
	// int ret = spc_init_log((char *)"C:/z/simplelog-topic/win64/Debug/simplelog.cfg");
#ifndef UNIX_LINUX
	// ret = spc_init_log((char*)"C:/z/simplelog-topic/win64/Debug/simplelog.cfg");
	snprintf(input.folder, SPC_PATH_FOLDER, "C:/z/simplelog-challenge/win64/Debug/simplelog.cfg");
#else
	// ret = spc_init_log((char*)"simplelog.cfg");
	snprintf(input.folder, SPC_PATH_FOLDER, "simplelog.cfg");
#endif
	input.is_master = ismaster ? 1 : 0;
	ret = spc_init_log_ext(&input);

	if (!ismaster) {
		spc_console_log("====================Start.\n");
		dotest();
		spc_console_log("==================End.\n");
	} else {
		FILE *fp = 0;
		while (1) {
			spc_console_log("this is master process.");
			spllog(SPC_LOG_INFO, "this is master process.");
			spc_sleep(5);
#ifndef UNIX_LINUX
			fp = fopen("trigger_master.txt", "r");
#else
			fp = fopen("trigger_master.txt", "r");
#endif
			if (fp) {
				break;
			}
		}
		if (fp) {
			fclose(fp);
		}
	}
	spc_finish_log();
	return 0;
}
