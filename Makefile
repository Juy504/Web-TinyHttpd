PWD=$(shell pwd)
CGI_PATH=$(PWD)/cgi-bin
SER_BIN=httpd
CLI_BIN=client
SER_SRC=httpd.c
CLI_SRC=client.c
INCLUDE=.
CC=gcc
FLAGS=-o
LDFLAGS=-lpthread#-static

.PHONY:all
all:$(SER_BIN) $(CLI_BIN) cgi 
.PHONY:cgi
cgi:
	for name in `echo $(CGI_PATH)`;\
	do\
			cd $$name;\
			make;\
			cd -;\
	done

$(SER_BIN):$(SER_SRC)
	$(CC) $(FLAGS) $@ $^ $(LDFLAGS)
$(CLI_BIN):$(CLI_SRC)
	$(CC) $(FLAGS) $@ $^ $(LDFLAGS)
.PHONY:output
output:all
	mkdir -p output/htdocs/cgi-bin
	cp httpd output
	cp -rf htdocs/* output/htdocs
	for name in `echo $(CGI_PATH)`;\
	do\
		cd $$name;\
		make output;\
		cd -;\
	done
.PHONY:clean
clean:
	rm -rf $(SER_BIN) $(CLI_BIN) output
	for name in `echo $(CGI_PATH)`;\
	do\
			cd $$name;\
			make clean;\
			cd -;\
	done


