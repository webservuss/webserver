#ifndef TestServer_hpp
#define TestServer_hpp

#include <stdio.h>
#include <unistd.h>

#include "SimpleServer.hpp"


namespace HDE
{
    
        class TestServer: public SimpleServer
        {

            private:
            char buffer[30000] = {0};
            int new_socket;
            virtual void accepter()= 0;
            virtual void handeler()= 0;
            virtual void responder()= 0;
            public:
            TestServer();
            void launch();
        };
        
    
}
#endif