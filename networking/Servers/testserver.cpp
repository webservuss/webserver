#include "testserver.hpp"
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
        // char * msg= "GET / HTTP/1.1\r\n"
        //         "Host: 192.241.213.46:6880\r\n"
        //         "Upgrade-Insecure-Requests: 1\r\n"
        //         "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
        //         "User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_12_3) AppleWebKit/602.4.8 (KHTML, like Gecko) Version/10.0.3 Safari/602.4.8\r\n"
        //         "Accept-Language: en-us\r\n"
        //         "Accept-Encoding: gzip, deflate\r\n"
        //         "Connection: keep-alive\r\n\r\n";
    // write(new_socket, msg, strlen(msg));
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