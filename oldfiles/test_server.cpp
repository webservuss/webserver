#include "test_server.hpp"

HTTP:: test_server::test_server(): simple_server(AF_INET, SOCK_STREAM, 0, 80, INADDR_ANY, 10)
{
    launch();
}
void HTTP::test_server::accepter()
{
   struct sockaddr_in  address = get_socket()->get_address();
   int addrlen = sizeof(address);
   new_socket = accept(get_socket()->get_sock(), (struct sockaddr *)&address, (socklen_t * )&addrlen);
   read(new_socket, buffer, 30000);
}
void HTTP::test_server::handeler()
{
    std::cout << buffer << std::endl;
}
void HTTP::test_server::responder()
{
	// send only accepts a void pointer
	const void *hi = "hi";
	int message_len = 2;
	send(new_socket, hi, message_len, 0 );
    close(new_socket);
}
void HTTP::test_server::launch()
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
