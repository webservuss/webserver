#ifndef testserver_hpp
#define testserver_hpp

#include <stdio.h>

#include "SimpleServer.hpp"


namespace HDE
{
    
        class TestServer: public SimpleServer
        {

            private:
            virtual void accepter()= 0;
            virtual void handeler()= 0;
            virtual void responder()= 0;
            public:
        };
        
    
}
#endif