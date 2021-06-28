#ifndef TestServer_hpp
#define TestServer_hpp

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <fstream>
#include <string>

#include "SimpleServer.hpp"


namespace HDE
{
    
        class TestServer: public SimpleServer
        {

            private:
                char buffer[30000] = {0};
                int new_socket;
                void accepter();
                void handeler();
                void responder();
            public:
                TestServer();
                void launch();
        };
        
    
}
#endif