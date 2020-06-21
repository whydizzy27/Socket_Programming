#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<time.h>
#define BUF_SIZE 1024

void error_handling(char *message);
int atoi2(char str);

int main(int argc, char *argv[])
{
	int serv_sock, clnt_sock;
	char message[BUF_SIZE];
	int str_len, i;
	int data[4]={0,};
	int k=0;
	int sub_k=0;
	int msgcnt=10;
	int strike =0;
	int ball = 0;
	char score[2]={'0','0'}; 
	
	srand((unsigned int)time(NULL));
	struct sockaddr_in serv_adr;
	struct sockaddr_in clnt_adr;
	socklen_t clnt_adr_sz;

	if(argc != 2){
		printf("Usage: %s<port>\n", argv[0]);
		exit(1);
	}

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if(serv_sock == -1)
		error_handling("socket() error");

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));

	if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
		error_handling("bind() error");
	
	if(listen(serv_sock, 5) == -1)
		error_handling("listen() error");
	
	clnt_adr_sz = sizeof(clnt_adr);
		clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
		if(clnt_sock == -1)
			error_handling("accept() error");
		else
			printf("Connected client %d \n", i+1);

///////////////////////////////////////////////////////////
	
	for(k=0;k<4;k++){
      		data[k]=rand()%10;
      		for(sub_k=0;sub_k<k;sub_k++){
         		if(data[k]==data[sub_k]){
           			k--;
            			break;
        		 }
      		}
   	}
   
   	printf("******************************************************\n");
   	printf("\n");
   	printf("!!!Random Numbers: ");
   	for(k=0;k<4;k++)
      		printf("%d ",data[k]);
   	printf("\n");
	printf("\n");
	printf("******************************************************\n");
   
   	while(1)
   	{   
      		strike=0;
      		ball=0;

      		if((str_len = read(clnt_sock, message, BUF_SIZE)) != 0)
      		{
         		printf("Clnt's choice: %s ", message);
      		}
      
      		if(atoi2(message[0])==data[0]){
         		strike+=1;   
      		}
      		if(atoi2(message[1])==data[1]){
         		strike+=1;
      		}
      		if(atoi2(message[2])==data[2]){
         		strike+=1;
      		}
      		if(atoi2(message[3])==data[3]){
         		strike+=1;
      		}
      		////////////////////////////////////////////////////////
      
      		if(atoi2(message[0])==data[1]){
         		ball+=1;
      		}
      		if(atoi2(message[0])==data[2]){
        		 ball+=1;
      		}
      		if(atoi2(message[0])==data[3]){
         		ball+=1;
      		}
      		////////////////////////////////
      		if(atoi2(message[1])==data[0]){
         		ball+=1;
      		}
      		if(atoi2(message[1])==data[2]){
         		ball+=1;
      		}
      		if(atoi2(message[1])==data[3]){
         		ball+=1;
      		}
      		////////////////////////////////
      		if(atoi2(message[2])==data[0]){
         		ball+=1;
      		}
      		if(atoi2(message[2])==data[1]){
         		ball+=1;
      		}
      		if(atoi2(message[2])==data[3]){
         		ball+=1;
      		}
      		////////////////////////////////
      		if(atoi2(message[3])==data[0]){
         		ball+=1;
      		}
      		if(atoi2(message[3])==data[1]){
         		ball+=1;
      		}
      		if(atoi2(message[3])==data[2]){
         		ball+=1;
     		}
		///////////////////////////////
      		score[1]=(char)strike;
      		score[0]=(char)ball;	

      		write(clnt_sock, score, sizeof(score));

      		if(strike==4){
         		printf("\n");
        	 	printf("C L N T is R I G H T\n");	
         		for(k=0;k<4;k++){
      				data[k]=rand()%10;
      				for(sub_k=0;sub_k<k;sub_k++){
         				if(data[k]==data[sub_k]){
            					k--;
            					break;
         				}
      				}
   			}
		/////////////////////////////////////////////////////////////////////////   
   		printf("******************************************************\n");
   		printf("\n");
   		printf("!!!Next Random Numbers: ");
   		for(k=0;k<4;k++)
      			printf("%d ",data[k]);
   		printf("\n");
		printf("\n");
		printf("******************************************************\n");
      		}
      
   	} //while
   close(clnt_sock);
   close(serv_sock);
   return 0;
   
}
void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

int atoi2(char str){
	int num = str - 0x30;
	return num;
}
