#include "listeningSocket.hpp"


HDE::listentingSocket::listentingSocket(int domain , int service, int protocol, int port, 
        u_long interface,  int bklg) : bindingSocket(domain, service, protocol, port, interface)
{
    backlog = bklg;
    start_listening();
    test_connection(listening);
}


void HDE::listentingSocket::start_listening()
{
    listening = listen(get_sock(), backlog);
}

int    HDE::listentingSocket::get_listening()
{
    return listening;
}

int  HDE::listentingSocket::get_backlog()
{
    return backlog;
}
