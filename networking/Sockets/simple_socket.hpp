#ifndef simple_socket_hpp
#define simple_socket_hpp

#include <netinet/in.h>
#include <iostream>
#include <vector>
#include <netinet/in.h>
#include <sys/time.h>

namespace HTTP {
    class simple_socket
    {
        protected:
            int                 _sock;
        private:
            struct sockaddr_in  _address;
            int                 _domain;
            int                 _service;
            int                 _protocol;
            int                 _port;
            u_long              _interface;
        public:
		    class	socket_error_ex : public std::exception {
		    public:
		        virtual const char*	what() const throw();
		    };
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
