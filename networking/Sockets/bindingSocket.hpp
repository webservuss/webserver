#ifndef bindingSocket_hpp
#define bindingSocket_hpp

#include "stdio.h"
#include "simpleSocket.hpp"
namespace HDE {

class bindingSocket : public simpleSocket {
    private:
         int binding;
         void         connect_to_network(int sock, struct sockaddr_in address);
    public:
        // constructor
        bindingSocket(int domain, int service, int protocol, int port, u_long interface);
        // virtual function from parent
        //int         connect_to_network(int sock, struct sockaddr_in address);
        int get_binding();
};
}

#endif
