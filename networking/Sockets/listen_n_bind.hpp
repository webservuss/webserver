#ifndef listen_n_bind_hpp
#define listen_n_bind_hpp

#include "stdio.h"
#include "simple_socket.hpp"
// #include "../../http.hpp"

namespace HTTP 
{
    class listen_n_bind : public simple_socket {
        private:
            int     binding;
        	int     backlog;
	        int     listening;
    	    /* call bind */
        	void    connect_to_network(int sock, struct sockaddr_in address);
        	/* call listen */
        	void    start_listening();

   		public:
        	/* constructor */
        	listen_n_bind(int domain, int service, int protocol, int port, 
			u_long interface, int bklg);
        	/* getters */
        	int     get_listening();
        	int     get_backlog();
        	int     get_binding();
	};
}
#endif