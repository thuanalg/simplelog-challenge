#Refer to source code: "fork/main.c"
1. Come to "src/linux"
2. make clean; make
3. ./xmaster_run.sh
#Fork 1 time, then there is one parent ( 10 threads), one child (10 threads).  Total: 20 million records.
4. ./xfork 
5. ./xstop

#[2025-04-15 13:59:59.750622821] [main.c:main:196] [thid: 140252787935040] ====================Start.

#[2025-04-15 13:59:59.750691109] [main.c:main:196] [thid: 140252787935040] ====================Start.

#[2025-04-15 14:00:04.904682117] [main.c:main:198] [thid: 140252787935040] ==================End.

#[2025-04-15 14:00:04.905058491] [main.c:main:198] [thid: 140252787935040] ==================End.

Speed: 3.879728 million records per second
Size of file: (2000200036 + 357589480) bytes

       1 
       2 [2025-04-15 13:59:54.213305515] [I] [tid        140043502073664]        [pid     21182] [main.cpp:main:166] this is master process.
       3 [2025-04-15 13:59:59.213486155] [I] [tid        140043502073664]        [pid     21182] [main.cpp:main:166] this is master process.
       4 [2025-04-15 13:59:59.750725175] [I] [tid        140252781094464]        [pid     21187] [main.c:posix_thread_routine:148] Log: 3
       5 [2025-04-15 13:59:59.750726285] [I] [tid        140252781094464]        [pid     21187] [main.c:posix_thread_routine:148] Log: 4
       6 [2025-04-15 13:59:59.750751755] [I] [tid        140252781094464]        [pid     21187] [main.c:posix_thread_routine:148] Log: 23
       7 [2025-04-15 13:59:59.750762510] [I] [tid        140252781094464]        [pid     21187] [main.c:posix_thread_routine:148] Log: 33

3030323 [2025-04-15 14:00:04.904978019] [I] [tid        140252781094464]        [pid     21188] [main.c:posix_thread_routine:148] Log: 999990
3030324 [2025-04-15 14:00:04.904982264] [I] [tid        140252781094464]        [pid     21188] [main.c:posix_thread_routine:148] Log: 999996
3030325 [2025-04-15 14:00:09.213771204] [I] [tid        140043502073664]        [pid     21182] [main.cpp:main:166] this is master process.
3030326 [2025-04-15 14:00:14.213956571] [I] [tid        140043502073664]        [pid     21182] [main.cpp:main:166] this is master process.
