#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <unistd.h>


void error(char *msg)
{
  perror(msg);
  exit(1);
}
void read_file(char * file, int & n)
{
  freopen(file,"r",stdin);
  scanf("%d",&n);
  fclose(stdin);
}
void write_file(char * file, const int & n)
{
  freopen(file,"w",stdout);
  printf("%d\n",n);
  fclose(stdout);
}

struct lb_server
/*
  Load Balancer server-side
  Is a simple server that attendent
  the connection of the user
  and use h buffer of the request connection
  to pass to back-end server

  See client-side for the other part of the LB
 */
{
  int sockfd, newsockfd, portno, clilen, pid;
  struct sockaddr_in serv_addr, cli_addr;
  lb_server(){}
  lb_server(int port)
  {
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
      error((char *)"ERROR opening socket");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    portno = port;
    serv_addr.sin_port = htons(portno);//lookme
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
             sizeof(serv_addr)) < 0)
      error((char *)"ERROR on binding");
    listen(sockfd,5);
    clilen = sizeof(cli_addr);
  }

  int new_connection();
};
int lb_server::new_connection()
{
  return newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, (socklen_t *)&clilen);
}

struct lb_client{
  int clientSocket;
  int clientPortNo;
  int sizeofServerDIr;
  struct sockaddr_in serverDir;
  struct hostent *server;
  lb_client(char *serverName,int port){
    clientSocket = socket(AF_INET,SOCK_STREAM,0);
    if(clientSocket<0)
      error((char *)"ERROR al abrir el socket");

    server = gethostbyname(serverName);

    if(server==NULL)
      error((char *)"ERROR al no encontrar el servidor");
    bzero((char *)&serverDir, sizeof(serverDir));
    serverDir.sin_family=AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&serverDir.sin_addr.s_addr,server->h_length);
    serverDir.sin_port=htons(port);

  }

  int client_connect();
  int client_write(char *buffer);
  int client_read(char *buffer);
};// lb_client;

int lb_client::client_connect(){
  return connect( clientSocket,(struct sockaddr *) &serverDir, sizeof(serverDir));
}//if <0 is returned, error connecting

int lb_client::client_write(char *buffer){
  return write(clientSocket,buffer,strlen(buffer));
}//if <0 is returned, error writing to socket

int lb_client::client_read(char *buffer){//TODO: char *buffer[] or char &buffer[] ????
  return read(clientSocket, buffer,sizeof(buffer));
}//if <0 is returned, error reading from socket



void dostuff (int sock,int serverNo){
  struct sockaddr_in serv_addr_bak;
  struct hostent *server_bak;
  int n;
  char buffer[256];
  bzero(buffer,256);
  n = read(sock,buffer,255);
  if (n < 0) error((char *)"ERROR reading from socket");
  printf("Here is the message: %s\n",buffer);

  lb_client client_side((char *)"127.0.0.1",3000);

  if(client_side.client_connect()<0)
    error((char *) "ERROR connecting");

  if(client_side.client_write(buffer)<0)
    error((char *)"ERROR writing to socket client-side");

  char response[256];

  bzero(response,strlen(response));

  if(client_side.client_read(response)<0)
    error((char *) "ERROR reading from socket client-side");

  printf("\n\t Here is your response\n%s\n",response);


  n = write(sock,response,strlen(response));
  if (n < 0) error((char *)"ERROR writing to socket");
}



int main(int argc, char *argv[])
{
  if (argc < 2)
    {
      fprintf(stderr,"ERROR, no port provided\n");
      exit(1);
    }

  lb_server server (atoi(argv[1]));

  int server_no = 0;
  while ( true )
    {
      if(server.new_connection() < 0)
        error((char *) "ERROR on accept");

      server.pid = fork();

      if(server.pid < 0)
        error((char *) "ERROR on fork");

      if(server.pid == 0)
        {
          close(server.sockfd);

          read_file((char *)"server_no",server_no);

          //do_something
          dostuff(server.newsockfd,server_no);

          ++server_no;
          server_no %= 3;//changeme

          write_file((char *)"server_no",server_no);

          exit(0);
        }
      else
        close(server.newsockfd);
    }
  /*Unreacheble code*/
  return 0;
}
