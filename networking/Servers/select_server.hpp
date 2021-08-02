#ifndef select_server_hpp
#define select_server_hpp

#include "../../http.hpp"
#include "../utils/http_funct.hpp"
#include "../Request/re_HTTP.hpp"
#include "../Servers/parser_conf.hpp"
#include "../utils/req_n_conf.hpp"

# include <sys/time.h>
#include "../Respond/respond.hpp"

#define BACKLOG 10

namespace HTTP
{
        typedef struct t_client_select {
            sockaddr_in         _client_addr;
            int                 _c_sock;
            struct timeval      _last_active;
            bool                _active;
            std::string         _header;
        }   t_client_select;

        typedef struct t_server_select {
            sockaddr_in                     _servers_addr;
            int                             _servers_socket;
            int                             _port;
            std::vector<t_client_select>    _clients;
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
                std::vector<t_server>           _parser_servers;
                // std::string                     _totalheader;
            public:
                /* constructor */
                // select_server();
                // constructor for multiple ports */
                select_server(std::vector<int> ports, std::vector<t_server> parser_servers);
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
                void            set_value_server_select_server(int servers_socket, sockaddr_in servers_addr, int port, t_server_select &server);
                void	        make_client(int client_socket, sockaddr_in addr, t_server_select &server);
                void            check_client_active(t_client_select &client);
                int             erase_client(int i, int j);
        };
}

#endif
