#include "connectingSocket.hpp"

// constructor
void    HDE::connectingSocket::connect_to_network(int sock, struct sockaddr_in address)
{
    connected = connect(sock, (struct sockaddr*)&address, sizeof(address));
}

// definition of connect_to_network virtual function
HDE::connectingSocket::connectingSocket(int domain, int service, int protocol, int port, u_long interface)
    :  simpleSocket(domain, service, protocol, port, interface)
{
    //establish network connection
    // set_connection(connect_to_network(get_sock(), get_address()));
    connect_to_network(get_sock(), get_address());
    test_connection(connected);
}
// THIS IS WHY IT IS NT WORKING
