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
int portNo,server_in_use=0;
string method;
struct SERVER
{
  string ip;
  int port;
  SERVER(){}
  SERVER(string IP, int P)
  {
    ip=IP;
    port=P;
  }
};
vector<SERVER> servers;
vector<int>connections;
void init()
{
  freopen("config","r",stdin);
  cin >> method;
  cin >> method;
  if(method!="roundrobin" && method!="lc" && method!="lbs" && method !="own")
    exit(-1);
  string server;
  getline(cin,server);
  scanf("port: %d",&portNo);

  int port;
  cin>>server;
  cin>>server;
  while(cin>>server>>port)
    servers.push_back(SERVER(server,port));
  fclose(stdin);
  connections = vector<int> (servers.size(),0);
}
void error(char *msg)
{
  perror(msg);
  exit(1);
}
int J;
SERVER select_server()
{
  if(method == "roundrobin")
    return servers[server_in_use%servers.size()];
  if(method == "lc")
    {
      int j=0 ;
      int m = 1 << 30;
      for(int i = 0; i<connections.size();++i)
        if(connections[i]<m)
          {
            j = i;
            m = connections[i];
          }
      ++connections[j];
      J = j;
      return servers[j];
    }
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
              ++server_in_use;
              SERVER selected = select_server();
              int pid = fork();
              if(pid < 0)
                error((char *) "ERROR on fork");
              if(pid == 0)
                {
                  server.~ServerSocket();
                  string data;
                  new_sock >> data;//Attending new connection
                  //cerr<<data;
                  ClientSocket tt(selected.ip,selected.port);
                  tt << data;//Sending request to back-end
                  usleep(200000);
                  int total = (tt >> data);//Getting Response
                  //cerr<<data;
                  //|sleep(20);
                  new_sock << data;//Sending response to the user
                  while(total>0)
                    {
                      usleep(5000);
                      total = tt >> data;
                      if(total > 0)
                        {
                          new_sock << data;
                          //  cerr<<data;
                        }
                      else
                        {
                          break;// error reading from socket
                        }
                    }
                  //cerr<<"Sali\n";
                  tt.~ClientSocket();
                  exit(0);
                }
              else
                new_sock.~ServerSocket();
              --connections[J];
              for(int i = 0; i<connections.size();++i)
                {
                  assert(connections[i] == 0);
                  printf("%d connections %d\n",servers[i].port,connections[i]);
                }

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
