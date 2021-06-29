#ifndef simple_server_hpp
#define simple_server_hpp

#include <stdio.h>


#include "../../http.hpp"

namespace HTTP{
    
class simple_server{
    private:
    	listen_n_bind * socket;
    	virtual void accepter()= 0;
    	virtual void handeler()= 0;
    	virtual void responder()= 0;
    public:
    	simple_server(int domain, int service, int protocol, int port, u_long interface, int bklg);
   		virtual void launch()= 0;
    	listen_n_bind * get_socket();
};

}

#endif
