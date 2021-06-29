#include "listen_n_bind.hpp"
#include "simple_socket.hpp"

//call bind
void    HTTP::listen_n_bind::connect_to_network(int sock, struct sockaddr_in address)
{
    binding = bind(sock, (struct sockaddr*)&address, sizeof(address));
}

// call listen
void HTTP::listen_n_bind::start_listening()
{
    listening = listen(get_sock(), backlog);
}

// create simple socket, bind, check, listen, check
HTTP::listen_n_bind::listen_n_bind(int domain, int service, int protocol, int port, u_long interface, int bklg) : simple_socket(domain, service, protocol, port, interface)
{
    connect_to_network(get_sock(), get_address());
    test_connection(binding);
    backlog = bklg;
    start_listening();
    test_connection(listening);
}

//getters
int    HTTP::listen_n_bind::get_listening()
{
    return listening;
}

int  HTTP::listen_n_bind::get_backlog()
{
    return backlog;
}

int  HTTP::listen_n_bind::get_binding()
{
    return binding;
}
