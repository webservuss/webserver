#include "select_server.hpp"

/* constructor calls simple_server and launches */
// HTTP::simple_server::simple_server(int domain, int service, int protocol, int port, u_long interface, int bklg)
HTTP:: select_server::select_server(): simple_server(AF_INET, SOCK_STREAM, 0, 80, INADDR_ANY, BACKLOG)
{
    launch();
}

/*copy constructor */
HTTP::select_server::select_server(const select_server& x)
    : simple_server(AF_INET, SOCK_STREAM, 0, 80, INADDR_ANY, BACKLOG)

{
    _highsock = x._highsock;
    _socks = x._socks;
	for (int i = 0; i < BACKLOG; i++)
	    _connectlist[i] = x._connectlist[i];
}

/*assignment operator */
HTTP::select_server& HTTP::select_server::operator=(const select_server& x)
{
	_highsock = x._highsock;
   	_socks = x._socks;
	for (int i = 0; i < BACKLOG; i++)
	    _connectlist[i] = x._connectlist[i];
    return *this;
}

// /*destructor */
HTTP::select_server::~select_server() {}

	
/* fd_set for select(), consist of the sock veriable in case a new, plus all already accepted sockets  */
/* FD_ZERO() clears out the fd_set called socks, so that it doesn't contain any file descriptors. */
/* FD_SET() adds the FD "sock" to the fd_set */
/* Loops through all the possible connections and adds those sockets to the fd_set */
/* select() 1st argument is the highest FD plus 1 */
/* 2nd argument is  address of the fd_set that contains sockets we're waiting to be readable (including the listening socket). */
/* 3rd argument is an fd_set that you want to know if you can write on -- we doesn't use it, so it passes 0 NULL. */
/* 4th argument is sockets you're waiting for out-of-band data for, which usually, you're not. */
/* 5th argument is a time-out of how long select() should block */
/* select() returns the number of sockets that had things going on with them -- i.e. they're readable. */
/* Once select() returns, the original fd_set has been modified so it now reflects the state of why select() woke up */
int HTTP::select_server::selecter()
{
	int 			readsocks;
    struct timeval  timeout;
    int 			sock = get_socket()->get_sock();
	
	FD_ZERO(&_socks);
	FD_SET(sock,&_socks);
    _highsock = sock;
	for (int listnum = 0; listnum < BACKLOG; listnum++) {
		if (_connectlist[listnum] != 0) {
			FD_SET(_connectlist[listnum],&_socks);
			if (_connectlist[listnum] > _highsock)
				_highsock = _connectlist[listnum];
		}
	}
	timeout.tv_sec = 5;
	timeout.tv_usec = 0;
	readsocks = select(_highsock+1, &_socks, (fd_set *) 0, (fd_set *) 0, &timeout);
	return (readsocks);
}

/* Now check connectlist for available data */
/* We have a new connection coming in!  We'll try to find a spot for it in connectlist. */
void    HTTP::select_server::accepter()
{
	int					connection;
    int					sock = get_socket()->get_sock();
    struct sockaddr_in  address = get_socket()->get_address();
    int 				addrlen = sizeof(address);

    if (FD_ISSET(sock,&_socks))
	{
		connection = accept(sock, (struct sockaddr *)&address, (socklen_t * )&addrlen);
		if (connection < 0) 
		{
			std::cout << "error in accept" << std::endl;
			exit(EXIT_FAILURE);
		}
		set_non_blocking(connection);
		for (int listnum = 0; (listnum < BACKLOG) && (connection != -1); listnum ++)
		if (_connectlist[listnum] == 0) 
		{
			std::cout << "\nConnection accepted:   FD=" << connection << "Slot" << listnum << std::endl;
			_connectlist[listnum] = connection;
			connection = -1;
		}
		if (connection != -1)
		{
        	std::cout << "\nNo room left in queue for new client.\n";
			close(connection);
		}
	}
}

/* Run through our sockets and check to see if anything happened with them, if so 'service' them. */
/* if recv fails connection closed, close this end and free up entry in connectlist */
/* else send correct to browser */
void    HTTP::select_server::handeler()
{
    int		valread;
    char	buffer[30000];
    int		sock = get_socket()->get_sock();

    // get the http
    HTTP::re_HTTP r (buffer);
	for (int listnum = 0; listnum < BACKLOG; listnum++)
	{
		if (FD_ISSET(_connectlist[listnum],&_socks))
        {
			if ((valread = recv(_connectlist[listnum], buffer, 3000, 0)) < 0) 
			{
	    		std::cout << "\nConnection lost: FD=" << _connectlist[listnum] << " Slot" << listnum << std::endl;
				close(_connectlist[listnum]);
				_connectlist[listnum] = 0;
			} 
    		//else
			//{
//				send(_connectlist[listnum] , "HTTP/1.1 200 OK\n" , 16 , 0 );
//				send(_connectlist[listnum] , "Content-length: 50\n" , 19 , 0 );
//				send(_connectlist[listnum] , "Content-Type: text/html\n\n" , 25 , 0 );
//				send(_connectlist[listnum] , "<html><body><H1> YAY SOMETHING Found!!</H1></body></html>" , 50 , 0 );
//				std::cout << "\nResponded buffer is: " << buffer << std::endl;
//			}
		}
	}
}

/* launch in an endless loop */
void    HTTP::select_server::launch()
{
    int 			readsocks;
    fd_set 			temp_fds;

	memset((char *) &_connectlist, 0, sizeof(_connectlist));
    while(true)
    {
        std::cout << "...................WAITING////" << std::endl;
 		readsocks = selecter();
		if (readsocks < 0)
		{
        	std::cout << "error select " << std::endl;
			exit(EXIT_FAILURE);
		}
		if (readsocks == 0) /* Nothing  to read, just show alive */
			std::cout << ".";
        else
		{
			accepter();
			handeler();
		}
		std::cout << "...................DONE////" << std::endl;
    }
}
