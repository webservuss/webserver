#include "listening_socket.hpp"


HTTP::listening_socket::listening_socket(int domain , int service, int protocol, int port, 
        u_long interface,  int bklg) : binding_socket(domain, service, protocol, port, interface)
{
    backlog = bklg;
    start_listening();
    test_connection(listening);
}


void HTTP::listening_socket::start_listening()
{
    listening = listen(get_sock(), backlog);
}

int    HTTP::listening_socket::get_listening()
{
    return listening;
}

int  HTTP::listening_socket::get_backlog()
{
    return backlog;
}
