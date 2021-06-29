#include "simple_socket.hpp"

// defult connstructor

HTTP::simple_socket::simple_socket(int domain, int service, int protocol, int port, u_long interface)
{
    int reuse_addr = 1;
    //define address structure
    memset(&address,0,sizeof(address));
    address.sin_family = domain;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = htonl(interface);
    //establish socket
    sock = socket(domain, service, protocol);
    test_connection(sock);
    // able rebinding while a previous connection is still in TIME_WAIT state, allow re-use of local address
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse_addr, sizeof(reuse_addr));
    // right now this loops endlesly
    set_non_blocking(sock);
}

//test connection virtual function
void    HTTP::simple_socket::test_connection(int item_to_test)
{
    // confirm that the socket or connection has been properly established
    if (item_to_test < 0)
    {
        perror("Failed to connect..");
        exit(EXIT_FAILURE);        
    }
}

// set non-blocking
/* IF you want to set a specific flag and leave the other flags as-is, 
then you must F_GETFL the old flags, | the new flag in, and then F_SETFL the result. 
This must be done as two separate system calls; */
void                HTTP::simple_socket::set_non_blocking(int sock)
{
    int opts;

	opts = fcntl(sock,F_GETFL);
	if (opts < 0) {
		perror("fcntl(F_GETFL)");
		exit(EXIT_FAILURE);
	}
	opts = (opts | O_NONBLOCK);
	if (fcntl(sock,F_SETFL,opts) < 0) {
		perror("fcntl(F_SETFL)");
		exit(EXIT_FAILURE);
	}
	return;
}

//getter functions
int                 HTTP::simple_socket::get_sock()
{
    return (sock);
}

struct sockaddr_in  HTTP::simple_socket::get_address()
{
    return (address);
}
