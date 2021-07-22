#ifndef select_server_hpp
#define select_server_hpp

#include "../../http.hpp"
#include "../utils/http_funct.hpp"

#define BACKLOG 100

namespace HTTP
{
        class select_server
        {
            private:
                /* variables needed for select()*/
                int             _highsock;
                fd_set          _read_fds;
                fd_set          _write_fds;
                fd_set          _write_backup;
                fd_set          _read_backup;
                // std::vector<int>  _connectlist;
                int             _connectlist[BACKLOG]; //bklg?
                int             _sockets[BACKLOG]; //bklg?
                // listen_n_bind * _socket;
                std::vector<sockaddr_in> _servers_addr;
                std::vector<int>        _servers_socket;
                std::vector<sockaddr_in> _client_addr;
                std::vector<int>        _client_socket;
            public:
                /* constructor */
                select_server();
                //
                select_server(std::vector<int> ports);
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
                int     		selecter();
                void    		accepter(int i);
                void            read_from_client(int i);
                void            send_response(int i);
                // int             accept_new_connection(int server_socket);

                void    		handeler();
                void    		launch();
            	/* getter */
		        listen_n_bind * get_socket();
        };
}
#endif