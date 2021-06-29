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
                char            buffer[30000];
                struct timeval  timeout;
                int             connection;
                int             connectlist[5]; // dont want 100 want it to be backlog get_socket()->get_backlog()
                int             highsock;
                fd_set          socks;
                int             new_socket;
            public:
                select_server();
                void    setnonblocking(int sock); // should use one from other class
                void    accepter();
                void    handeler();
				void	responder();
                void    launch();
        };
        
    
}
#endif