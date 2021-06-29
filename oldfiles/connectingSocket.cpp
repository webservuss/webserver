#include "connectingSocket.hpp"

// constructor
void    HTTP::connectingSocket::connect_to_network(int sock, struct sockaddr_in address)
{
    connected = connect(sock, (struct sockaddr*)&address, sizeof(address));
}

// definition of connect_to_network virtual function
HTTP::connectingSocket::connectingSocket(int domain, int service, int protocol, int port, u_long interface)
    :  simple_socket(domain, service, protocol, port, interface)
{
    connect_to_network(get_sock(), get_address());
    test_connection(connected);
}
