#ifndef select_server_hpp
#define select_server_hpp

#include "../../http.hpp"
#include "../utils/http_funct.hpp"

#define BACKLOG 10

namespace HTTP
{
        class select_server
        {
            private:
                /* variables needed for select()*/
                int             _highsock;
                fd_set          _read_fds;
                fd_set          _write_fds;
                int             _connectlist[BACKLOG];
                int             _sockets[BACKLOG]; //bklg?
                listen_n_bind * _socket;
            public:
                /* constructor */
                select_server();
                /* constructor specifying port */
                // select_server(int port);
                /* constructor specifying multiple port */
                select_server(int *port, int numb_ports);
                /*copy constructor */
                select_server(const select_server& x);
                // /*assignment operator */
                select_server& operator=(const select_server& x);
                // /*destructor */

                ~select_server();
                /* call select with the correct FD_SET */
                // int             selecter(int numb);
                /* implement the virtual void functions from simple_server
                to accept, handle respond and launch */
                int     		selecter(int numb);
                void    		accepter(int numb);
                // int             accept_new_connection(int server_socket);

                void    		handeler();
                void    		launch(int numb_ports);
            	/* getter */
		        listen_n_bind * get_socket();
        };
}
#endif