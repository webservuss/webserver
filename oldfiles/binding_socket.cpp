
#include "binding_socket.hpp"

// constructor
void    HTTP::binding_socket::connect_to_network(int sock, struct sockaddr_in address)
{
    binding = bind(sock, (struct sockaddr*)&address, sizeof(address));
    // return (void(bind(sock, (struct sockaddr*)&address, sizeof(address))));
}

//definition of connect_to_network virtual function
HTTP::binding_socket::binding_socket(int domain, int service, int protocol, int port, u_long interface)
    :  simple_socket(domain, service, protocol, port, interface)
{
    //establish network connection
    connect_to_network(get_sock(), get_address());
    test_connection(binding);
}
