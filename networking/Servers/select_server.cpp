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

#define BUFFER_SIZE (4024 * 1024) // 1Mb

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
{ 
	_highsock = x._highsock;
   	_read_fds = x._read_fds;
    _write_fds = x._write_fds;
    _write_backup = x._write_backup;
    _read_backup = x._read_backup;
    _servers = x._servers;
}

/*assignment operator */
HTTP::select_server& HTTP::select_server::operator=(const select_server& x)
{ 
	_highsock = x._highsock;
   	_read_fds = x._read_fds;
    _write_fds = x._write_fds;
    _write_backup = x._write_backup;
    _read_backup = x._read_backup;
    _servers = x._servers;
    return *this;
}

/*destructor */
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
	
	timeout.tv_sec = 5;
	timeout.tv_usec = 0;
	readsocks = select(FD_SETSIZE, &_read_fds, &_write_fds, (fd_set *) 0, &timeout);
	if( readsocks < 0)
		throw select_error_ex();
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
			throw select_server::select_error_ex();
	set_non_blocking(connection);
	make_client(connection, addr, _servers[i]);
	FD_SET(connection,&_read_backup);
    
}

/* Run through our sockets and check to see if anything happened with them, if so 'service' them. */
/* if recv fails connection closed, close this end and free up entry in connectlist */
/* else send correct to browser */
int    HTTP::select_server::read_from_client(int i, int j)
{
	int				valread;
	char 			*buffer;
	struct timeval	now;

	buffer = new char [BUFFER_SIZE + 1];
	bzero(buffer, BUFFER_SIZE + 1);
	/* read from client */
	if ((valread = recv(_servers[i]._clients[j]._c_sock, &buffer[0], BUFFER_SIZE, 0)) < 0)
	{
		erase_client(i, j);
		delete [] buffer;
		throw select_error_ex();
	}
	std::string stringbuff2 = std::string(buffer);
	if (valread == 0)
	{
		delete [] buffer;
		FD_CLR(_servers[i]._clients[j]._c_sock, &_read_backup);
		return (0);
	}
	gettimeofday(&now, NULL);
	_servers[i]._clients[j]._last_active = now;
	_servers[i]._clients[j]._header = "";
	/* Check if we are expecting a body */
	if (_servers[i]._clients[j]._expect_body)
	{
		if (HTTP::post_expected_body(_servers[i]._clients[j], buffer, valread))
		{
			std::string body = "";
			HTTP::respond::post_response(_servers[i]._clients[j]);
			FD_SET(_servers[i]._clients[j]._c_sock, &_write_backup);
		}
		delete [] buffer;
//		if (_servers[i]._clients[j]._post_done == true)
//			FD_CLR(_servers[i]._clients[j]._c_sock, &_read_backup);
		return valread;
	}
	/* parse buffer into request */
	std::string stringbuff = std::string(buffer);
	t_req_n_config							r_n_c;
	re_HTTP									requestinfo (stringbuff.substr(0, stringbuff.find("\r\n\r\n") + 2));
	std::map <std::string, std::string > 	reqmap = requestinfo._map_header;
	r_n_c._req_map = reqmap;
	r_n_c._parser_server = _parser_servers[i];
	/*create response */
	respond m (r_n_c, _servers[i]._clients[j], buffer);
	if (stringbuff.substr(0, 4) != "POST")
	{
		_servers[i]._clients[j]._header = m.getTotalheader();
		FD_CLR(_servers[i]._clients[j]._c_sock, &_read_backup);
	}
	FD_SET(_servers[i]._clients[j]._c_sock, &_write_backup);
	delete [] buffer;
	if (reqmap["Connection:"] == " close")
		_servers[i]._clients[j]._close_connection = true;
	return valread;
}

void HTTP::select_server::send_response(int i, int j)
{
	struct timeval now;

	int sendval = 0;
	std::string buf(_servers[i]._clients[j]._header);
	if (_servers[i]._clients[j]._header.size())
	{
		if (buf.size() >= 512*512) {
			sendval = send(_servers[i]._clients[j]._c_sock, &buf.c_str()[0], 512*512, 0);
			_servers[i]._clients[j]._header = _servers[i]._clients[j]._header.substr(sendval, _servers[i]._clients[j]._header.size() - sendval);
		}
		else {
			sendval = send(_servers[i]._clients[j]._c_sock, &buf.c_str()[0], buf.size(), 0);
			_servers[i]._clients[j]._header = "";
		}
	}
	if (sendval < 0)
	{
		erase_client(i, j);
		throw select_error_ex();
	}
	if (sendval == 0 || (_servers[i]._clients[j]._close_connection))
		_servers[i]._clients[j]._active = false;
	gettimeofday(&now, NULL);
	_servers[i]._clients[j]._last_active = now;
	if (_servers[i]._clients[j]._header.empty())
		FD_CLR(_servers[i]._clients[j]._c_sock, &_write_backup);
}

int              HTTP::select_server::erase_client(int i, int j)
{
	FD_CLR(_servers[i]._clients[j]._c_sock, &_read_backup);
	FD_CLR(_servers[i]._clients[j]._c_sock, &_read_fds);
	FD_CLR(_servers[i]._clients[j]._c_sock, &_write_fds);
	FD_CLR(_servers[i]._clients[j]._c_sock, &_write_backup);

	close(_servers[i]._clients[j]._c_sock);
	_servers[i]._clients.erase(_servers[i]._clients.begin() + j);
	return (0);
}

/* launch in an endless loop */
void    HTTP::select_server::launch()
{
    FD_ZERO(&_read_backup);
    FD_ZERO(&_write_backup);
    /* add servers to read_backup */
	for (unsigned long listnum = 0; listnum < _servers.size(); listnum++) {
		if (_servers[listnum]._servers_socket != 0) {
			FD_SET(_servers[listnum]._servers_socket,&_read_backup);
			if (_servers[listnum]._servers_socket > _highsock)
				_highsock = _servers[listnum]._servers_socket;
		}
	}
    std::cout << GREEN << "server starting..." << RESET << std::endl;
    while(true)
    {
		_read_fds = _read_backup;
		_write_fds = _write_backup;

		try{
			selecter();
		}
            catch(std::exception &e ){
       	 	std::cerr << e.what() << std::endl;
        	error_exit("error in select_Server", 1);
		}
		
	    for (unsigned long i = 0; i < _servers.size(); i++) 
        {
		
	    	
            if (FD_ISSET(_servers[i]._servers_socket, &_read_fds)) {
				try{
					accepter(i);
				}
				catch(std::exception &e ){
        			std::cerr << e.what() << std::endl;
        			error_exit("error in accepter", 1);
				}
            }
            for (unsigned long j = 0; j < _servers[i]._clients.size(); j++)
			{
				check_client_active(_servers[i]._clients[j]);
				if (_servers[i]._clients[j]._active == false)
				{
					j = erase_client(i, j);
					break;
				}
                if (FD_ISSET(_servers[i]._clients[j]._c_sock, &_read_fds)) {
					try{
						read_from_client(i, j);
					}
					    catch(std::exception &e ){
        					std::cerr << e.what() << std::endl;
        					error_exit("connection lost", 1);
						}
                    
					if (_servers[i]._clients[j]._active == false)
					{
						j = erase_client(i, j);
						break;
					}
				}
                if (FD_ISSET(_servers[i]._clients[j]._c_sock, &_write_fds)) {
					try{
						send_response(i, j);
					}
					    catch(std::exception &e ){
        					std::cerr << e.what() << std::endl;
        					error_exit("connection lost", 1);
						}
					if (_servers[i]._clients[j]._active == false)
					{
						j = erase_client(i, j);
						break;	
					}
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
	if (difference > 5)
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
	struct timeval 	now;

	gettimeofday(&now, NULL);
	newclient._c_sock = client_socket;
	newclient._last_active = now;
	newclient._active = true;
	newclient._client_addr = addr;
	server._clients.push_back(newclient);
	newclient._close_connection = false;
	newclient._header = "";
	newclient._expect_body = false;
	newclient._chunked = false;
}

/* error throw */
const char  *HTTP::select_server::select_error_ex::what() const  throw()
{
	return ("Error in select_server");
}
