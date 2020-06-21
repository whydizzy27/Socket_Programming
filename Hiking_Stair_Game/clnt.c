#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>

#define BUF_SIZE 1024

void error_handling(char *message);
int atoii(char str);
int iitoa(int i);

int main(int argc, char *argv[])
{
 int sock;

 char message[BUF_SIZE];
 char max[1];
 int str_len, recv_len, recv_cnt;

 int serv=0;  // server stair
 int clnt=0;  // clnt stair 

 double total=0;
 double s_win=0;
 double c_win=0;
 
 double c_rate=0;
 double s_rate =0;

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
 else{
  puts("");
 }

 puts("==============GAME START===============");
 puts("=== 0:Scissor 1:Rock 2:Paper Q:quit ===");
 puts("win) Scissor : +2, Rock : +3, Paper : +5");
 puts("lose) Scissor : -1, Rock : -2, Paper : -4\n\n");  
 
 
 
 while(1)
 {
 
  fputs("Your Choice?(Q : quit) : " , stdout);
  fgets(message, BUF_SIZE, stdin);

  str_len = write(sock, message, strlen(message));

  recv_len = 0;

  if(!strcmp(message,"q\n") || !strcmp(message, "Q\n"))
   break;

  if((str_len = read(sock, message, BUF_SIZE)) != 0)
  {
   if(message[2]=='0'){
    printf("Client Win!!! \(>O<)/\n");
    printf("clnt Num : %d , serv Num : %d \n",atoii(message[0]),atoii(message[1]));
    printf("-----------------------------------\n");
    total++;
    c_win++;
    } 
   else if(message[2]=='1'){
    printf("==draw==\n");
    printf("clnt Num : %d , serv Num : %d \n",atoii(message[0]),atoii(message[1]));
    printf("-----------------------------------\n");
    total++;

    }
   else if(message[2]=='2'){
    printf("Client Lose...(T_T) \n");
    printf("clnt Num : %d , serv Num : %d \n",atoii(message[0]),atoii(message[1]));
    printf("-----------------------------------\n");
    total++;
    s_win++;
    }
   else if(message[2]=='3'){
    printf("!!Client Win!! Congraturaion!! \n");
    printf("clnt Num : %d , serv Num : %d \n",atoii(message[0]),atoii(message[1]));
    break;
    }
   else if(message[2]=='4'){
    printf("Server Win.. Next Chance..... \n");
    printf("clnt Num : %d , serv Num : %d \n",atoii(message[0]),atoii(message[1]));
    break;
    }
 
  }
 }

 close(sock);
 return 0;
}

void error_handling(char *message)
{
 fputs(message, stderr);
 fputc('\n', stderr);
 exit(1);
}

int atoii(char str){
 int num = str - 0x30;
 return num;
}

int iitoa(int i){
        char ch=i+0x30;
        return ch;
} 
