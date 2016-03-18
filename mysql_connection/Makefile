WORK_PATH=$(shell pwd)
BIN=sql_connect
CC=g++
INCLUDE= -I${WORK_PATH}/mysql/include -I .
LIB_PATH= -L${WORK_PATH}/mysql/lib/
SRC=sql_conn.cpp

LIB_NAME= -l mysqlclient

${BIN}:${SRC}
	${CC} -o $@ $^ ${INCLUDE} ${LIB_PATH} ${LIB_NAME}  -lrt -ldl  -g -D_DEBUG_

.PHONY:clean
clean:
	rm -f ${BIN}
