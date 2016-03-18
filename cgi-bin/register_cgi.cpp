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

int main(int arg,char*argv[])
{

	char post_data[1024];
	char query_string[1024];
	memset(post_data,'\0',sizeof(post_data));
	memset(query_string,'\0',sizeof(query_string));
	char* str = NULL;
	str =  getenv("QUERY_STRING");
	strcpy(query_string,getenv("QUERY_STRING"));
	printf("query_string : >%s\n",query_string);
	int j = 0;
	while(j<strlen(query_string))
	{
		post_data[j] = query_string[j];
		j++;
	}
	post_data[j] = '\0';
	cout<<"post_data : >"<<post_data<<endl;
	cout<<"<html>"<<endl;
	cout<<"<head> show message</head>"<<endl;
	cout<<"<body>"<<endl;
	int i = 0;
	while(post_data[i] != '\0')
	{
		if(post_data[i] == '=' || post_data[i] == '&')
		{
			post_data[i] = ' ';
		}
		i++;
	}
	cout<<"post_data : >"<<post_data<<endl;
	char name[30];
	char age[30];
	char school[30];
	memset(name,'\0',sizeof(name));
	memset(age,'\0',sizeof(age));
	memset(school,'\0',sizeof(school));
	sscanf(post_data,"%*s %s %*s %s %*s %s",name,age,school);
	cout<<"name >"<<name<<endl;
	cout<<"age >"<<age<<endl;
	cout<<"school >"<<school<<endl;
	string _data = "'";
	_data += name;
	_data += "',";
	_data += age;
	_data += ",'";
	_data += school;
	_data += "'";
	
	cout<<"_data > "<<_data<<endl;
	const string _host = ip;
	const string _user = user;
	const string _passwd = passwd;
	const string _db = db;

	sql_connecter conn(_host,_user,_passwd,_db);
	conn.begin_connect();
	conn.insert_sql(_data);

	cout<<"</body>"<<endl;
	cout<<"</html>"<<endl;
}
