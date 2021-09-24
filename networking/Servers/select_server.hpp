#ifndef select_server_hpp
#define select_server_hpp

#include <iostream>
#include <vector>
#include <netinet/in.h>
#include <sys/time.h>
#include "../Sockets/listen_n_bind.hpp"
#include "parser_conf.hpp"

#define BACKLOG 100000

namespace HTTP
{
	typedef struct          s_client_select {
		sockaddr_in         _client_addr;
		int                 _c_sock;
		struct timeval      _last_active;
		bool                _active;
		bool 				_close_connection;
		std::string         _header;

		/* variables needed for POST method */
		bool 				_chunked;
		bool 				_expect_body;
		bool 				_post_done;
		std::string			_filename;
		int					_content_length;
		int					_total_body_length;
	}   					t_client_select;

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

	public:
		class	select_error_ex : public std::exception {
		public:
			virtual const char*	what() const throw();
		};
		/* constructor for multiple ports */
		select_server(std::vector<int> ports, std::vector<t_server> parser_servers);
		/*copy constructor */
		select_server(const select_server& x);
		 /*assignment operator */
		select_server& operator=(const select_server& x);
		/*destructor */
		~select_server();
		int     		selecter();
		void    		accepter(int i);
		int             read_from_client(int i, int j);
		void            send_response(int i, int j);
		void    		launch();
		/* getter */
		listen_n_bind * get_socket(); 
		void            set_value_server_select_server(int servers_socket, sockaddr_in servers_addr, int port, t_server_select &server);
		void	        make_client(int client_socket, sockaddr_in addr, t_server_select &server);
		void            check_client_active(t_client_select &client);
		int             erase_client(int i, int j);
	};
}

#endif
