#ifndef listen_n_bind_hpp
#define listen_n_bind_hpp

#include "stdio.h"
#include "simple_socket.hpp"

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
			/* empty constructor for complian form */
            listen_n_bind();

   		public:
        	/* constructor */
        	listen_n_bind(int domain, int service, int protocol, int port, 
			u_long interface, int bklg);
            /*copy constructor */
            listen_n_bind(const listen_n_bind& x);
            /*assignment operator */
            listen_n_bind& operator=(const listen_n_bind& x);
            /*destructor */
            ~listen_n_bind();

        	/* getters */
        	int     get_listening() const;
        	int     get_backlog() 	const;
        	int     get_binding() 	const;
	};
}
#endif