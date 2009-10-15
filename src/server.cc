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

  lb_server()
  {
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
      error((char *)"ERROR opening socket");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
  }
  lb_server(int port)
  {
    lb_server();
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
