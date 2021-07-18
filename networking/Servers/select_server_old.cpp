#include "select_server.hpp"

/* constructor calls simple_server and launches */
HTTP:: select_server::select_server()
{
    _socket = new HTTP::listen_n_bind(AF_INET, SOCK_STREAM, 0, 80, INADDR_ANY, BACKLOG);
	_sockets[0] = get_socket()->get_sock();
	launch(1);
}

// /* constructor calls simple_server and launches with correct port */
// HTTP:: select_server::select_server(int port): simple_server(AF_INET, SOCK_STREAM, 0, port, INADDR_ANY, BACKLOG)
// {
//     launch();
// }

// /* constructor calls simple_server and launches with multiplecorrect port */
HTTP:: select_server::select_server(int *port, int numb_ports)
{
	for (int i = 0; i < numb_ports; i++)
	{
		_socket = new HTTP::listen_n_bind(AF_INET, SOCK_STREAM, 0, port[i], INADDR_ANY, BACKLOG);
		_sockets[i] = get_socket()->get_sock();
	}
    launch(numb_ports);
}

/*copy constructor */
HTTP::select_server::select_server(const select_server& x)
    // : simple_server(AF_INET, SOCK_STREAM, 0, 80, INADDR_ANY, BACKLOG)
{
    _highsock = x._highsock;
    _read_fds = x._read_fds;
	for (int i = 0; i < BACKLOG; i++)
	    _connectlist[i] = x._connectlist[i];
}

/*assignment operator */
HTTP::select_server& HTTP::select_server::operator=(const select_server& x)
{
	_highsock = x._highsock;
   	_read_fds = x._read_fds;
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
int HTTP::select_server::selecter(int numb)
{
	int 			readsocks;
    struct timeval  timeout;
	
	FD_ZERO(&_read_fds);
	// FD_SET(_sockets[numb],&_read_fds);
	// for (int i = 0; i < numb; i++)
	// 	FD_SET(_sockets[i],&_read_fds);
    _highsock = _sockets[numb];
	for (int listnum = 0; listnum < BACKLOG; listnum++) {
		if (_connectlist[listnum] != 0) {
			// FD_SET(_connectlist[listnum],&_read_fds);
			if (_connectlist[listnum] > _highsock)
				_highsock = _connectlist[listnum];
		}
	}
	timeout.tv_sec = 5;
	timeout.tv_usec = 0;
	readsocks = select(_highsock+1, &_read_fds, (fd_set *) 0, (fd_set *) 0, &timeout);
	return (readsocks);
}

/* Now check connectlist for available data */
/* We have a new connection coming in!  We'll try to find a spot for it in connectlist. */
void    HTTP::select_server::accepter(int numb)
{
	int					connection;
    struct sockaddr_in  address = get_socket()->get_address();
    int 				addrlen = sizeof(address);

    if (FD_ISSET(_sockets[numb],&_read_fds))
	{
		connection = accept(_sockets[numb], (struct sockaddr *)&address, (socklen_t * )&addrlen);
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

	for (int listnum = 0; listnum < BACKLOG; listnum++)
	{
		if (FD_ISSET(_connectlist[listnum],&_read_fds))
        {
			if ((valread = recv(_connectlist[listnum], buffer, 3000, 0)) < 0) 
			{
	    		std::cout << "\nConnection lost: FD=" << _connectlist[listnum] << " Slot" << listnum << std::endl;
				close(_connectlist[listnum]);
				_connectlist[listnum] = 0;
			} 
    		else 
			{
				send(_connectlist[listnum] , "HTTP/1.1 200 OK\n" , 16 , 0 );  
				send(_connectlist[listnum] , "Content-length: 50\n" , 19 , 0 );  
				send(_connectlist[listnum] , "Content-Type: text/html\n\n" , 25 , 0 );  
				send(_connectlist[listnum] , "<html><body><H1> YAY SOMETHING Found!!</H1></body></html>" , 50 , 0 );  
				std::cout << "\nResponded buffer is: " << buffer << std::endl;
			}
		}
	}
}

/* launch in an endless loop */
void    HTTP::select_server::launch(int numb_ports)
{
    int 			readsocks;
	int				numb;
    fd_set 			temp_fds;

	memset((char *) &_connectlist, 0, sizeof(_connectlist));
	numb = numb_ports;
    while(true)
    {
		if (numb == numb_ports)
			numb = 0;
        std::cout << "...................WAITING////" << std::endl;
 		readsocks = selecter(numb);
		if (readsocks < 0)
		{
        	std::cout << "error select " << std::endl;
			exit(EXIT_FAILURE);
		}
		if (readsocks == 0) /* Nothing  to read, just show alive */
			std::cout << ".";
        else
		{
			accepter(numb);
			handeler();
		}
		std::cout << "...................DONE////" << std::endl;
		numb++;
    }
}

/* getter */
HTTP::listen_n_bind * HTTP::select_server::get_socket()
{
    return _socket;
}

// #include "select_server.hpp"

// /* constructor calls simple_server and launches */
// HTTP:: select_server::select_server()
// {
//     _socket = new HTTP::listen_n_bind(AF_INET, SOCK_STREAM, 0, 80, INADDR_ANY, BACKLOG);
// 	_sockets[0] = get_socket()->get_sock();
// 	launch(1);
// }

// // /* constructor calls simple_server and launches with correct port */
// // HTTP:: select_server::select_server(int port): simple_server(AF_INET, SOCK_STREAM, 0, port, INADDR_ANY, BACKLOG)
// // {
// //     launch();
// // }

// // /* constructor calls simple_server and launches with multiplecorrect port */
// HTTP:: select_server::select_server(int *port, int numb_ports)
// {
// 	for (int i = 0; i < numb_ports; i++)
// 	{
// 		_socket = new HTTP::listen_n_bind(AF_INET, SOCK_STREAM, 0, port[i], INADDR_ANY, BACKLOG);
// 		_sockets[i] = get_socket()->get_sock();
// 	}
//     launch(numb_ports);
// }

// /*copy constructor */
// HTTP::select_server::select_server(const select_server& x)
//     // : simple_server(AF_INET, SOCK_STREAM, 0, 80, INADDR_ANY, BACKLOG)
// {
//     _highsock = x._highsock;
//     _read_fds = x._read_fds;
// 	for (int i = 0; i < BACKLOG; i++)
// 	    _connectlist[i] = x._connectlist[i];
// }

// /*assignment operator */
// HTTP::select_server& HTTP::select_server::operator=(const select_server& x)
// {
// 	_highsock = x._highsock;
//    	_read_fds = x._read_fds;
// 	for (int i = 0; i < BACKLOG; i++)
// 	    _connectlist[i] = x._connectlist[i];
//     return *this;
// }

// // /*destructor */
// HTTP::select_server::~select_server() {}

	
// /* fd_set for select(), consist of the sock veriable in case a new, plus all already accepted sockets  */
// /* FD_ZERO() clears out the fd_set called socks, so that it doesn't contain any file descriptors. */
// /* FD_SET() adds the FD "sock" to the fd_set */
// /* Loops through all the possible connections and adds those sockets to the fd_set */
// /* select() 1st argument is the highest FD plus 1 */
// /* 2nd argument is  address of the fd_set that contains sockets we're waiting to be readable (including the listening socket). */
// /* 3rd argument is an fd_set that you want to know if you can write on -- we doesn't use it, so it passes 0 NULL. */
// /* 4th argument is sockets you're waiting for out-of-band data for, which usually, you're not. */
// /* 5th argument is a time-out of how long select() should block */
// /* select() returns the number of sockets that had things going on with them -- i.e. they're readable. */
// /* Once select() returns, the original fd_set has been modified so it now reflects the state of why select() woke up */
// // int HTTP::select_server::selecter(int numb)
// // {
// 	// int 			readsocks;
//     // struct timeval  timeout;
	
// 	// FD_ZERO(&_read_fds);
// 	// // FD_SET(_sockets[numb],&_read_fds);
// 	// for (int i = 0; i < numb; i++)
// 	// 	FD_SET(_sockets[i],&_read_fds);
//     // _highsock = _sockets[numb];
// 	// for (int listnum = 0; listnum < BACKLOG; listnum++) {
// 	// 	if (_connectlist[listnum] != 0) {
// 	// 		// FD_SET(_connectlist[listnum],&_read_fds);
// 	// 		if (_connectlist[listnum] > _highsock)
// 	// 			_highsock = _connectlist[listnum];
// 	// 	}
// 	// }
// 	// timeout.tv_sec = 5;
// 	// timeout.tv_usec = 0;
// 	// readsocks = select(_highsock+1, &_read_fds, (fd_set *) 0, (fd_set *) 0, &timeout);
// 	// return (readsocks);
// // }

// /* Now check connectlist for available data */
// /* We have a new connection coming in!  We'll try to find a spot for it in connectlist. */
// // void    HTTP::select_server::accepter(int numb)
// int    HTTP::select_server::accept_new_connection(int server_socket)
// {
// 	int addrlen = sizeof(struct sockaddr_in);
// 	int client_socket;
// 	struct sockaddr_in address;
// 	client_socket = accept(server_socket, (struct sockaddr *)&address, (socklen_t * )&addrlen);


// 	// if (client_socket == (accept(server_socket, (struct sockaddr *)&address, (socklen_t * )&addrlen)) < 0) 
// 	// {
// 	// 	std::cout << "error in accept" << std::endl;
// 	// 	exit(EXIT_FAILURE);
// 	// }
// 	return (client_socket);


// 	// int					connection;
//     // struct sockaddr_in  address = get_socket()->get_address();
//     // int 				addrlen = sizeof(address);

//     // if (FD_ISSET(_sockets[numb],&_read_fds))
// 	// {
// 	// 	connection = accept(_sockets[numb], (struct sockaddr *)&address, (socklen_t * )&addrlen);
// 	// 	if (connection < 0) 
// 	// 	{
// 	// 		std::cout << "error in accept" << std::endl;
// 	// 		exit(EXIT_FAILURE);
// 	// 	}
// 	// 	set_non_blocking(connection);
// 	// 	for (int listnum = 0; (listnum < BACKLOG) && (connection != -1); listnum ++)
// 	// 	if (_connectlist[listnum] == 0) 
// 	// 	{
// 	// 		std::cout << "\nConnection accepted:   FD=" << connection << "Slot" << listnum << std::endl;
// 	// 		_connectlist[listnum] = connection;
// 	// 		connection = -1;
// 	// 	}
// 	// 	if (connection != -1)
// 	// 	{
//     //     	std::cout << "\nNo room left in queue for new client.\n";
// 	// 		close(connection);
// 	// 	}
// 	// }
// }

// /* Run through our sockets and check to see if anything happened with them, if so 'service' them. */
// /* if recv fails connection closed, close this end and free up entry in connectlist */
// /* else send correct to browser */
// void    HTTP::select_server::handeler(int client_socket)
// {
// 	char	buffer[30000];
// 	size_t bytes_read;
// 	int msgsize = 0;
// 	char actualpath[30000];

// 	if ((bytes_read = recv(client_socket, buffer, 3000, 0)) < 0) 
// 	{
// 		std::cout << "error recv" << std::endl;
// 	} 
// 	std::cout << "request: " << buffer << std::endl;
// 	send(client_socket , "HTTP/1.1 200 OK\n" , 16 , 0 );  
// 	send(client_socket , "Content-length: 50\n" , 19 , 0 );  
// 	send(client_socket , "Content-Type: text/html\n\n" , 25 , 0 );  
// 	send(client_socket , "<html><body><H1> YAY SOMETHING Found!!</H1></body></html>" , 50 , 0 );
// 	close(client_socket);
// 	return;
// 	// read the clients message -- the name of the file to read

//     // int		valread;
//     // char	buffer[30000];

// 	// for (int listnum = 0; listnum < BACKLOG; listnum++)
// 	// {
// 	// 	if (FD_ISSET(_connectlist[listnum],&_read_fds))
//     //     {
// 	// 		if ((valread = recv(_connectlist[listnum], buffer, 3000, 0)) < 0) 
// 	// 		{
// 	//     		std::cout << "\nConnection lost: FD=" << _connectlist[listnum] << " Slot" << listnum << std::endl;
// 	// 			close(_connectlist[listnum]);
// 	// 			_connectlist[listnum] = 0;
// 	// 		} 
//     // 		else 
// 	// 		{
// 	// 			send(_connectlist[listnum] , "HTTP/1.1 200 OK\n" , 16 , 0 );  
// 	// 			send(_connectlist[listnum] , "Content-length: 50\n" , 19 , 0 );  
// 	// 			send(_connectlist[listnum] , "Content-Type: text/html\n\n" , 25 , 0 );  
// 	// 			send(_connectlist[listnum] , "<html><body><H1> YAY SOMETHING Found!!</H1></body></html>" , 50 , 0 );  
// 	// 			std::cout << "\nResponded buffer is: " << buffer << std::endl;
// 	// 		}
// 	// 	}
// 	// }
// }

// /* launch in an endless loop */
// void    HTTP::select_server::launch(int numb_ports)
// {
//     int 			readsocks;
// 	int				numb;
//     fd_set 			backup_read_fds;
//     fd_set 			backup_write_fds;

// 	FD_ZERO(&backup_read_fds);
// 	FD_ZERO(&backup_write_fds);
// 	for (int i = 0; i < numb; i++)
// 		FD_SET(_sockets[i], &backup_read_fds);
//     while(true)
//     {
// 		int check = 0;
// 		// because select is destructive
// 		_read_fds = backup_read_fds;
// 		_write_fds = backup_write_fds;
// 		// if (numb == numb_ports)
// 		// 	numb = 0;
//         std::cout << "...................WAITING////" << std::endl;

// 		if (select(FD_SETSIZE, &_read_fds, &_write_fds, NULL, NULL) < 0)
// 		{
// 			std::cout << "error in select" << std::endl;
// 			exit(1);
// 		}
// 		// when select returns the _read_fds contains only the fds that are ready for reading 
// 		// this is why we make a copy
// 		// for (int i = 0; i < FD_SETSIZE; i++)
// 		// { // check if in
// 			// if (FD_ISSET(i, &_read_fds)) {
// 			// 		if (i == _sockets[i]) { // this is a new connection
// 			// 			int client_socket = accept_new_connection(_sockets[0]);
// 			// 			// add newly accepted socket, the new client connection, to the set of
// 			// 			// sockets that we are watching
// 			// 			FD_SET(client_socket, &backup_read_fds);
// 			// 		}
// 			// 	else // socket ready to read from is one of those client sockets
// 			// 	{ // just want to read its data and handle connection
// 			// 		handeler(i);
// 			// 		// once handled remove socket from list of FDs watching
// 			// 		FD_CLR(i, &backup_read_fds);
// 			// 	}
// 			// }
// 		// }
// 		for (int i = 0; i < numb_ports; i++)
// 		{
// 			if (FD_ISSET(_sockets[i], &_read_fds)) {
// 				printf("request on port %d", i);
// 				handeler(i); }
// 		}
// 		for (int i = 0; i < numb_ports; i++) close(_sockets[i]);
// 	}
// 	// int 			readsocks;
	
// 	// for (int i = 0; i < numb; i++)
// 	// 	FD_SET(_sockets[i],&_read_fds);
//     // _highsock = _sockets[numb];
// 	// for (int listnum = 0; listnum < BACKLOG; listnum++) {
// 	// 	if (_connectlist[listnum] != 0) {
// 	// 		// FD_SET(_connectlist[listnum],&_read_fds);
// 	// 		if (_connectlist[listnum] > _highsock)
// 	// 			_highsock = _connectlist[listnum];
// 	// 	}
// 	// }
// 	// readsocks = select(_highsock+1, &_read_fds, (fd_set *) 0, (fd_set *) 0, &timeout);
// 	// return (readsocks);


// 	// 	if (readsocks < 0)
// 	// 	{
//     //     	std::cout << "error select " << std::endl;
// 	// 		exit(EXIT_FAILURE);
// 	// 	}
// 	// 	if (readsocks == 0) /* Nothing  to read, just show alive */
// 	// 		std::cout << ".";
//     //     else
// 	// 	{
// 	// 		accepter(numb);
// 	// 		handeler();
// 	// 	}
// 	// 	std::cout << "...................DONE////" << std::endl;
// 	// 	// numb++;
//     // }
// }

// /* getter */
// HTTP::listen_n_bind * HTTP::select_server::get_socket()
// {
//     return _socket;
// }





