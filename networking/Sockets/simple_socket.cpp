#include <string.h>
#include <stdio.h>
#include <cstdlib>
#include "simple_socket.hpp"
#include "../utils/http_funct.hpp"

/* constructor setting up simple socket
    define address structure, establish socket and test,
    enable rebinding while a previous connection is still in TIME_WAIT state, 
    allow re-use of local address and set non blocking */

const char  *HTTP::simple_socket::socket_error_ex::what() const  throw()
{
	return ("Error in simple_socket");
}

HTTP::simple_socket::simple_socket(int domain, int service, int protocol, int port, u_long interface)
    : _domain(domain), _service(service), _protocol(protocol), _port(port), _interface(interface)
{
    int reuse_addr = 1;
    memset(&_address,0,sizeof(_address));
    _address.sin_family = _domain;
    _address.sin_port = htons(_port);
    _address.sin_addr.s_addr = htonl(_interface);
    _sock = socket(_domain, _service, _protocol);
    test_connection(_sock);
	setsockopt(_sock, SOL_SOCKET, SO_REUSEADDR, &reuse_addr, sizeof(reuse_addr));
    set_non_blocking(_sock);
}

/* copy constructor */
HTTP::simple_socket::simple_socket(const simple_socket& x)
{
    _sock = x._sock;
    _address = x._address;
}

/* assignment operator */
HTTP::simple_socket& HTTP::simple_socket::operator=(const simple_socket& x)
{
    _sock = x._sock;
    _address = x._address;
    return *this;
}

/* destructor */
HTTP::simple_socket::~simple_socket() {}

/* test socket or connection has been properly established */
void    HTTP::simple_socket::test_connection(int item_to_test)
{
    if (item_to_test < 0)
    {
        throw simple_socket::socket_error_ex();
    }
}

/* getter functions */
int                 HTTP::simple_socket::get_sock()         const
{
    return (_sock);
}

struct sockaddr_in  HTTP::simple_socket::get_address()      const
{
    return (_address);
}

int                 HTTP::simple_socket::get_domain()       const
{
    return (_domain);
}

int                 HTTP::simple_socket::get_service()      const
{
    return (_service);
}

int                 HTTP::simple_socket::get_protocol()     const
{
    return (_protocol);
}

int                 HTTP::simple_socket::get_port()          const
{
    return (_port);
}

u_long              HTTP::simple_socket::get_interface()    const
{
    return (_interface);
}
