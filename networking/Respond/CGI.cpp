#include <limits.h>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "CGI.hpp"
#include "sys/wait.h"
#include "../utils/colors.hpp"

HTTP::CGI::CGI(std::map <std::string, std::string> request, const t_server server, const std::string &path, const std::string &root)
{
	_status_code = 0;
	_request = request;
	_server = server;
	_path = path;
	_placeholder = root;

	set_port();
	set_path_info();
	set_request_method();
	set_script_filename();
	set_redirect_status();
	set_server_software();
	set_cgi_location();
	set_auto_index_path();
	set_cgi_body();
}

void HTTP::CGI::set_redirect_status()
{
	_redirect_status ="REDIRECT_STATUS=200";
}

void HTTP::CGI::set_request_method()
{
	_request_method = "REQUEST_METHOD=" + _request["METHOD"];
}

void HTTP::CGI::set_path_info()
{
	_path_info = "PATH_INFO=/" + _path;
}

void HTTP::CGI::set_script_filename()
{
	char buf[256];
	getcwd(buf, 256);
	_script_filename ="SCRIPT_FILENAME=" + std::string(buf) + "/" + _path;
}

void HTTP::CGI::set_port()
{
	_port = "SERVER_PORT=" + ft_numtos(_server._port);
}

void HTTP::CGI::set_server_software()
{
	_server_software = "SERVER_SOFTWARE=Webservuss";
}

int HTTP::CGI::get_status_code()
{
	return _status_code;
}

void HTTP::CGI::set_cgi_location()
{
	char buf[512];
	getcwd(buf, 512);
	_cgi_location = std::string(buf) + "/bin/php-cgi_macos";
}

void HTTP::CGI::set_auto_index_path()
{
	char buf[512];
	getcwd(buf, 512);
	std::string server_root = _placeholder;
	_auto_index_path = "AUTO_INDEX=" + std::string(buf) + "/" + server_root + _request["URI"];
}

HTTP::CGI::~CGI()
{}

void HTTP::CGI::set_cgi_body()
{
	char *argv[] = {(char *)_cgi_location.c_str(),
				 	(char *)_path.c_str(),
				 	NULL };

	char *env[] = { (char *)_port.c_str(),
					(char *)_redirect_status.c_str(),
					(char *)_script_filename.c_str(),
					(char *)_path_info.c_str(),
					(char *)_server_software.c_str(),
					(char *)_request_method.c_str(),
					(char *)_auto_index_path.c_str(),
					NULL};

	pid_t			pid;
	int 			p[2];

	pipe(p);
	pid = fork();
	int status = -1;
	if (pid == 0) 
	{
		dup2(p[1], 1);
		close(p[0]);
		close(p[1]);
		if (execve(argv[0], argv, env) == -1)
			exit(1);
		exit(0);
	}
	else {
		wait(&status);
		if (status)
		{
			close(p[0]);
			close(p[1]);
			_status_code = 500;
		}
		else
		{
			char *buffer = new char[4096 * 4096];
			close(p[1]);
			int bytes_read = read(p[0], buffer, 4096 *4096);
			close(p[0]);
			if (bytes_read < 0)
				_status_code = 500;
			else
			{
				_status_code = 200;
				buffer[bytes_read] = 0;
				std::string tmp = std::string(buffer);
				int start = tmp.find("\r\n\r\n") + 4;
				tmp = tmp.substr(start, tmp.length() - start);
				_cgi_body = tmp;
			}
			delete [] buffer;
		}
	}
}

const std::string &HTTP::CGI::get_cgi_body() const
{
	return _cgi_body;
}

/* to test */
void HTTP::CGI::print_all_cgi_vars()
{
	std::cout << RED << "++++++++++++++++++++"
	<< "\n_port; " << _port
    << "\n_path; " << _path
	<< "\n_request_method; " << _request_method
	<< "\n_redirect_status; " << _redirect_status
	<< "\n_path_info; " << _path_info
	<< "\n_server_software; " << _server_software
	<< "\n_script_filename; " << _script_filename
	<< "\n_cgi_location; " << _cgi_location << "\n++++++++++++" << RESET << std::endl;
}