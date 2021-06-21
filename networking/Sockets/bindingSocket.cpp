
#include "bindingSocket.hpp"

// constructor
void    HDE::bindingSocket::connect_to_network(int sock, struct sockaddr_in address)
{
    return (void(bind(sock, (struct sockaddr*)&address, sizeof(address))));
}

//definition of connect_to_network virtual function
// HDE::bindingSocket::bindingSocket(int domain, int service, int protocol, int port, u_long interface)
//     :  simpleSocket(domain, service, protocol, port, interface)
// {
//     //establish network connection
//     set_connection(connect_to_network(get_sock(), get_address()));
//     test_connection(get_connection());
// }
