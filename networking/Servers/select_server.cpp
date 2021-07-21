#include "select_server.hpp"

/* constructor calls simple_server and launches */
HTTP:: select_server::select_server()
{
	listen_n_bind * socket = new HTTP::listen_n_bind(AF_INET, SOCK_STREAM, 0, 80, INADDR_ANY, BACKLOG);
	_servers_socket.push_back(socket->get_sock());
	_servers_addr.push_back(socket->get_address());
	launch();
}

HTTP:: select_server::select_server(std::vector<int> ports)
{
	for (int i = 0; i < ports.size(); i++)
	{
		listen_n_bind * socket = new HTTP::listen_n_bind(AF_INET, SOCK_STREAM, 0, ports[i], INADDR_ANY, BACKLOG);
		std::cout << "creating port " << ports[i] << std::endl;
		_servers_addr.push_back(socket->get_address());
		_servers_socket.push_back(socket->get_sock());
	}
	launch();
}


/*copy constructor */
HTTP::select_server::select_server(const select_server& x)
{
    // _socket = new HTTP::listen_n_bind(AF_INET, SOCK_STREAM, 0, 80, INADDR_ANY, BACKLOG);
	// _highsock = x._highsock;
    // _read_fds = x._read_fds;
	// for (int i = 0; i < BACKLOG; i++)
	//     _connectlist[i] = x._connectlist[i];
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
int HTTP::select_server::selecter()
{
	int 			readsocks;
    struct timeval  timeout;
    // int 			sock = _socket->get_sock();
	
    std::cout << "in selecter " << std::endl;
	std::cout << std::endl;
	timeout.tv_sec = 5;
	timeout.tv_usec = 0;
	readsocks = select(FD_SETSIZE, &_read_fds, &_write_fds, (fd_set *) 0, &timeout);
	if (readsocks < 0)
	{
       	std::cout << "error select " << std::endl;
		exit(EXIT_FAILURE);
	}
    std::cout << "out selecter" << std::endl;
	return (readsocks);
}

/* Now check connectlist for available data */
/* We have a new connection coming in!  We'll try to find a spot for it in connectlist. */
void    HTTP::select_server::accepter(int i)
{
        std::cout << "in accepter " << std::endl;
    	int					    connection;
		sockaddr_in				addr = _servers_addr[i];
    	int 					addrlen = sizeof(_servers_addr[i]);
        connection = accept(_servers_socket[i], (struct sockaddr *)&addr, (socklen_t * )&addrlen);
        if (connection < 0) 
		{
   			printf("Oh dear, something went wrong! %s\n", strerror(errno));
			std::cout << "error in accept" << std::endl;
			exit(EXIT_FAILURE);
		}
		set_non_blocking(connection);
        if (std::find(_client_socket.begin(), _client_socket.end(), connection) == _client_socket.end()) {
        // someName not in name, add it
            _client_socket.push_back(connection);
            _client_addr.push_back(addr);
        }
        // _client_socket.push_back(connection);
        // _client_addr.push_back(addr);
        std::cout << "NEW CLIENT: " << connection << std::endl;
		// for (int listnum = 0; (listnum < BACKLOG) && (connection != -1); listnum ++)
		// if (_connectlist[listnum] == 0) 
		// {
		// 	std::cout << "\nConnection accepted:   FD=" << connection << "Slot" << listnum << std::endl;
		// 	_connectlist[listnum] = connection;
		// 	connection = -1;
		// }
		// if (connection != -1)
		// {
        // 	std::cout << "\nNo room left in queue for new client.\n";
		// 	close(connection);
		// }
        // FD_SET(_servers_socket[i], &_read_backup);
		FD_SET(connection,&_read_fds);
		// FD_SET(connection,&_write_backup);
        // FD_SET(connection, &_read_fds);
        std::cout << "out accepter" << std::endl;
}

/* Run through our sockets and check to see if anything happened with them, if so 'service' them. */
/* if recv fails connection closed, close this end and free up entry in connectlist */
/* else send correct to browser */
void    HTTP::select_server::read_from_client(int i)
{
    int		valread;
    char	buffer[30000];

    std::cout << "in read_from_client " << std::endl;
	if ((valread = recv(_client_socket[i], buffer, 3000, 0)) < 0) 
	{
		std::cout << "\nConnection lost: FD=" << _client_socket[i] << " Slot" << i << std::endl;
		printf("Oh dear, something went wrong! %s\n", strerror(errno));
		close(_client_socket[i]);
		_client_socket[i] = 0;
	}
	std::cout << "\nResponded buffer is: " << buffer << std::endl;
    FD_SET(_client_socket[i], &_write_fds);
            // FD_SET(_client_socket[i], &_write_backup);
}

void HTTP::select_server::send_response(int i)
{
    std::cout << "in send response" << std::endl;
	send(_client_socket[i] , "HTTP/1.1 200 OK\n" , 16 , 0 );  
	send(_client_socket[i] , "Content-length: 50\n" , 19 , 0 );  
	send(_client_socket[i] , "Content-Type: text/html\n\n" , 25 , 0 );  
	send(_client_socket[i] , "<html><body><H1> YAY SOMETHING Found!!</H1></body></html>" , 50 , 0 );  
    std::cout << " response mid" << std::endl;
    // FD_SET(_client_socket[i], &_write_backup);
    std::cout << "out send response" << std::endl;
    FD_CLR(_client_socket[i], &_write_backup);
}

/* launch in an endless loop */
void    HTTP::select_server::launch()
{
    int 			readsocks;

    FD_ZERO(&_read_backup);
    FD_ZERO(&_write_backup);
    // add servers to read_backup
	for (int listnum = 0; listnum < _servers_socket.size(); listnum++) {
		if (_servers_socket[listnum] != 0) {
			FD_SET(_servers_socket[listnum],&_read_backup);
			if (_servers_socket[listnum] > _highsock)
				_highsock = _servers_socket[listnum];
		}
	}
    _client_socket.clear();
    _client_addr.clear();
    while(true)
    {
        _read_fds = _read_backup;
        _write_fds = _write_backup;
        std::cout << "...................WAITING////" << std::endl;
        selecter();
	    for (int i = 0; i < _servers_socket.size(); i++) 
        {
            if (FD_ISSET(_servers_socket[i], &_read_fds)) {
                accepter(i);
            }
            for (int j = 0; j < _client_socket.size(); j++) {
                std::cout << "in for loop " << j << std::endl;
                if (FD_ISSET(_client_socket[j], &_read_fds)) {
                    read_from_client(j);
                }
                if (FD_ISSET(_client_socket[j], &_write_fds)) {
                    // parse_request();
                    send_response(j);
                }
            }
		}
		std::cout << "...................DONE////" << std::endl;
    }
}


// while (true) {
//     readFds = readFdsBackup;
//     writeFds = writeFdsBackup;
// ​
//     if (select(maxFD, &readFds, &writeFds, 0, 0) == -1)
//         throw std::runtime_error("Select failed");
// ​
//     for (server in servers) {
//         if (FD_ISSET(server->socketFD, &readFds)) {
//             add_client();
//             FD_SET(clientfd, &readFdsBackup);
//         }
//         for (client in server->clients()) {
//             if (FD_ISSET(client->fd, &readFds)) {
//                 read_from_client();
//                 if (done_reading())
//                     FD_SET(client->fd, &writeFdsBackup);
//             }
// ​
//             if (FD_ISSET(client->fd, &writeFds)) {
//                 parse_request();
//                 send_response();
//                 FD_CLR(client->fd, &writeFdsBackup);
//             }
//         }
//     }
// }