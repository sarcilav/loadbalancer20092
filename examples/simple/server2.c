/* A simple server in the internet domain using TCP
   The port number is passed as an argument 
   This version runs forever, forking off a separate 
   process for each connection
*/
#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

void dostuff(int,int); /* function prototype */
void error(char *msg)
{
  perror(msg);
  exit(1);
}

int main(int argc, char *argv[]){
  int sockfd, newsockfd, portno, clilen, pid;
  struct sockaddr_in serv_addr, cli_addr;

  if (argc < 2) {
    fprintf(stderr,"ERROR, no port provided\n");
    exit(1);
  }
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) 
    error("ERROR opening socket");
  bzero((char *) &serv_addr, sizeof(serv_addr));
  portno = atoi(argv[1]);
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portno);
  if (bind(sockfd, (struct sockaddr *) &serv_addr,
	   sizeof(serv_addr)) < 0) 
    error("ERROR on binding");
  listen(sockfd,5);
  clilen = sizeof(cli_addr);
  int serverNo=0; //DELETE

  while (1) {
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, (socklen_t *)&clilen);
    if (newsockfd < 0) 
      error("ERROR on accept");
    pid = fork();
    if (pid < 0)
      error("ERROR on fork");
    if (pid == 0)  {
      close(sockfd);
      freopen("serverNo","r",stdin);
      scanf("%d",&serverNo);
      fclose(stdin);
      dostuff(newsockfd,serverNo);
      
      serverNo++;
      
      serverNo%=3;
      //      printf("-----%d\n",serverNo);
      freopen("serverNo","w",stdout);
      printf("%d\n",serverNo);
      //      fflush(stdout);
      fclose(stdout);
      
      exit(0);
    }
    else close(newsockfd);
  } /* end of while */
  return 0; /* we never get here */
}

/******** DOSTUFF() *********************
	  There is a separate instance of this function 
	  for each connection.  It handles all communication
	  once a connnection has been established.
*****************************************/
void dostuff (int sock,int serverNo){
  
  int n;
  char buffer[256];
  bzero(buffer,256);
  n = read(sock,buffer,255);
  if (n < 0) error("ERROR reading from socket");
  printf("Here is the message: %s\n",buffer);
  /////////
  int sockfd, portno, n;
  struct sockaddr_in serv_addr;
  struct hostent *server;
  portno = atoi(argv[2]);
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) 
    error("ERROR opening socket");
  server = gethostbyname(argv[1]);
  if (server == NULL) {
    fprintf(stderr,"ERROR, no such host\n");
    exit(0);
  }
  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  bcopy((char *)server->h_addr, 
	(char *)&serv_addr.sin_addr.s_addr,
	server->h_length);
  serv_addr.sin_port = htons(portno);
  if (connect(sockfd,&serv_addr,sizeof(serv_addr)) < 0) 
    error("ERROR connecting");
  bzero(buffer,256);
  //  fgets(buffer,255,stdin);
  n = write(sockfd,buffer,strlen(buffer));
  if (n < 0) 
    error("ERROR writing to socket");
  bzero(buffer,256);
  n = read(sockfd,buffer,255);
  if (n < 0) 
    error("ERROR reading from socket");
  printf("%s\n",buffer);
  /////////
  char buff[20];
  sprintf(buff,"I got your message %d",serverNo);
  n = write(sock,buff,strlen(buff));
  if (n < 0) error("ERROR writing to socket");
}
