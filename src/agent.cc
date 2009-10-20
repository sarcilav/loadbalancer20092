#include <iostream>
#include "ServerSocket.h"
#include <stdlib.h>
using namespace std;
int main (){

  ServerSocket agente(9000);
  while(true)
    {
      ServerSocket new_socket;
      agente.accept(new_socket);
      int pid = fork();
      if(pid<0)
        exit(-1);
      else if(pid == 0)
        {

          string load;
          new_socket >> load;
          system("uptime|cut -d: -f5|cut -d, -f1>ServerLoad.lb20092");
          FILE *input = fopen( "ServerLoad.lb20092", "r" );
          load = "";
          char buffer[255];
          fscanf(input,"%s\0",buffer);
          load = buffer;
          new_socket << load;

          new_socket.~ServerSocket();

          exit(0);
        }
      else
        new_socket.~ServerSocket();
    }

  return 0;
}
