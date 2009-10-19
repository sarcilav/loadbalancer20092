// Implementation of the ClientSocket class

#include "ClientSocket.h"
#include "SocketException.h"
using namespace std;

ClientSocket::ClientSocket (string host, int port )
{
  if ( ! Socket::create() )
    {
      throw SocketException ( "Could not create client socket." );
    }

  if ( ! Socket::connect ( host, port ) )
    {
      throw SocketException ( "Could not bind to port." );
    }
  //Socket::set_non_blocking(true);
}

/*
const ClientSocket& ClientSocket::operator << ( const string& s ) const
{
  if ( ! Socket::send ( s ) )
    {
      throw SocketException ( "Could not write to socket." );
    }

  return *this;

}
*/
bool ClientSocket::operator << ( const string& s ) const
{
  if ( ! Socket::send ( s ) )
    {
      throw SocketException ( "Could not write to socket." );
    }

  return Socket::is_valid();

}

int ClientSocket::operator >> ( string& s ) const
{
  int status;

  if ( ! (status = Socket::recv ( s )))
    {
      throw SocketException ( "Could not read from socket." );
    }

  return status;
}
