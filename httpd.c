#include "httpd.h"

void usage(char* proc)
{
	printf("usage : %s, [ip][port]\n",proc);
}

void not_found(int client)
{
	char buf[1000];
	sprintf(buf,"HTTP/1.0 404 NOT FOUND\r\n");
	send(client,buf,strlen(buf),0);
	sprintf(buf,"Content-type : text\r\n");
	send(client,buf,strlen(buf),0);
	sprintf(buf,"\r\n");
	send(client,buf,strlen(buf),0);
	sprintf(buf,"<html></br><p> Information Not Found\r\n");
	send(client,buf,strlen(buf),0);
	return;
}
void print_log(const char* fun, int line,int err_no,const char* err_str)
{
	printf("[%s : %d] [%d][%s]\n",fun,line,err_no,err_str);
}
void echo_error_to_client(int client,int err_code)
{
	switch(err_code)
	{
		case 404:
			not_found(client);
			break;
		 default:
			break;
	}
}

void clear_header(int client)
{
	char buf[1024];
	memset(buf,'\0',sizeof(buf));
	int ret = 0;
	do{
		ret = get_line(client,buf,sizeof(buf));//http head write to socket
	}while(ret>0 && strcmp(buf,"\n")!=0);
}

void echo_html(int client,const char* path, unsigned int file_size)
{
	printf("enter html\n");
	if(!path)
		return;
	int fd = open(path,O_RDONLY);
	if(fd < 0)
	{
		echo_error_to_client(client,404);
		return;
	}
	char echo_line[1024];
	memset(echo_line,'\0',sizeof(echo_line));
	strcat(echo_line,"HTTP/1.0 200 OK\r\n\r\n");
	
	send(client,echo_line,strlen(echo_line),0);
	send(client,"\r\n",strlen("\r\n"),0);
	if(sendfile(client,fd,NULL,file_size) < 0) //means read() write()
	{
		echo_error_to_client(client,504);
		close(fd);
		return;
	}
	printf("sendfile success\n");
	close(fd);
}
int get_line(int sock,char* buf, int max_len)
{
	if(!buf || max_len < 0)
	{
		return -1;
	}
	int i = 0;
	int n = 0;
	char c = '\0';
	while(i<max_len-1 && c !='\n')
	{
		n = recv(sock,&c,1,0);
		if( n > 0)//success
		{
			if(c == '\r')
			{
				n = recv(sock,&c,1,MSG_PEEK);//MSG_PEEK receive '\r'each time
				if( n>0 && c == '\n')//'\r\n'
				{
					recv(sock,&c,1,0);//recv '\n'	
				}
				else{
					c = '\n';
				}
			}
			printf("%c ",c);
			buf[i++] = c;
		}else{
			c = '\n';
		}
	}
	buf[i] = '\0';
	return i;
}
void exe_cgi(int sock_client,const char* path,const char* method,const char*query_string)
{
	char buf[_COMM_SIZE];
	int numchars = 0;
	int content_length = -1;
	
	int cgi_input[2] = {0,0};
	int cgi_output[2] = {0,0};
	
	pid_t id;
	if(strcasecmp(method,"GET") == 0)
	{
		clear_header(sock_client);//???
	}else{//POST   
		do{
			memset(buf,'\0',sizeof(buf));
			numchars = get_line(sock_client,buf,sizeof(buf));
			printf("exe_cgI:  num%d\n",numchars);
			if(strncasecmp(buf,"Content-Length:",strlen("Content-Length:")) == 0)
			{

				printf("exr_:> %s\n",buf);

				content_length = atoi(&buf[16]);//
				printf("exe->:   %d\n",content_length);
			}
		}while(numchars > 0 && strcmp(buf,"\n") != 0);
		if(content_length == -1)
		{
			echo_error_to_client(sock_client,404);
			return;
		}
	}
	memset(buf,'\0',sizeof(buf));
	strcpy(buf,HTTP_VERSION);
	strcat(buf," 200 OK\r\n\r\n");
	send(sock_client,buf,strlen(buf),0);
	if(pipe(cgi_input) == -1)
	{
	//	close(cgi_output[0]);
	//	close(cgi_output[1]);
		echo_error_to_client(sock_client,504);
		return;
	}
	printf("exe_cgi post\n");
	if(pipe(cgi_output) == -1)
	{
		close(cgi_input[0]);
		close(cgi_input[1]);
		echo_error_to_client(sock_client,504);
		return;
	}
	else
	{
		printf("pipe success\n");
	}
	if((id = fork()) < 0)
	{
		printf("fork error\n");
		close(cgi_input[0]);
		close(cgi_input[1]);
		close(cgi_output[0]);
		close(cgi_output[1]);
		echo_error_to_client(sock_client,504);
		return;
	}
	else if(id == 0)//child
	{
		printf("this is child\n");
		char method_env[_COMM_SIZE];
		char query_env[_COMM_SIZE/10];
		char content_len_env[_COMM_SIZE];
		memset(method_env,'\0',sizeof(method_env));
		memset(query_env,'\0',sizeof(query_env));
		memset(content_len_env,'\0',sizeof(content_len_env));
		close(cgi_input[1]);
		close(cgi_output[0]);

		printf("exe_cgi post\n");
		dup2(cgi_input[0],0); //STDIN to cgi_input 
		dup2(cgi_output[1],1); //STDOUT to cgi_output
		sprintf(method_env,"REQUEST_METHOD=%s",method);
		printf("REQUEST_METHOD_ENV :> %s\n",method_env);
		putenv(method_env); //add method_env  to environment variable
		
		if(strcasecmp("GET",method) == 0)
		{
			sprintf(query_env,"QUERY_STRING=%s",query_string);
			putenv(query_env);
			printf("GET,,	QUSTRING : >%s\n",query_env);
		}else{//POST

			printf("exe_cgi post\n");
			sprintf(content_len_env,"CONTENT_LENGTH=%d",content_length);
			printf("POST,, CONTENT_LENGTH:>%s\n",content_len_env);
			putenv(content_len_env);
		}
		execl(path,path,NULL);
		exit(1);
	}
	else{//father
		printf("this is father\n");
		close(cgi_input[0]);
		close(cgi_output[1]);

		int i = 0;
		char c = '\0';
		if(strcasecmp("POST",method) == 0)
		{
			printf("enter father post\n");
			for(;i<content_length;i++)
			{
				recv(sock_client,&c,1,0); //select pool
				write(cgi_input[1],&c,1);
			}
		}
		while(read(cgi_output[0],&c,1) > 0)
		{
			send(sock_client,&c,1,0);
		}
		close(cgi_input[1]);
		close(cgi_output[0]);
		int ret = waitpid(id,NULL,0);
		if(ret == id)
		{
			printf("waitpid success!\n");
		}
		else
		{
			printf("waitpid failed\n");
		}
		printf("after waitpid ... id :> %d\n",getpid());
	}
}
void *accept_request(void *arg)
{
	
	pthread_detach(pthread_self());//detach self with main thread

	int sock_client = (int)arg;//???
//	echo_error_to_client(sock_client,404);
	int cgi = 0;
	char method[_COMM_SIZE];
	char *query_string = NULL;
	char url[_COMM_SIZE];
	char path[_COMM_SIZE];
	char buffer[_COMM_SIZE];
	int i = 0;int j = 0;
	memset(method,'\0',sizeof(method));
	memset(url,'\0',sizeof(url));
	memset(path,'\0',sizeof(path));
	memset(buffer,'\0',sizeof(buffer));
	if(get_line(sock_client,buffer,sizeof(buffer))<0)
	{
		echo_error_to_client(sock_client,502);
		return (void*)-1;
	}
	while(!isspace(buffer[j]) && i<sizeof(method)-1 && j<sizeof(buffer))
	{
		method[i] = buffer[j];
		i++,j++;
	}
	while(isspace(buffer[j]) && j<sizeof(buffer))
	{
		j++;//clear isspace
	}
	i = 0;
	while(!isspace(buffer[j]) && i<sizeof(url)-1 && j<sizeof(buffer))
	{
		url[i] = buffer[j];
		i++,j++;
	}
	if(strcasecmp(method,"GET") && strcasecmp(method,"POST"))
	{
		echo_error_to_client(sock_client,502);
		return NULL;
	}
	if(strcasecmp(method,"POST") == 0)
	{
		cgi = 1;//??????
		printf("method is post\n");
	}

	if(strcasecmp(method,"GET") == 0)
	{
		query_string = url;
		while(*query_string != '?' && *query_string != '\0')
		{
			query_string++;
		}
		if(*query_string == '?')
		{
			*query_string = '\0';
			query_string++;
			cgi = 1;//?????
		}
		printf("get url is :%s\n",url);
	}
	else
	{
		echo_error_to_client(sock_client,404);
	}
	printf("url is :%s\n",url);
	printf("query_string : %s\n",query_string);
	sprintf(path,"htdocs%s",url);
	if(path[strlen(path)-1] == '/')
	{
		strcat(path,MAIN_PAGE);
	}
	struct stat st;
	if(stat(path,&st) < 0)//file not exist
	{
		clear_header(sock_client);
		echo_error_to_client(sock_client,404);
	}else{//file exist
		if(S_ISDIR(st.st_mode))
		{
			strcat(path,"/");
			strcat(path,MAIN_PAGE);
		}else if(st.st_mode & S_IXUSR || st.st_mode & S_IXGRP||\
				 st.st_mode & S_IXOTH)// & ???
		{
			cgi = 1;
		}else{
			printf("%s\n",path);	
		}
		if(cgi) //want to use chart send to server
		{
			printf("exe_cgi before\n");
			exe_cgi(sock_client,path,method,query_string);
		}else   //get main_page message
		{
			clear_header(sock_client);
			printf("echo a connect\n");
			echo_html(sock_client,path,st.st_size);
		}
	}
	close(sock_client);
	return NULL;
}
int start(int port)
{
	int listen_sock = socket(AF_INET,SOCK_STREAM,0);
	if(listen_sock == -1)
	{
		print_log(__FUNCTION__,__LINE__,errno,strerror(errno));
		exit(1);
	}

	int flag = 1;
	setsockopt(listen_sock, SOL_SOCKET, SO_REUSEPORT, &flag, sizeof(flag));

	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_port = htons(port);
	local.sin_addr.s_addr = htonl(INADDR_ANY);
	socklen_t len = sizeof(local);
	if(bind(listen_sock,(const struct sockaddr*)&local,len) == -1)
	{
		print_log(__FUNCTION__,__LINE__,errno,strerror(errno));
		exit(2);
	}
	if(listen(listen_sock,_BACKLOG_) == -1)
	{
		print_log(__FUNCTION__,__LINE__,errno,strerror(errno));
		exit(3);
	}
	return listen_sock;
}
int main(int argc,char *argv[])
{
	if(argc != 2)
	{
		usage(argv[0]);
		exit(1);
	}
	
	int port = atoi(argv[1]);
	int sock = start(port);
	struct sockaddr_in client;
	socklen_t len = 0;
	while(1)
	{
		int new_sock = accept(sock,(struct sockaddr*)&client,&len);
		if(new_sock<0)
		{
			print_log(__FUNCTION__,__LINE__,errno,strerror(errno));
			continue;
		}
		pthread_t new_thread;
		pthread_create(&new_thread,NULL,accept_request,(void*)new_sock); 
	}
	return 0;
}
