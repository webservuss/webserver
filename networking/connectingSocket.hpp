#ifndef connectingSocket_hpp
#define connectingSocket_hpp

// this class implements a call to the connect() function of sockets as a definition to the connect_to_network virtual function

#include "stdio.h"
#include "simpleSocket.hpp"
namespace HDE {

class connectingSocket : public simpleSocket {
    public:
        // constructor
        connectingSocket(int domain, int service, int protocol, int port, u_long interface);
        // virtual function from parent
        int         connect_to_network(int sock, struct sockaddr_in address);

};
}

#endif
