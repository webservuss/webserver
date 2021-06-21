#ifndef SimpleServer_hpp
#define SimpleServer_hpp

#include <stdio.h>


#include "../hdelibc_networking.hpp"

namespace HDE{
    
class SimpleServer{



    private:
    listentingSocket * socket;
    virtual void accepter()= 0;
    virtual void handeler()= 0;
    virtual void responder()= 0;
    public:
    SimpleServer(int domain, int service, int protocol, int port, u_long interface, int bklg);
    virtual void launch()= 0;
    listentingSocket * get_socket();
};

}

#endif
