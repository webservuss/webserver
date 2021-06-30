#include "listen_n_bind.hpp"
#include "simple_socket.hpp"

/* create simple socket, bind, check, listen, check */
HTTP::listen_n_bind::listen_n_bind(int domain, int service, int protocol, int port, u_long interface, int bklg) 
        : simple_socket(domain, service, protocol, port, interface)
{
    connect_to_network(get_sock(), get_address());
    test_connection(binding);
    backlog = bklg;
    start_listening();
    test_connection(listening);
}

/* call bind */
void    HTTP::listen_n_bind::connect_to_network(int sock, struct sockaddr_in address)
{
    binding = bind(sock, (struct sockaddr*)&address, sizeof(address));
}

/* call listen */
void    HTTP::listen_n_bind::start_listening()
{
    listening = listen(get_sock(), backlog);
}

/* getters */
int     HTTP::listen_n_bind::get_listening()    const
{
    return listening;
}

int     HTTP::listen_n_bind::get_backlog()      const
{
    return backlog;
}

int     HTTP::listen_n_bind::get_binding()      const
{
    return binding;
}

/* complian form */
/* empty constructor */
HTTP::listen_n_bind::listen_n_bind() : simple_socket(0, 0, 0, 0, 0) {binding = 0; backlog = 0; listening = 0;}

/*copy constructor */
HTTP::listen_n_bind::listen_n_bind(const listen_n_bind& x)
    : simple_socket(x.get_domain(), x.get_service(), x.get_protocol(), x.get_port(), x.get_interface())
{
    binding = x.get_binding();
    backlog = x.get_backlog();
    listening = x.get_listening();
}

/*assignment operator */
HTTP::listen_n_bind& HTTP::listen_n_bind::operator=(const listen_n_bind& x)
{
    if (binding != x.binding || backlog != x.backlog || listening != x.listening)
    {
        binding = x.binding;
        backlog = x.backlog;
        listening = x.listening;
    }
    return *this;
}

/*destructor */
HTTP::listen_n_bind::~listen_n_bind() {}
