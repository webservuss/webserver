#ifndef simpleSocket_hpp
#define simpleSocket_hpp

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>

namespace HDE {

    class simpleSocket
    {
        public:
            // constructor
            simpleSocket(int domain, int service, int protocol, int port, u_long interface);
            // virtual function to connect to a network
            virtual void         connect_to_network(int sock, struct sockaddr_in address) = 0;
            // function to test sockets and connections
            void                test_connection(int item_to_test);
            // getter functions
            int                 get_sock();
            //int                 get_connection();
            struct sockaddr_in  get_address();
            // setter function
            //void                set_connection(int con);
        private:
            int                 sock;
           //int                 connection;
            struct sockaddr_in  address;
    };
}

#endif
