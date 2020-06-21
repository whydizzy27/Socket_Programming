

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include <sys/types.h>
#include <netinet/in.h>
#include <pthread.h> //thread library

void *recv_message(void *sock);

char username[100];//world variable

int main(int argc, char *argv[])
{
	struct sockaddr_in your_addr;

	int mysock, yoursock;
	int your_addr_size;
	
	char message[500];
	int portnum;
	int len;

	pthread_t sendt,recvt;

	char resource[600];
	char ip[INET_ADDRSTRLEN];
	

	if(argc > 3) {
		printf("arguments over");
		exit(1);
	}

	portnum = atoi(argv[2]);
	strcpy(username,argv[1]);
	mysock = socket(AF_INET,SOCK_STREAM,0);
	memset(your_addr.sin_zero,'\0',sizeof(your_addr.sin_zero));
	your_addr.sin_family = AF_INET;
	your_addr.sin_port = htons(portnum);
	your_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	if(connect(mysock,(struct sockaddr *)&your_addr,sizeof(your_addr)) < 0) {
		perror("connection error");
		exit(1);
	}

	inet_ntop(AF_INET, (struct sockaddr *)&your_addr, ip, INET_ADDRSTRLEN);

	printf("------------------------------------------------------------\n"); 
	printf("Client Notice : connected to %s \n\nJoin in Chatting room made by YDG!!\n",ip); //enter chatting room
	printf("------------------------------------------------------------\n\n"); 
	printf("////////////////////////\n"); 
	printf("my ID : %s \n", username);
	printf("////////////////////////\n\n"); 
	printf("me(%s) : \n",username);

	pthread_create(&recvt,NULL,recv_message,&mysock);


	while(fgets(message,500,stdin) > 0) { 
		printf("\nme(%s) : \n",username);
		strcpy(resource,username); //ex) pch:ok destination side
		strcat(resource," : ");//strcat : string connect only two arguments
		strcat(resource,message);
		
		len = write(mysock,resource,strlen(resource));
		
		if(len < 0) {
			perror("send fail");
			exit(1);
		}
		
		memset(message,'\0',sizeof(message));
		memset(resource,'\0',sizeof(resource));

	}

	pthread_join(recvt,NULL);

	close(mysock);

}


void *recv_message(void *sock)
{
	int yoursock = *((int *)sock);

	char message[500];
	int len;

	while((len = recv(yoursock,message,500,0)) > 0) { //message receive loop. source side

		message[len] = '\0';

		printf("# Receiving another people's message...\n\n");

		fputs(message,stdout);

		printf("\nme(%s) : \n",username);
		memset(message,'\0',sizeof(message));
	}
}
