#ifndef SimpleServer_hpp
#define SimpleServer_hpp

#include <stdio.h>


#include "../hdelibc_networking.hpp"

namespace HDE{
    
class SimpleServer{



    private:
    listentingSocket * socket;

    public:
    SimpleServer(int domain, int service, int protocol, int port, u_long interface, int bklg);
    }

};

#endif