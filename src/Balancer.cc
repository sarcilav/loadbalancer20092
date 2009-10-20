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
}
void error(char *msg)
{
  perror(msg);
  exit(1);
}

SERVER select_server()
{
  freopen("balancer.log","a",stdout);
  cout<<time(0)<<" ";
  if(method == "roundrobin")
    {
      cout<<method<<" "<<servers[server_in_use%servers.size()].ip<<" "<<servers[server_in_use%servers.size()].port<<"\n";
      fclose(stdout);
      return servers[server_in_use%servers.size()];
    }
  if(method == "lc")
    {
      cout<<method<<"\n";

      int load=1<<30;
      int index;
      for(int i=0;i<servers.size();++i)
        {
          ClientSocket s(servers[i].ip,9000);
          s << "lc";
          usleep(200000);
          string loads;
          s >> loads;
          int t;
          sscanf(loads.c_str(),"%d",&t);
          cout<<servers[i].ip<<" "<<servers[i].port<<" "<<t-2<<"\n";
          if(t<load)
            {
              index = i;
              load = t;
            }
          s.~ClientSocket();
        }
      cout<<"Select "<<servers[index].ip<<" "<<servers[index].port<<" "<<load-2<<"\n";
      fclose(stdout);

      return servers[index];
    }
  if(method == "lbs")
    {
      cout<<method<<"\n";
      float load=1e7;
      int index;
      for(int i=0;i<servers.size();++i)
        {
          ClientSocket s(servers[i].ip,9000);
          s << "lbs";
          usleep(200000);
          string loads;
          s >> loads;
          double t;
          sscanf(loads.c_str(),"%lf",&t);
          cout<<servers[i].ip<<" "<<servers[i].port<<" "<<t<<"\n";
          if(t<load)
            {
              index = i;
              load = t;
            }
          s.~ClientSocket();
        }
      cout<<"Select "<<servers[index].ip<<" "<<servers[index].port<<" "<<load<<"\n";
      fclose(stdout);
      return servers[index];
    }
  if(method == "own")
    {
      cout<<method<<"\n";
      int min_time=1<<30;
      int index;
      for(int i=0;i<servers.size();++i)
        {
          ClientSocket s(servers[i].ip,servers[i].port);
          string loads;
          int time = 0;
          s << "GET \r\n\r\n";
          while((s >> loads)<0)
            {
              ++time;
            }
          cout<<servers[i].ip<<" "<<servers[i].port<<" "<<time<<"\n";
          if(time<min_time)
            {
              index = i;
              min_time = time;
            }

          s.~ClientSocket();
        }
      cout<<"Select "<<servers[index].ip<<" "<<servers[index].port<<" "<<min_time<<"\n";
      fclose(stdout);
      return servers[index];

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
