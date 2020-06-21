#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>

#define BUF_SIZE 1024
#define BUF_LEN 128

void error_handling(char *message);

int main(int argc, char *argv[])
{
	int sock;
	char message[BUF_SIZE];
	int str_len, recv_len, recv_cnt;
	int strike=0;
	int ball=0;
	
	int msgcnt=10;
	int data[4]={0,};

	struct sockaddr_in serv_adr;

	if(argc != 3){
		printf("Usage: %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	sock = socket(PF_INET, SOCK_STREAM, 0);
	if(sock == -1)
		error_handling("socket() error");

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_adr.sin_port = htons(atoi(argv[2]));

	if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
		error_handling("connect() error!");

	else
		puts("");
		puts("************************************************************************\n");
		puts("! ! G A M E  S T A R T ! !\n");
		puts("************************************************************************\n");
		puts("If you want to end this game, press 'q/Q'.");
		puts("And you have 10 chances to play games.\nIf you can't clear this game in 10 times, you LOSE.\n");
		puts("");
	
	
	while(1)
	{
		fputs("GUESS! : " , stdout);
		fgets(message, BUF_SIZE, stdin);

		str_len = write(sock, message, strlen(message));
		recv_len = 0;
	
		if(!strcmp(message,"q\n") || !strcmp(message, "Q\n")){
			printf("Q U I T  T H E  G A M E ! ! !\n");
			break;
		}

		if((str_len = read(sock, message, BUF_SIZE)) != 0)
		{
			if((int)message[1]==4){
				printf("\n");
				printf("C O R R E C T ! ! ! G A M E  C L E A R~\n");
				printf("You completed the games in %d times\n",11-msgcnt);
				printf("If you want to end this game, press 'q/Q'.");
				printf("\n");
				printf("\n");

			}

			else{ 
				strike=(int)message[1];
				ball=(int)message[0];

				msgcnt--;

				printf("%d Strike %d Ball\n",strike,ball);
			
			} 

			if(msgcnt==0){
				printf("\nY o u L o s e . G A M E O V E R .\n");
				printf("\n");
				break;
			}
		} //if
	} //while

	close(sock);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
