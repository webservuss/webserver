#ifndef listeningSocket_hpp
#define listeningSocket_hpp

// this class implements a call to the connect() function of sockets as a definition to the connect_to_network virtual function

#include "stdio.h"
#include "bindingSocket.hpp"

namespace HDE {

class listentingSocket:public bindingSocket{
    private:
        int backlog;
        int listening;
    public:
        listentingSocket(int domain , int service, int protocol, int port, 
        u_long interface,  int bklg);
        void    start_listening();
        int     get_listening();
        int     get_backlog();

};

}

#endif
