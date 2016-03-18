#ifndef __HTTPD_H__
#define __HTTPD_H__

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
//#include <strings.h>
#include <errno.h>
#include <arpa/inet.h>
#include <pthread.h>
//#include <ctype.h>
#include <sys/sendfile.h>
//#include <sys/wait.h>
#include <unistd.h>
int get_line(int sock,char* buf, int max_len);

#define _BACKLOG_ 5
#define _COMM_SIZE 1024
#define MAIN_PAGE "index.html"
#define HTTP_VERSION "HTTP/1.0"
#endif
