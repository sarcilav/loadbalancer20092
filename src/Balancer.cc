#include "SocketException.h"
#include "ServerSocket.h"
#include "ClientSocket.h"
#include <stdio.h>
#include <iostream>
#include <string>
#include <string.h>
#include <strings.h>
#include <vector>
#include <stdlib.h>
#include <unistd.h>
using namespace std;
int portNo;

vector<ClientSocket> servers;
void init()
{
  freopen("config","r",stdin);
  scanf("port: %d",&portNo);

  string server;
  int port;
  cin>>server;
  cin>>server;
  //while(cin>>server>>port)
  //servers.push_back(ClientSocket(server,port));
  fclose(stdin);
}
void error(char *msg)
{
  perror(msg);
  exit(1);
}

int main()
{
  try
    {
      init();
      ServerSocket server(portNo);
      while ( true )
        {

          try
            {
              ServerSocket new_sock;
              server.accept ( new_sock );
              int pid = fork();
              if(pid < 0)
                error((char *) "ERROR on fork");
              if(pid == 0)
                {
                  server.~ServerSocket();
                  string data;
                  new_sock >> data;//Attending new connection

                  cerr<<data;
                  ClientSocket tt("localhost",3000);
                  tt << data;//Sending request to back-end


                  int total = (tt >> data);//Getting Response
                  cerr<<data;
                  new_sock << data;//Sending response to the user

                  while(total-data.size()>0)
                    {
                      total -= data.size();
                      tt >> data;
                      cerr<<data;
                      new_sock << data;//Sending response to the user
                    }
                  tt.~ClientSocket();
                  exit(0);
                }
              else
                new_sock.~ServerSocket();
            }
          catch ( SocketException& ) {}
        }


    }
  catch ( SocketException& e )
    {
      cout << "Exception was caught:" << e.description() << "\nExiting.\n";
    }
  return 0;
}
