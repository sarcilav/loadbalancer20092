// Definition of the ClientSocket class

#ifndef ClientSocket_class
#define ClientSocket_class

#include "Socket.h"
using namespace std;

class ClientSocket : Socket
{
 public:

  ClientSocket ( std::string host, int port );
  virtual ~ClientSocket(){};

  //const ClientSocket& operator << ( const string& ) const;
  bool operator << ( const string& ) const;
  int operator >> ( string& ) const;

};


#endif
