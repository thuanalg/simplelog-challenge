target=libsimplelog-challenge.so
debugFlag=-g -fPIC -Wall -shared 
releaseFlag=-fPIC -Wall -shared
LIBS+=-lpthread
src=../simplelog-challenge.c
INC=-I../ -I../../include
SRCTEST_PREFIX=../../tests
#CFLAGS=-ansi -D UNIX_LINUX
CFLAGS=-std=gnu89 -D UNIX_LINUX
#CPPFLAGS=-std=c++98 
#CPPFLAGS=-std=c++98 -D UNIX_LINUX -D __UNIX_LINUX_CPP11_AND_NEWERS
CPPFLAGS=-D UNIX_LINUX -D __UNIX_LINUX_CPP11_AND_NEWERS__

debug:
	gcc ${debugFlag} -o ${target} ${src} ${LIBS} ${CFLAGS} ${INC}
	gcc -g -o simple_c_example  ${SRCTEST_PREFIX}/c_test/main.c ${src} ${LIBS} ${INC} ${CFLAGS}
	gcc -g -o simple_c_fork  ${SRCTEST_PREFIX}/fork/main.c ${src} ${LIBS} ${INC} ${CFLAGS}
	g++ -g -o simple_cpp_example  ${SRCTEST_PREFIX}/cpp_test/main.cpp ${LIBS} ${CPPFLAGS} ${INC} -L./ -lsimplelog-challenge
	g++ -g -o simple_cpp_example_direct  ${SRCTEST_PREFIX}/cpp_test/main.cpp ${src} ${LIBS} ${INC} ${CPPFLAGS}

release:
	gcc ${releaseFlag} -o ${target} ${src} ${LIBS} ${CFLAGS} ${INC}
	gcc -o simple_c_example  ${SRCTEST_PREFIX}/c_test/main.c ${src} ${LIBS} ${INC} ${CFLAGS}
	gcc -o simple_c_fork  ${SRCTEST_PREFIX}/fork/main.c ${src} ${LIBS} ${INC} ${CFLAGS}
	g++ -o simple_cpp_example  ${SRCTEST_PREFIX}/cpp_test/main.cpp ${LIBS} ${INC} ${CPPFLAGS} -L./ -lsimplelog-challenge
	g++ -o simple_cpp_example_direct  ${SRCTEST_PREFIX}/cpp_test/main.cpp ${src} ${LIBS} ${INC} ${CPPFLAGS}

clean:
	rm -rf ${target} *.so* simple_*
