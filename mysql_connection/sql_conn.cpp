#include "sql_conn.h"

sql_connecter::sql_connecter(const string &_host,const string &_user,const string &_passwd,const string &_db)
{
	this->mysql_base = mysql_init(NULL);
	this->res  = NULL;
	this->host = _host;
	this->user = _user;
	this->passwd = _passwd;
	this->db   = _db;
}

bool sql_connecter::begin_connect()
{
	if(mysql_real_connect(mysql_base,host.c_str(),user.c_str(),passwd.c_str(),db.c_str(),3306,NULL,0) == NULL)
	{
		cout<<"error"<<endl;
		return false;
	}else{
		cout<<"right"<<endl;
		cout<<"host.c_str : "<<host.c_str()<<'\t';
		cout<<"db.c_str : "<<db.c_str()<<'\t';
		cout<<endl;
	}
	return true;
}
bool sql_connecter::select_sql(string field_name[],string _out_data[][4],int& _out_row)
{
	string sql = "SELECT * FROM Stu_Info";
	if(mysql_query(mysql_base,sql.c_str()) == 0)
	{
		cout<<"query connect  success"<<endl;
	}
	else
	{
		cout<<"query connect  failed"<<endl;
	}
	res = mysql_store_result(mysql_base);
	int row_num = mysql_num_rows(res);
	int field_num = mysql_num_fields(res);
	_out_row = row_num;

	MYSQL_FIELD *fd = NULL;
	int i = 0;
	for(;fd = mysql_fetch_field(res);)
	{
		cout<<fd->name<<"\t";
		field_name[i++] = fd->name;
	}
	cout<<endl;

	for(int index=0; index<row_num; index++)
	{
		MYSQL_ROW _row = mysql_fetch_row(res);
		if(_row)
		{
			int start = 0;
			for(; start < field_num ; start++)
			{
				_out_data[index][start] = _row[start];
				cout<<_row[start]<<"\t";
			}
			cout<<endl;
		}
	}
	return true;
}
bool sql_connecter::insert_sql(const string &data)
{
	string sql = "INSERT INTO Stu_Info (name,age,school) values ";
	sql += "(";
	sql += data;
	sql += ")";
	if(mysql_query(mysql_base,sql.c_str()) == 0)
	{
		cout<<"insert query success!\n"<<endl;
		return true;
	}else
	{
		cout<<"insert query failed!\n"<<endl;
		return false;
	}
}
bool sql_connecter::close_connect()
{
	mysql_close(mysql_base);
	cout<<"connect closed\n"<<endl;
}
sql_connecter::~sql_connecter()
{
	close_connect();
	if(res != NULL)
	{
		free(res);
	}
}
#ifdef _DEBUG_
int main()
{
	string _sql_data[1024][4];
	string header[4];
	int curr_row = -1;
	const string _host = "192.168.15.129";
	const string _user = "jy";
	const string _passwd = "123";
	const string _db = "test";
	const string _data = "'lili',22,'wenli'";
	cout<<_data<<endl;
	sql_connecter conn(_host,_user,_passwd,_db);
	conn.begin_connect();
	conn.insert_sql(_data);
	conn.select_sql(header,_sql_data,curr_row);
	return 0;
}
#endif
