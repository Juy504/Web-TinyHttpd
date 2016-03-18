#include <iostream>
#include <stdio.h>
#include <string.h>
#include <string>
#include <stdlib.h>
#include "sql_conn.h"
using namespace std;

const string ip = "192.168.15.129";
const string user = "jy";
const string passwd = "123";
const string db = "test";

int main()
{
	int content_length = -1;
	char method[1024];
	char query_string[1024];
	char post_data[4096];
	memset(method,'\0',sizeof(method));
	memset(query_string,'\0',sizeof(query_string));
	memset(post_data,'\0',sizeof(post_data));
	cout<<"<html>"<<endl;
	cout<<"<head> show message</head>"<<endl;
	cout<<"<body>"<<endl;
	const string _host = ip;
	const string _user = user;
	const string _passwd = passwd;
	const string _db = db;
	cout<<"after defination "<<endl;
	string head[4];
	string _sql_data[1024][4];
	int curr_row = -1;

	cout<<"sql_connecter conn success"<<endl;
	sql_connecter conn(_host,_user,_passwd,_db);
	conn.begin_connect();
	cout<<"begin_connect success"<<endl;
	conn.select_sql(head,_sql_data,curr_row);
	for(int i = 0; i<4;i++)
	{
		cout<<"<th>"<<head[i]<<"</th>"<<endl;
	}
	cout<<"</tr>"<<endl;
	for(int i = 0; i<curr_row; i++)
	{
		cout<<"<tr>"<<endl;
		for(int j = 0; j<4; j++)
		{
			cout<<"<td>"<<_sql_data[i][j]<<"</td>"<<endl;
		}
		cout<<"/tr"<<endl;
	}
	cout<<"</table>"<<endl;

	cout<<"</body>"<<endl;
	cout<<"</html>"<<endl;
}
