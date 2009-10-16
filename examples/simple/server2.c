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
#include <strings.h>
#include <stdlib.h>
#include <unistd.h>

void dostuff(int,int); /* function prototype */
void error(char *msg){
  perror(msg);
  exit(1);
}

int main(int argc, char *argv[]){
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
  struct sockaddr_in serv_addr_bak;
  struct hostent *server_bak;
  int n;
  char buffer[256];
  bzero(buffer,256);
  n = read(sock,buffer,255);
  if (n < 0) error("ERROR reading from socket");
  printf("Here is the message: %s\n",buffer);




  /////////client
  int ports[]={3000,4000,80};
  char serverName[3][256]={{"10.0.1.22"},{"10.0.1.22"},{"10.0.1.28"}};
  int sockfd,portno=0, n_bak;

  portno = ports[serverNo];
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0)
    error("ERROR opening socket");
  server_bak = gethostbyaddr(serverName[1],AF_INET);
  if (server_bak == NULL) {
    fprintf(stderr,"ERROR, no such host\n");
    exit(0);
  }
  printf("bugui %s\n",server_bak->h_addr);
  bzero((char *) &serv_addr_bak, sizeof(serv_addr_bak));
  serv_addr_bak.sin_family = AF_INET;
  //  bcopy( (char *)server_bak->h_addr, (char *)&serv_addr_bak.sin_addr.s_addr, server_bak->h_length);
  serv_addr_bak.sin_port = htons(portno);
  if (connect(sockfd,&serv_addr_bak,sizeof(serv_addr_bak)) < 0)
    error("ERROR connecting");
  bzero(buffer,256);
  //  fgets(buffer,255,stdin);
  n_bak = write(sockfd,buffer,strlen(buffer));
  if (n_bak < 0)
    error("ERROR writing to socket");
  bzero(buffer,256);
  n_bak = read(sockfd,buffer,255);
  if (n_bak < 0)
    error("ERROR reading from socket");
  printf("%s\n",buffer);
  /////////







  char buff[20];
  sprintf(buff,"I got your message %d",serverNo);
  n = write(sock,buff,strlen(buff));
  if (n < 0) error("ERROR writing to socket");
}
