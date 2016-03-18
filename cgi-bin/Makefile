FIND_PATH =-I/home/jy/Myhttpd/mysql_connection -I/home/jy/Myhttpd/mysql_connection/mysql/include -L/home/jy/Myhttpd/mysql_connection/mysql/lib -lmysqlclient 

.PHONY:all
all:debug_cgi show_data_cgi register_cgi
debug:debug_cgi.c
	gcc -o $@ $^
show_data_cgi:show_data_cgi.cpp  /home/jy/Myhttpd/mysql_connection/sql_conn.cpp
	g++ -o $@ $^ ${FIND_PATH}
register_cgi:register_cgi.cpp  /home/jy/Myhttpd/mysql_connection/sql_conn.cpp
	g++ -o $@ $^ ${FIND_PATH}
.PHONY:output
output:
	cp debug_cgi ../output/htdocs/cgi-bin
	cp show_data_cgi ../output/htdocs/cgi-bin
	cp register_cgi ../output/htdocs/cgi-bin
.PHONY:clean
clean:
	rm -rf debug_cgi  show_data_cgi register_cgi
