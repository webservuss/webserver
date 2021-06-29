#ifndef select_server_hpp
#define select_server_hpp

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <fstream>
#include <string>

#include "simple_server.hpp"


namespace HTTP
{
    
        class select_server: public simple_server
        {

            private:
                char buffer[30000] = {0};
                struct timeval  timeout;
                int             connection;
                int             connectlist[5]  = {0};; // dont want 100 want it to be backlog get_socket()->get_backlog()
                int             highsock;
                fd_set          socks;
                int             new_socket;
            public:
                select_server();
                void    setnonblocking(int sock); // should use one from other class
                void    accepter();
                void    handle_new_connection();
                void    handeler();
                void    responder(int listnum);
                void    launch();
        };
        
    
}
#endif