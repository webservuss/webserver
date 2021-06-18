
#include "simpleSocket.hpp"

// defult connstructor

HDE::simpleSocket::simpleSocket(int domain, int service, int protocol, int port, u_long interface)
{
    //define address structure
    address.sin_family = domain;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = htonl(interface);
    //establish socket
    sock = socket(domain, service, protocol);
    test_connection(sock);
}

//test connection virtual function

void HDE::simpleSocket::test_connection(int item_to_test)
{
    // confirm that the socket or connection has been properly established
    if (item_to_test < 0)
    {
        perror("Failed to connect..");
        exit(EXIT_FAILURE);        
    }
}

//getter functions
int         HDE::simpleSocket::get_sock()
{
    return (sock);
}

int            HDE::simpleSocket::get_connection()
{
    return (connection);
}

struct sockaddr_in HDE::simpleSocket::get_address()
{
    return (address);
}

// setter function

void    HDE::simpleSocket::set_connection(int con)
{
    connection = con;
    return;
}

