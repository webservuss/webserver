#ifndef selectServer_hpp
#define selectServer_hpp

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <fstream>
#include <string>

#include "SimpleServer.hpp"


namespace HDE
{
    
        class selectServer: public SimpleServer
        {

            private:
                char buffer[30000] = {0};
                struct timeval  timeout;
                int             connection;
                int             connectlist[5]; // dont want 100 want it to be backlog get_socket()->get_backlog()
                int             highsock;
                fd_set          socks;
                int             new_socket;
            public:
                selectServer();
                void    setnonblocking(int sock); // should use one from other class
                void    accepter();
                void    handle_new_connection();
                void    handeler();
                void    responder(int listnum);
                void    launch();
        };
        
    
}
#endif