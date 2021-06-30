#include "simple_server.hpp"

/* constructor calls listen_n_bind class creating a simple socket, listening and binding*/
HTTP::simple_server::simple_server(int domain, int service, int protocol, int port, u_long interface, int bklg){

    socket = new HTTP::listen_n_bind(domain, service, protocol, port, interface, bklg); 
    // watch out for memory leaks //delete socket !!!!
}

/* getter */
HTTP::listen_n_bind * HTTP::simple_server::get_socket()
{
    return socket;
}