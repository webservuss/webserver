#include "SimpleServer.hpp"


HDE::SimpleServer::SimpleServer(int domain, int service, int protocol, int port, u_long interface, int bklg){

    /* new pointer watch out for memory leak*/
    socket = new listentingSocket(domain, service, protocol, port, interface, bklg);
    //delete socket;
}

HDE::listentingSocket * HDE::SimpleServer::get_socket()
{
    return socket;
}