#include "selectServer.hpp"

HDE:: selectServer::selectServer(): SimpleServer(AF_INET, SOCK_STREAM, 0, 80, INADDR_ANY, 5)
{
    launch();
}
	
/* First put together fd_set for select(), which will consist of the sock 
    veriable in case a new connection is coming in, plus all the sockets 
    we have already accepted. */
/* FD_ZERO() clears out the fd_set called socks, so that it doesn't contain any file descriptors. */
/* FD_SET() adds the file descriptor "sock" to the fd_set, so that select() will 
    return if a connection comes in on that socket (which means you have to do accept(), etc. */
/* Loops through all the possible connections and adds those sockets to the fd_set */
void HDE::selectServer::accepter()
{
    int sock = get_socket()->get_sock();
    int bklg = get_socket()->get_backlog();
	FD_ZERO(&socks);
	FD_SET(sock,&socks);
    highsock = sock;
	for (int listnum = 0; listnum < bklg; listnum++) {
		if (connectlist[listnum] != 0) {
			FD_SET(connectlist[listnum],&socks);
			if (connectlist[listnum] > highsock)
				highsock = connectlist[listnum];
		}
	}
}

void HDE::selectServer::setnonblocking(int sock)
{
	int opts;

	opts = fcntl(sock,F_GETFL);
	if (opts < 0) {
		perror("fcntl(F_GETFL)");
		exit(EXIT_FAILURE);
	}
	opts = (opts | O_NONBLOCK);
	if (fcntl(sock,F_SETFL,opts) < 0) {
		perror("fcntl(F_SETFL)");
		exit(EXIT_FAILURE);
	}
	return;
}

/* We have a new connection coming in!  We'll
try to find a spot for it in connectlist. */
void   HDE::selectServer::handle_new_connection()
{
    int sock = get_socket()->get_sock();
    int bklg = get_socket()->get_backlog();
    struct sockaddr_in  address = get_socket()->get_address();
    int addrlen = sizeof(address);

	connection = accept(sock, (struct sockaddr *)&address, (socklen_t * )&addrlen);
	if (connection < 0) {
		perror("accept");
		exit(EXIT_FAILURE);
	}
	setnonblocking(connection);
	for (int listnum = 0; (listnum < bklg) && (connection != -1); listnum ++)
		if (connectlist[listnum] == 0) {
			std::cout << "\nConnection accepted:   FD=" << connection << "Slot" << listnum << std::endl;
			connectlist[listnum] = connection;
			connection = -1;
		}
	if (connection != -1) { /* No room left in the queue! */
        std::cout << "\nNo room left for new client.\n";
		close(connection);
	}
}

	/* Now check connectlist for available data */
	/* Run through our sockets and check to see if anything
		happened with them, if so 'service' them. */
void    HDE::selectServer::handeler()
{
    int sock = get_socket()->get_sock();
    int bklg = get_socket()->get_backlog();
    if (FD_ISSET(sock,&socks))
		handle_new_connection();
	for (int listnum = 0; listnum < bklg; listnum++) {
		if (FD_ISSET(connectlist[listnum],&socks))
            responder(listnum);
	}
}

void    HDE::selectServer::responder(int listnum)
{
    int valread;
    int sock = get_socket()->get_sock();
    if ((valread = recv(connectlist[listnum], buffer, 3000, 0)) < 0) {
	    /* Connection closed, close this end and free up entry in connectlist */
	    std::cout << "\nConnection lost: FD=" << connectlist[listnum] << " Slot" << listnum << std::endl;
	    close(connectlist[listnum]);
		connectlist[listnum] = 0;
	    } 
    else {
        buffer[valread] = '\0';  
		send(connectlist[listnum] , "HTTP/1.1 200 OK\n" , 16 , 0 );  
		send(connectlist[listnum] , "Content-length: 50\n" , 19 , 0 );  
		send(connectlist[listnum] , "Content-Type: text/html\n\n" , 25 , 0 );  
		send(connectlist[listnum] , "<html><body><H1> YAY SOMETHING Found</H1></body></html>" , 50 , 0 );  
		std::cout << "\nResponded buffer is: " << buffer << std::endl;
	}
}


		/* The first argument to select is the highest file
			descriptor value plus 1. In most cases, you can
			just pass FD_SETSIZE and you'll be fine. */
			
		/* The second argument to select() is the address of
			the fd_set that contains sockets we're waiting
			to be readable (including the listening socket). */
			
		/* The third parameter is an fd_set that you want to
			know if you can write on -- this example doesn't
			use it, so it passes 0, or NULL. The fourth parameter
			is sockets you're waiting for out-of-band data for,
			which usually, you're not. */
		
		/* The last parameter to select() is a time-out of how
			long select() should block. If you want to wait forever
			until something happens on a socket, you'll probably
			want to pass NULL. */
            		/* select() returns the number of sockets that had
			things going on with them -- i.e. they're readable. */
			
		/* Once select() returns, the original fd_set has been
			modified so it now reflects the state of why select()
			woke up. i.e. If file descriptor 4 was originally in
			the fd_set, and then it became readable, the fd_set
			contains file descriptor 4 in it. */
void    HDE::selectServer::launch()
{
    int readsocks;
    int sock = get_socket()->get_sock();
    fd_set temp_fds;

    highsock = sock;
	memset((char *) &connectlist, 0, sizeof(connectlist));
    while(true)
    {
        std::cout << "...................WAITING////" << std::endl;
 		accepter();
		timeout.tv_sec = 5;
		timeout.tv_usec = 0;
		readsocks = select(highsock+1, &socks, (fd_set *) 0, (fd_set *) 0, &timeout);
		if (readsocks < 0) {
        	std::cout << "error select " << std::endl;
			exit(EXIT_FAILURE);
		}
		if (readsocks == 0) { /* Nothing  to read, just show alive */
			std::cout << ".";
			// fflush(stdout);
		} 
        else
			handeler();
        std::cout << "...................DONE////" << std::endl;
    }
}
