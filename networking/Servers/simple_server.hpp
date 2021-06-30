#ifndef simple_server_hpp
#define simple_server_hpp

#include <stdio.h>

#include "../../http.hpp"

namespace HTTP{
    
class simple_server{
    private:
		/* socket created */
    	listen_n_bind * socket;
		/* virtual void functions to make sure all 
		servers inheriting from this simple server 
		accept, handle and respond */
    	virtual void 	accepter()= 0;
    	virtual void 	handeler()= 0;
    public:
		/* constructor */
    	simple_server(int domain, int service, int protocol, int port, u_long interface, 
						int bklg);
   		/* virtual void function to make sure all inheriting classes launch */
		virtual void 	launch()= 0;
    	/* getter */
		listen_n_bind * get_socket();
};

}

#endif
