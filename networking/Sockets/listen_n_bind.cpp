#include "listen_n_bind.hpp"

/* create simple socket, bind, check, listen, check */
HTTP::listen_n_bind::listen_n_bind(int domain, int service, int protocol, int port, u_long interface, int bklg) 
        : simple_socket(domain, service, protocol, port, interface)
{
    connect_to_network(get_sock(), get_address());
    try{
        test_connection(_binding);
    }
        catch(std::exception &e ){
            std::cerr << e.what() << std::endl;
            error_exit("connection fails", 1);
    }
    _backlog = bklg;
    start_listening();
    try{
        test_connection(_listening);
    }
    catch(std::exception &e ){
        std::cerr << e.what() << std::endl;
        error_exit("connection fails", 1);
    }
    
}

/* copy constructor */
HTTP::listen_n_bind::listen_n_bind(const listen_n_bind& x)
    : simple_socket(x.get_domain(), x.get_service(), x.get_protocol(), x.get_port(), x.get_interface())
{
    _binding = x.get_binding();
    _backlog = x.get_backlog();
    _listening = x.get_listening();
}

/* assignment operator */
HTTP::listen_n_bind& HTTP::listen_n_bind::operator=(const listen_n_bind& x)
{
    if (_binding != x._binding || _backlog != x._backlog || _listening != x._listening)
    {
        _binding = x._binding;
        _backlog = x._backlog;
        _listening = x._listening;
    }
    return *this;
}

/* destructor */
HTTP::listen_n_bind::~listen_n_bind() {}

/* call bind */
void    HTTP::listen_n_bind::connect_to_network(int sock, struct sockaddr_in address)
{
    _binding = bind(sock, (struct sockaddr*)&address, sizeof(address));
}

/* call listen */
void    HTTP::listen_n_bind::start_listening()
{
    _listening = listen(get_sock(), _backlog);
}

/* getters */
int     HTTP::listen_n_bind::get_listening()    const
{
    return _listening;
}

int     HTTP::listen_n_bind::get_backlog()      const
{
    return _backlog;
}

int     HTTP::listen_n_bind::get_binding()      const
{
    return _binding;
}
