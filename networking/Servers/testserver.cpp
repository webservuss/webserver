#include "TestServer.hpp"
#include <unistd.h>



HDE:: TestServer::TestServer(): SimpleServer(AF_INET, SOCK_STREAM, 0, 80, INADDR_ANY, 10)
{
    launch();
}


void HDE::TestServer::accepter()
{
   struct sockaddr_in  address = get_socket()->get_address();
   int addrlen = sizeof(address);
   new_socket = accept(get_socket()->get_sock(), (struct sockaddr *)&address, (socklen_t * )&addrlen);
   read(new_socket, buffer, 30000);

}

void HDE::TestServer::handeler()
{
    std::cout << buffer << std::endl;
}

void HDE::TestServer::responder()
{
    char  *hello = "hello from server";
    write(new_socket, hello, strlen(hello));
    close(new_socket);
}

void HDE::TestServer::launch()
{
    while(true)
    {
        std::cout << "...................WAITING////" << std::endl;
        accepter();
        handeler();
        responder();
        std::cout << "...................DONE////" << std::endl;


    }
}