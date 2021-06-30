#ifndef select_server_hpp
#define select_server_hpp

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <fstream>
#include <string>

#include "simple_server.hpp"
#include "../../http.hpp"
#include "../../http_funct.hpp"

#define BACKLOG 10

namespace HTTP
{
        class select_server : public simple_server
        {
            private:
                /* variable to recv() data */
                char            buffer[30000];
                /* variables needed for select()*/
                int             highsock;
                fd_set          socks;
                int             connectlist[BACKLOG];
            public:
                /* constructor */
                select_server();
                /*copy constructor */
                // select_server(const select_server& x);
                // /*assignment operator */
                // select_server& operator=(const select_server& x);
                // /*destructor */
                // ~select_server();
                // /* */
                int             selecter();
                /* implement the virtual void functions from simple_server
                to accept, handle respond and launch */
                void    		accepter();
                void    		handeler();
                void    		launch();
    
                // /* getter functions*/
                // char[3000]           get_buffer()        const;
                // int             get_highsock()      const;
                // fd_set          get_socks()         const;
                // int*            get_connectlist()   const;
        };
}
#endif