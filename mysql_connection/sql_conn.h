#ifndef __SQL_CONN__
#define __SQL_CONN__

#include <iostream>
#include <string>
#include <stdlib.h>
#include "mysql.h"
using namespace std;

class sql_connecter{
	public:
			sql_connecter(const string &_host,const string &_user,const string &_passwd,const string &_db);
			bool begin_connect();
			bool insert_sql(const string &data);
			bool select_sql(string field_name[],string _out_data[][4],int& _out_row );			
			bool close_connect();
			~sql_connecter();
	private:
			MYSQL_RES *res;
			MYSQL *mysql_base;
			string host;
			string user;
			string passwd;
			string db;
};
#endif
