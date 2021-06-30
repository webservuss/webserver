#include "simple_server.hpp"


HTTP::simple_server::simple_server(int domain, int service, int protocol, int port, u_long interface, int bklg){

    socket = new HTTP::listen_n_bind(domain, service, protocol, port, interface, bklg); // watch out for memory leak
    //delete socket;
}

HTTP::listen_n_bind * HTTP::simple_server::get_socket()
{
    return socket;
}