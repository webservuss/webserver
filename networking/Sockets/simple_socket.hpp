#ifndef simple_socket_hpp
#define simple_socket_hpp

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <fcntl.h>
#include "../../http_funct.hpp"

namespace HTTP {
    class simple_socket
    {
        private:
            int                 sock;
            struct sockaddr_in  address;
            int                 domain;
            int                 service;
            int                 protocol;
            int                 port;
            u_long              interface;
            /* empty constructor for complian form */
            // simple_socket();
        public:
            /* constructors */
            simple_socket(int domain, int service, int protocol, int port, u_long interface);
            /*copy constructor */
            simple_socket(const simple_socket& x);
            /*assignment operator */
            simple_socket& operator=(const simple_socket& x);
            /*destructor */
            ~simple_socket();

            /* virtual function to connect to a network */
            virtual void        connect_to_network(int sock, struct sockaddr_in address) = 0;
            /* test sockets and connections */
            void                test_connection(int item_to_test);

            /* getter functions */
            int                 get_sock()      const;
            struct sockaddr_in  get_address()   const;
            int                 get_domain()    const;
            int                 get_service()   const;
            int                 get_protocol()  const;
            int                 get_port()      const;
            u_long              get_interface() const;
    };
}

#endif