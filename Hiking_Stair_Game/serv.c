#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<time.h>
#define BUF_SIZE 1024

void error_handling(char *message);
int atoii(char str);
int iitoa(int i);

void rate_cal(double c_win,double s_win,double total);

int main(int argc, char *argv[])
{
 int serv_sock, clnt_sock;
 char message[BUF_SIZE];
 char ym[3]={'0','0'};
 
 int str_len, i=0;
 int random=0;

  

 int serv=0;  // server stair
 int clnt=0;  // clnt stair

 double total=0;
 double s_win=0;
 double c_win=0;
 
 double c_rate=0;
 double s_rate =0;


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
   printf("Connected client \n");  


 while(1)
 {
  random = rand()%3;


  if((str_len = read(clnt_sock, message, BUF_SIZE)) != 0)
  {
   printf("Client choice??? (0:scissor, 1:rock, 2:paper) : %s ", message);
   total++;
  }
  
  //0:gawe 1:bawe 2: bo    0: win  1: draw 2: lose
  if(message[0]=='0' && random==2){
   printf("Server's choice : %d \n",random);
   printf("Client Win!!! \(>O<)/\n");

   clnt = clnt + 1;
   serv = serv - 2;
   ym[2]='0';
   
   c_win++;
  }
  else if(message[0]=='0'&& random==0){
   printf("Server's choice : %d \n",random);
   printf("====draw====\n");

   ym[2]='1';
   
  }
  else if(message[0]=='0'&& random==1){
   printf("Server's choice : %d \n",random);
   printf("Client Lose...(T_T) \n");
   clnt =clnt -0;
   serv = serv +2;

   ym[2]='2';
   
     
   s_win++;
   }

  

  else if(message[0]=='1'&& random==0){
   printf("Server's choice : %d \n",random);
   printf("Client Win!!! \(>O<)/\n");
   clnt = clnt +2;
   serv = serv- 0;
   ym[2]='0';
   
   
   c_win++;
  }
  else if(message[0]=='1'&& random==1){
   printf("Server's choice : %d \n",random);
   printf("====draw====\n");
   ym[2]='1';
   
  }
  else if(message[0]=='1'&& random==2){
   printf("Server's choice : %d \n",random);
   printf("Client Lose...(T_T) \n");
   
   clnt =clnt - 1;
   serv =serv + 3;
   ym[2]='2';
   
   s_win++;
  }


  else if(message[0]=='2'&& random==1){
   printf("Server's choice : %d \n",random);
   printf("Client Win!!! \(>O<)/\n");

   clnt =clnt+ 3;
   serv =serv- 1;
   ym[2]='0';
  
   c_win++;
  }
  else if(message[0]=='2'&& random==2){
   printf("Server's choice : %d \n",random);
   printf("====draw====\n");
   ym[2]='1';
   
  }
  else if(message[0]=='2'&& random==0){
   printf("Server's choice : %d \n",random);
   printf("Client Lose...(T_T) \n");

   clnt =clnt - 2;
   serv =serv + 1;
   ym[2]='2';
   
   s_win++;
  }

  if(clnt<0){
   clnt = 0;
  }
  if(serv<0){
   serv = 0;
  }


  if(message[0]=='q'||message[0]=='Q'){
   break;
  }

  
  if(clnt>=9 ){
   printf("------------------------------------\n");
   printf("!!Client Win!! Congraturaion!! \n");
   printf("Client : 9 , Server : %d\n",serv);
   ym[2]='3';
   ym[1]=iitoa(serv);
   ym[0]=iitoa(9);
   rate_cal(c_win, s_win, total);
   write(clnt_sock, ym, sizeof(ym));
   break;
  }
  
  else if(serv>=9 ){
   printf("------------------------------------\n");
   printf("Server Win.. Next Chance..... \n");
   printf("Server : 9 , Client : %d\n",clnt);
   ym[2]='4';
   ym[1]=iitoa(9);
   ym[0]=iitoa(clnt);
   rate_cal(c_win, s_win, total);
   write(clnt_sock, ym, sizeof(ym));
   break;
  }
  
  else{
   ym[1]=iitoa(serv);
   ym[0]=iitoa(clnt);
   write(clnt_sock, ym, sizeof(ym));
  
   printf("clnt Num : %d, serv Num : %d\n",clnt,serv);
   printf("------------------------------------\n");
  }
 }
 close(clnt_sock);
 close(serv_sock);
 return 0;
}

void rate_cal(double c_win,double s_win,double total){
  double c_rate = (c_win/total)*100;
  double s_rate= (s_win/total)*100;
 printf("total : %.0f, Client win rate : %.0f%, Server win rate : %.0f% \n", total,c_rate, s_rate);
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

