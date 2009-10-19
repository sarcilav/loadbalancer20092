// Definition of the ServerSocket class

#ifndef ServerSocket_class
#define ServerSocket_class

#include "Socket.h"
using namespace std;

class ServerSocket : Socket
{
 public:

  ServerSocket ( int port );
  ServerSocket (){};
  virtual ~ServerSocket();

  const ServerSocket& operator << ( const string& ) const;
  const ServerSocket& operator >> ( string& ) const;

  void accept ( ServerSocket& );

};


#endif
