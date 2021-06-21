#include "listeningSocket.hpp"


/* ListeningSocket is a child of BindingSocket, specifying that once bound, the socket is to listen o the nework
on the given port for incoming connections  */


/* CONSTRUCTORS*/

HDE::listentingSocket::listentingSocket(int domain , int service,
int protocol, int port, u_long interface,  int bklg) : 
bindingSocket(domain , service,  protocol, port,  interface)
{
    /* set backlog variable*/
    backlog = bklg;
    /* start listeing to the network using listen() from sys socket.h*/
    start_listening();
    /* confirm the connection was succesful*/
    //std::cout << "3" << std::endl;
    test_connection(listening);
}

/* MUTATORS*/

void HDE::listentingSocket::start_listening()
{
        //std::cout << "1" << std::endl;
        //start listening on the network
        //std::cout << get_connection() << std::endl;
        /* needs a file descriptor with get_sock */
        listening = listen(get_sock(), backlog);
        //std::cout << "2" << std::endl;
}

/* GETTERS*/


int HDE::listentingSocket::get_listening()
{
    return listening;
}

int HDE::listentingSocket::get_backlog()
{
    return backlog;
}