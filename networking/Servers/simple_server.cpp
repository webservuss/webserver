#include "simple_server.hpp"

/* constructor calls listen_n_bind class creating a simple socket, listening and binding*/
HTTP::simple_server::simple_server(int domain, int service, int protocol, int port, u_long interface, int bklg){

    socket = new HTTP::listen_n_bind(domain, service, protocol, port, interface, bklg); 
}

/* getter */
HTTP::listen_n_bind * HTTP::simple_server::get_socket()
{
    return socket;
}

/*copy constructor */
HTTP::simple_server::simple_server(const simple_server& x)
{
    socket = x.socket;
}

/*assignment operator */
HTTP::simple_server& HTTP::simple_server::operator=(const simple_server& x)
{
    if (socket != x.socket)
        socket = x.socket;
    return *this;
}
/* destructor */
HTTP::simple_server::~simple_server()
{
    delete socket;
}
