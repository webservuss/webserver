#ifndef test_server_hpp
#define test_server_hpp

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <fstream>
#include <string>

#include "simple_server.hpp"


namespace HTTP
{
    
        class test_server: public simple_server
        {

            private:
                char buffer[30000] = {0};
                int new_socket;
                void accepter();
                void handeler();
                void responder();
            public:
                test_server();
                void launch();
        };
        
    
}
#endif