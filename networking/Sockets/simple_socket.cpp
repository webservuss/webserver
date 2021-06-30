#include "simple_socket.hpp"

/* constructor setting up simple socket
    define address structure 
    establish socket and test
    enable rebinding while a previous connection is still in TIME_WAIT state, allow re-use of local address
    and set non blocking */
HTTP::simple_socket::simple_socket(int domain, int service, int protocol, int port, u_long interface)
    : domain(domain), service(service), protocol(protocol), port(port), interface(interface)
{
    int reuse_addr = 1;
    memset(&address,0,sizeof(address));
    address.sin_family = domain;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = htonl(interface);
    sock = socket(domain, service, protocol);
    test_connection(sock);
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse_addr, sizeof(reuse_addr));
    set_non_blocking(sock);
}

/* test socket or connection has been properly established */
void    HTTP::simple_socket::test_connection(int item_to_test)
{
    if (item_to_test < 0)
    {
        perror("Failed to connect..");
        exit(EXIT_FAILURE);        
    }
}

/* getter functions */
int                 HTTP::simple_socket::get_sock()         const
{
    return (sock);
}

struct sockaddr_in  HTTP::simple_socket::get_address()      const
{
    return (address);
}

int                 HTTP::simple_socket::get_domain()       const
{
    return (domain);
}

int                 HTTP::simple_socket::get_service()      const
{
    return (service);
}

int                 HTTP::simple_socket::get_protocol()     const
{
    return (protocol);
}

int                 HTTP::simple_socket::get_port()          const
{
    return (port);
}

u_long              HTTP::simple_socket::get_interface()    const
{
    return (interface);
}

/* complian form */
/* empty constructor */
// HTTP::simple_socket::simple_socket() : sock(0) {memset(&address, 0, sizeof(struct sockaddr_in));}

/*copy constructor */
HTTP::simple_socket::simple_socket(const simple_socket& x)
{
    sock = x.sock;
    address = x.address;
}

/*assignment operator */
HTTP::simple_socket& HTTP::simple_socket::operator=(const simple_socket& x)
{
    // if (sock != x.sock || address != x.address)
    sock = x.sock;
    address = x.address;
    return *this;
}

/*destructor */
HTTP::simple_socket::~simple_socket() {}
