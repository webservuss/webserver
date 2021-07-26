#ifndef select_server_hpp
#define select_server_hpp

#include "../../http.hpp"
#include "../utils/http_funct.hpp"

#define BACKLOG 0

namespace HTTP
{
        typedef struct t_server_select {
            sockaddr_in         _servers_addr;
            int                 _servers_socket;
            std::vector<int>    _client_sockets;
        }   t_server_select;

        class select_server
        {
            private:
                /* variables needed for select()*/
                int                             _highsock;
                fd_set                          _read_fds;
                fd_set                          _write_fds;
                fd_set                          _write_backup;
                fd_set                          _read_backup;
                std::vector<t_server_select>    _servers;
            public:
                /* constructor */
                select_server();
                // constructor for multiple ports */
                select_server(std::vector<int> ports);
                /* constructor specifying multiple port */
                select_server(int *port, int numb_ports);
                /*copy constructor */
                select_server(const select_server& x);
                // /*assignment operator */
                select_server& operator=(const select_server& x);
                // /*destructor */
                ~select_server();
                int     		selecter();
                void    		accepter(int i);
                int             read_from_client(int i, int j);
                void            send_response(int i, int j);
                void    		launch();
            	/* getter */
		        listen_n_bind * get_socket();
                /* set values in struct */
                void            set_value_server_select_server(int servers_socket, sockaddr_in servers_addr, t_server_select &server);
                void            set_value_server_select_client(int client_socket, t_server_select &server);
        };
}
#endif