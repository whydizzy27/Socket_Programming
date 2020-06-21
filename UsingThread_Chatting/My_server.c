

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include <sys/types.h>
#include <netinet/in.h>
#include <pthread.h> //thread library

struct client_information {  //client 
	int socknum;  //socket number
	char ip[INET_ADDRSTRLEN]; //ip array(char)
};

//used function
void sendmessagetoclients(char *message,int currentvalue);
void *recv_message(void *sock);

int n = 0;
int clnts[100]; //size 100 client array

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; //pthread mutual exclusion & initialize



int main(int argc,char *argv[])
{

	int mysock,yoursock; 
	socklen_t your_addr_size;
	int portnum;
	
	struct sockaddr_in my_addr,your_addr;
	pthread_t sendt,recvt; //pthread_t : store thread ID

	int len; //length

	struct client_information ci; //struct variable ci made
	char message[500];
	char ip[INET_ADDRSTRLEN];;
	;

	if(argc > 2) {
		printf("arguments over!!");
		exit(1);
	}

	portnum = atoi(argv[1]);
	mysock = socket(AF_INET,SOCK_STREAM,0);
	memset(my_addr.sin_zero,'\0',sizeof(my_addr.sin_zero));
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(portnum);
	my_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); 

	your_addr_size = sizeof(your_addr);

	//bind
	if(bind(mysock,(struct sockaddr *)&my_addr,sizeof(my_addr)) != 0) {
		perror("bind() error"); //perror: output to strerror
		exit(1);
	}
	//listen
	if(listen(mysock,5) != 0) {
		perror("listen() error");
		exit(1);
	}
	
	while(1) {
		//accept
		if((yoursock = accept(mysock,(struct sockaddr *)&your_addr,&your_addr_size)) < 0) {
			perror("accept() error");
			exit(1);
		}

		pthread_mutex_lock(&mutex); //request mutual exclusion lock for entry to critical section
		inet_ntop(AF_INET, (struct sockaddr *)&your_addr, ip, INET_ADDRSTRLEN);
		printf("Server Notice : %s connected\n",ip); //ip connect

		//connection(server & n개의 clients)
		ci.socknum = yoursock;
		strcpy(ci.ip,ip); //ci : client_information structure's variable I made
		clnts[n] = yoursock;
		n++; //Index up
		pthread_create(&recvt,NULL,recv_message,&ci); //create thread
		pthread_mutex_unlock(&mutex); //give back mutual lock
	}
	
	return 0;
}

//메시지 수신 함수
void *recv_message(void *sock)
{

	struct client_information ci = *((struct client_information *)sock); //client_information structure variable 선언
	
	char message[500];

	int len,i,j;

	while((len = recv(ci.socknum,message,500,0)) > 0) {
		message[len] = '\0'; //\0 : Null과 비슷
		sendmessagetoclients(message,ci.socknum);
		fputs(message,stdout);
		memset(message,'\0',sizeof(message));
	}
	pthread_mutex_lock(&mutex);
	printf("Server Notice : %s disconnected\n",ci.ip);
	for(i = 0; i < n; i++) {
		if(clnts[i] == ci.socknum) {
			j = i;
			while(j < n-1) {
				clnts[j] = clnts[j+1];
				j++;
			}
		}
	}
	n--;
	pthread_mutex_unlock(&mutex);
}




//받은 메시지를 서버에 접속중인 클라이언트들에게 전송하는 함수
void sendmessagetoclients(char *message,int currentvalue) 
{
	int i;

	pthread_mutex_lock(&mutex);
	
	for(i = 0; i < n; i++) {

		if(clnts[i] != currentvalue) {
	
			if(send(clnts[i],message,strlen(message),0) < 0) {

				perror("send fail");
				continue;
			}
		}
	}

	pthread_mutex_unlock(&mutex);
}

