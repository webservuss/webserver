#ifndef TestServer_hpp
#define TestServer_hpp

#include <stdio.h>
#include <unistd.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <cstring>
#include <iterator>
#include <sstream>
#include <sstream>
#include <vector>
#include <iostream>
#include <map>
#include <string>
#include <map>

#include "SimpleServer.hpp"
#include "../Colors.hpp"


namespace HDE
{
    
        class TestServer: public SimpleServer
        {

            private:
                char buffer[30000] = {0};
                int new_socket;
                void config_parser_open();
                void accepter();
                 void handeler();
                void responder();
            public:
                TestServer();
                void launch();
        };
        
    
}
#endif