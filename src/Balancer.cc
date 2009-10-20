#include <time.h>
#include <cassert>
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

                  ClientSocket tt("200.12.180.5",80);
                  tt << data;//Sending request to back-end


                  usleep(200000);
                  int total = (tt >> data);//Getting Response

                  cerr<<data;

                  new_sock << data;//Sending response to the user
                  cerr << "Voy a traer el resto\n";
                  int i = 0;
                  cerr<< "Total = "<<total<<"\n";
                  while(total>0)
                    {
                      usleep(5000);
                      total = tt >> data;
                      ///cerr<< "Total = "<<total<<"\n";
                      //cerr<<i++<<"\n";
                      if(total > 0)
                        {
                          new_sock << data;

                          cerr<<data;
                          //cout<<data;
                        }
                      else
                        {
                          break;// error reading from socket
                        }
                    }
                    cerr<<"Sali\n";
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
