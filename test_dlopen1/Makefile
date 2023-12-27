
SRC = main.cc

CFLAGS = -g -ggdb -std=c++11
LFLAGS = -ldl
CXX = g++
BIN = test_dlopen


run: so debug
	./${BIN}

debug:
	${CXX} ${SRC} -o ${BIN} ${LFLAGS} ${CFLAGS}


so: func.cc
	${CXX} -shared -fPIC $< -o libfunc.so

# so: func.c
# 	gcc -shared -fPIC $< -o libfunc.so

clean:
	rm -rf ${BIN} *.so
