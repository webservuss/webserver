#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "select_server.hpp"
#include "../Request/re_HTTP.hpp"
#include "../utils/http_funct.hpp"
#include "../utils/req_n_conf.hpp"
#include "../Respond/respond.hpp"

#define BUFFER_SIZE (1024 * 1024) // 1Mb
// /* constructor calls simple_server and launches */ // need to add in parser_servers here too
// HTTP:: select_server::select_server()
// {
// 	listen_n_bind * socket = new HTTP::listen_n_bind(AF_INET, SOCK_STREAM, 0, 80, INADDR_ANY, BACKLOG);
// 	t_server_select newserver;
// 	set_value_server_select_server(socket->get_sock(), socket->get_address(), newserver);
// 	_servers.push_back(newserver);
// 	launch();
// }

HTTP:: select_server::select_server(std::vector<int> ports, std::vector<t_server> parser_servers)
{
	_parser_servers = parser_servers;
	for (unsigned long i = 0; i < ports.size(); i++)
	{
		listen_n_bind socket = HTTP::listen_n_bind(AF_INET, SOCK_STREAM, 0, ports[i], INADDR_ANY, BACKLOG);
		t_server_select newserver;
		set_value_server_select_server(socket.get_sock(), socket.get_address(), ports[i], newserver);
		_servers.push_back(newserver);
	}
	launch();
}

/*copy constructor */
HTTP::select_server::select_server(const select_server& x)
{ // TEST THIS // change
	_highsock = x._highsock;
   	_read_fds = x._read_fds;
    _write_fds = x._write_fds;
    _write_backup = x._write_backup;
    _read_backup = x._read_backup;
    _servers = x._servers;
}

/*assignment operator */
HTTP::select_server& HTTP::select_server::operator=(const select_server& x)
{ // TEST THIS
	_highsock = x._highsock;
   	_read_fds = x._read_fds;
    _write_fds = x._write_fds;
    _write_backup = x._write_backup;
    _read_backup = x._read_backup;
    _servers = x._servers;
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
    	int					    connection;
		sockaddr_in				addr;
    	int 					addrlen;

		addr = _servers[i]._servers_addr;
		addrlen = sizeof(_servers[i]._servers_addr);
        connection = accept(_servers[i]._servers_socket, (struct sockaddr *)&addr, (socklen_t * )&addrlen);
        if (connection < 0) 
		{
			std::cout << "error in accept" << strerror(errno) << std::endl;
			exit(EXIT_FAILURE);
		}
		set_non_blocking(connection);
		make_client(connection, addr, _servers[i]);
        std::cout << "NEW CLIENT: " << connection << std::endl;
		FD_SET(connection,&_read_backup);
        std::cout << "out accepter" << std::endl;
}

/* Run through our sockets and check to see if anything happened with them, if so 'service' them. */
/* if recv fails connection closed, close this end and free up entry in connectlist */
/* else send correct to browser */
int    HTTP::select_server::read_from_client(int i, int j)
{
    int				valread;
    char 			*buffer;
	std::string		stringbuff;
	struct timeval	now;

	buffer = (char *)malloc(sizeof(char *) * BUFFER_SIZE + 1);
	if (!buffer) {
		perror("Malloc error");
		exit(1);
	}
	bzero(buffer, BUFFER_SIZE + 1);

	/* read from client */
	if ((valread = recv(_servers[i]._clients[j]._c_sock, &buffer[0], BUFFER_SIZE, 0)) < 0)
	{
		std::cout << "\nConnection lost: FD=" << _servers[i]._clients[j]._c_sock << " Slot" << i  << "error " << strerror(errno)<< std::endl;
		close(_servers[i]._clients[j]._c_sock);
		_servers[i]._clients.erase(_servers[i]._clients.begin() + j);
	    FD_CLR(_servers[i]._clients[j]._c_sock, &_read_backup);
		exit(EXIT_FAILURE);
	}
	/* Check if we are expecting a body */
	if (_servers[i]._clients[j]._expect_body)
	{
		_servers[i]._clients[j]._total_body_length += valread;
		HTTP::post_expected_body(_servers[i]._clients[j], buffer, valread);
		if (valread == 0 || (_servers[i]._clients[j]._total_body_length == _servers[i]._clients[j]._content_length))
		{
			_servers[i]._clients[j]._expect_body = false;
			FD_SET(_servers[i]._clients[j]._c_sock, &_write_backup);
			free(buffer);
			/* send a response to client POST is done */
			return 0;
		}
		//sleep(1);
		free(buffer);
		return valread;
	}

	//update clients last active
	gettimeofday(&now, NULL);
	_servers[i]._clients[j]._last_active = now;
	// parse buffer into request
	stringbuff = std::string(buffer);
	std::cout << stringbuff << std::endl;

	t_req_n_config							r_n_c;
	re_HTTP									requestinfo (stringbuff);
	std::map <std::string, std::string > 	reqmap = requestinfo._map_header;
	r_n_c._req_map = reqmap;
	r_n_c._parser_server = _parser_servers[i];
	respond m (r_n_c);
	_servers[i]._clients[j]._header = m.getTotalheader();
	// add client to write backups so next loop correct thing will be written
    FD_SET(_servers[i]._clients[j]._c_sock, &_write_backup);
	return valread;
}

void HTTP::select_server::send_response(int i, int j)
{
    std::cout << "in send response" << std::endl;
	std::cout << "_servers[i]._clients[j]._header: " << _servers[i]._clients[j]._header << std::endl;
	struct timeval now;

	//update clients last active
	gettimeofday(&now, NULL);
	_servers[i]._clients[j]._last_active = now;
	send(_servers[i]._clients[j]._c_sock , _servers[i]._clients[j]._header.c_str(), _servers[i]._clients[j]._header.size() , 0 );  
    FD_CLR(_servers[i]._clients[j]._c_sock, &_write_backup);
}

int              HTTP::select_server::erase_client(int i, int j)
{
	FD_CLR(_servers[i]._clients[j]._c_sock, &_read_backup);
	FD_CLR(_servers[i]._clients[j]._c_sock, &_read_fds);
	FD_CLR(_servers[i]._clients[j]._c_sock, &_write_fds);
	FD_CLR(_servers[i]._clients[j]._c_sock, &_write_backup);
	_servers[i]._clients.erase(_servers[i]._clients.begin() + j);
	j = 0;
	return (j);
}


/* launch in an endless loop */
void    HTTP::select_server::launch()
{
    FD_ZERO(&_read_backup);
    FD_ZERO(&_write_backup);
    // add servers to read_backup
	for (unsigned long listnum = 0; listnum < _servers.size(); listnum++) {
		if (_servers[listnum]._servers_socket != 0) {
			FD_SET(_servers[listnum]._servers_socket,&_read_backup);
			if (_servers[listnum]._servers_socket > _highsock)
				_highsock = _servers[listnum]._servers_socket;
		}
	}
    while(true)
    {
        _read_fds = _read_backup;
        _write_fds = _write_backup;
        std::cout << "...................WAITING////" << std::endl;
        selecter();
	    for (unsigned long i = 0; i < _servers.size(); i++) 
        {
            if (FD_ISSET(_servers[i]._servers_socket, &_read_fds)) {
                accepter(i);
            }
            for (unsigned long j = 0; j < _servers[i]._clients.size(); j++)
			{
				check_client_active(_servers[i]._clients[j]);
				if (_servers[i]._clients[j]._active == false)
				{
					j = erase_client(i, j);
					if (_servers[i]._clients.size() == 0)
						break;
				}
                if (FD_ISSET(_servers[i]._clients[j]._c_sock, &_read_fds)) {
                    if (read_from_client(i, j) == 0)
					{
						j = erase_client(i, j);
						if (_servers[i]._clients.size() == 0)
							break;
					}
                }
                if (FD_ISSET(_servers[i]._clients[j]._c_sock, &_write_fds)) {
                    send_response(i, j);
					FD_CLR(_servers[i]._clients[j]._c_sock, &_write_backup);
                }
            }
		}
	}
}

void HTTP::select_server::check_client_active(t_client_select &client)
{

	struct timeval start = client._last_active;
	struct timeval end;

	gettimeofday(&end, NULL);
	float difference = (end.tv_sec - start.tv_sec) + 1e-6*(end.tv_usec - start.tv_usec);
	if (difference > 60)
		client._active = false;
}


void            HTTP::select_server::set_value_server_select_server(int servers_socket, sockaddr_in servers_addr, int port, t_server_select &server)
{
	server._servers_addr = servers_addr;
	server._servers_socket = servers_socket;
	server._port = port;

}

void		HTTP::select_server::make_client(int client_socket, sockaddr_in addr, t_server_select &server)
{
	t_client_select newclient;
	struct timeval now;

	gettimeofday(&now, NULL);
	newclient._c_sock = client_socket;
	newclient._last_active = now;
	newclient._active = true;
	newclient._client_addr = addr;
	server._clients.push_back(newclient);
}
