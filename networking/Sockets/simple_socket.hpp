#ifndef simple_socket_hpp
#define simple_socket_hpp

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <fcntl.h>

namespace HTTP {

    class simple_socket
    {
        private:
            int                 sock;
            struct sockaddr_in  address;

        public:
            /* constructor */
            simple_socket(int domain, int service, int protocol, int port, u_long interface);
            /* virtual function to connect to a network */
            virtual void        connect_to_network(int sock, struct sockaddr_in address) = 0;
            /* test sockets and connections */
            void                test_connection(int item_to_test);
            /* test non-blocking */
            void                set_non_blocking(int sock);
            /* getter functions */
            int                 get_sock();
            struct sockaddr_in  get_address();
    };
}

#endif
