### Makefile Load Balancer project

## Author: sebastian@wormwood
## Version: $Id: Makefile,v 0.0 2009/10/20 20:29:16 sebastian Exp $
## Keywords: loadbalancer c/c++ socket linux
## X-URL: http://github.com/sarcilav/loadbalancer20092
CPP = g++
CFLAGS = -O2
OBJ = ClientSocket.o ServerSocket.o Socket.o
RM = rm -f

Socket.o: 
	$(CPP) $(CFLAGS) -c Socket.cc -o Socket.o
ClientSocket.o: Socket.o
	$(CPP) $(CFLAGS) -c ClientSocket.cc -o ClientSocket.o
ServerSocket.o: Socket.o
	$(CPP) $(CFLAGS) -c ServerSocket.cc -o ServerSocket.o
Balancer.o: Socket.o ClientSocket.o ServerSocket.o
	$(CPP) $(CFLAGS) -c Balancer.cc -o Balancer.o
all: Balancer.o
	$(CPP) $(CFLAGS) $(OBJ) Balancer.o -o Balancer
agent: Socket.o ClientSocket.o ServerSocket.o
	$(CPP) $(CFLAGS) $(OBJ) agent.cc -o agent
clean: 
	${RM} $(OBJ) $(BIN)





### Makefile ends here
