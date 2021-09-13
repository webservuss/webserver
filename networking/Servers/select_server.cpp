#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fstream>
#include "select_server.hpp"
#include "../Request/re_HTTP.hpp"
#include "../utils/http_funct.hpp"
#include "../utils/req_n_conf.hpp"
#include "../utils/utils.hpp"
#include "../utils/colors.hpp"
#include "../Respond/respond.hpp"

#define BUFFER_SIZE (104 * 104) // 1Mb

//#define BUFFER_SIZE (1024) // 1Mb
// /* constructor calls simple_server and launches */ // need to add in parser_servers here too
// HTTP:: select_server::select_server()
// {
// 	listen_n_bind * socket = new HTTP::listen_n_bind(AF_INET, SOCK_STREAM, 0, 80, INADDR_ANY, BACKLOG);
// 	t_server_select newserver;
// 	set_value_server_select_server(socket->get_sock(), socket->get_address(), newserver);
// 	_servers.push_back(newserver);
// 	launch();
// }



const char  *HTTP::select_server::select_error_ex::what() const  throw()
{

	    std::cout <<BLUE <<  "ERROR in select server " << RESET << std::endl;
	return ("Error in select_server");
}


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
	if( readsocks < 0)
	{
		try{	
			throw select_error_ex();
		}
			catch (std::exception &e ){
				std::cerr << YELLOW << "error in select" << RESET << std::endl;
			}
		
	}
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
			
			//try{
				std::cout << "error in accept" << std::endl;
				throw select_server::select_error_ex();
			//}
			//catch(std::exception &e ){
				///std::cout << "error in accept" << std::endl;
		//	}
			
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
	struct timeval	now;

	buffer = (char *)malloc(sizeof(char *) * BUFFER_SIZE + 1);
	if (!buffer) {

		//throw select_server::select_error_ex();
		std::string err =  "Malloc error "; 
    	error_exit(err, 1);
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
	/* TODO check valread == 0 here; FD_SET as well? */
	if (valread == 0)
	{
//		FD_SET(_servers[i]._clients[j]._c_sock, &_write_backup);
		free(buffer);
//		_servers[i]._clients[j]._active = false;
		return 0;
	}

	//update clients last active
	gettimeofday(&now, NULL);
	_servers[i]._clients[j]._last_active = now;
	_servers[i]._clients[j]._header = "";
	/* Check if we are expecting a body */
	if (_servers[i]._clients[j]._expect_body)
	{
		//_servers[i]._clients[j]._total_body_length += valread;
		if (HTTP::post_expected_body(_servers[i]._clients[j], buffer, valread))
		{
			std::string body = "";
			HTTP::respond::post_response(_servers[i]._clients[j], _servers[i]._clients[j]._total_body_length, body);
			FD_SET(_servers[i]._clients[j]._c_sock, &_write_backup);
		}
		free(buffer);
		return valread;
	}
	// parse buffer into request
	std::string stringbuff = std::string(buffer);
	t_req_n_config							r_n_c;
	re_HTTP									requestinfo (stringbuff.substr(0, stringbuff.find("\r\n\r\n") + 2));
	std::map <std::string, std::string > 	reqmap = requestinfo._map_header;
	r_n_c._req_map = reqmap;
	r_n_c._parser_server = _parser_servers[i];
	respond m (r_n_c, _servers[i]._clients[j], buffer, valread);


//	/* Check if POST and(!) contains a body	*/
	if (stringbuff.substr(0, 4) == "POST")
	{

			//m.post_handle_request(_servers[i]._clients[j], r_n_c, stringbuff, buffer, valread);
			FD_SET(_servers[i]._clients[j]._c_sock, &_write_backup);
			std::cout << _servers[i]._clients[j]._post_done << std::endl;
			free(buffer);
			return valread;
	}
	if (stringbuff.substr(0, 3) == "GET")
		_servers[i]._clients[j]._header = m.getTotalheader();
	//_servers[i]._clients[j]._header = "HTTP/1.1 204 No Content\r\n\r\n";
	// add client to write backups so next loop correct thing will be written
	std::cout << YELLOW << _servers[i]._clients[j]._header << RESET << std::endl;
    FD_SET(_servers[i]._clients[j]._c_sock, &_write_backup);
    free(buffer);
	return valread;
}

void HTTP::select_server::send_response(int i, int j)
{
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
	std::vector<t_client_select>::iterator it;
	it = _servers[i]._clients.begin();
	it = it + j;
	//_servers[i]._clients.erase(_servers[i]._clients.begin() + j);
	std::cout << RED << j << " " << _servers[i]._clients.size() << "value: " << _servers[i]._clients[j]._c_sock << std::endl;
//	_servers[i]._clients[j]._c_sock = 0;
	close(_servers[i]._clients[j]._c_sock);
	_servers[i]._clients.erase(it);
	std::cout << _servers[i]._clients.size() << "value: " << _servers[i]._clients[j]._c_sock << RESET << std::endl;
	std::cout << RED << "erase client?" << std::endl;
	std::cout << BLUE << _servers[i]._clients[j]._c_sock << RESET << std::endl;
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
	    	if (i == 0)
	    		std::cout << MAGENTA << _servers[0]._clients.size() << RESET << std::endl;
            if (FD_ISSET(_servers[i]._servers_socket, &_read_fds)) {
            	std::cout << "ACCEPTER" << std::endl;
                accepter(i);
            }
            for (unsigned long j = 0; j < _servers[i]._clients.size(); j++)
			{
				check_client_active(_servers[i]._clients[j]);
				if (_servers[i]._clients[j]._active == false)
				{
					std::cout << "_ACTIVE == FALSE" << std::endl;
					//close(_servers[i]._clients[j]._c_sock);
					j = erase_client(i, j);
					//if (_servers[i]._clients.size() == 0)
						break;
				}
                else if (FD_ISSET(_servers[i]._clients[j]._c_sock, &_read_fds)) {
					std::cout << "FD_ISSET, &_read_fds" << std::endl;
					std::cout << "i and j: " << i << " " << j << std::endl;

                    if (read_from_client(i, j) == 0)
					{
						//close(_servers[i]._clients[j]._c_sock);
						j = erase_client(i, j);
//						if (_servers[i]._clients.size() == 0)
							break;
					}
				}
                else if (FD_ISSET(_servers[i]._clients[j]._c_sock, &_write_fds)) {
					std::cout << "FD_ISSET, &_write_fds" << std::endl;
                    send_response(i, j);
					FD_CLR(_servers[i]._clients[j]._c_sock, &_write_backup);

//					if (_servers[i]._clients[j]._post_done) {
//						std::cout << GREEN << "_post_done" << RESET << std::endl;
//						close(_servers[i]._clients[j]._c_sock);
//						j = erase_client(i, j);
//						if (_servers[i]._clients.size() == 0)
//							break;
//					}
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
