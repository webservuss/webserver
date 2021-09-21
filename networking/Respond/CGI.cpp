//
// Created by ruben on 12-08-21.
//

#include <limits.h>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "CGI.hpp"
#include "sys/wait.h"
#include "../utils/colors.hpp"

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

HTTP::CGI::CGI(std::map <std::string, std::string> request, const t_server server, const std::string &path)
{
	_status_code = 0;
	_request = request;
	_server = server;
	_path = path;

	set_port();
	set_path_info();
	set_request_method();
	set_script_filename();
	set_redirect_status();
	set_server_software();
	set_cgi_location();

	//print_all_cgi_vars();


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
	//_path_info = "PATH_INFO=/" + _request["URI"];
	_path_info = "PATH_INFO=/" + _path;
}

void HTTP::CGI::set_script_filename()
{
	char buf[256];
	getcwd(buf, 256);

	//_script_filename ="SCRIPT_FILENAME=" + std::string(buf) + "/www/html_pages/" + _request["URI"];
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

HTTP::CGI::~CGI()
{
	// check for leaks: https://stackoverflow.com/questions/67124700/why-is-address-sanitizer-not-detecting-this-simple-memory-leak

}




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
					NULL};

	pid_t pid;
	int p[2];

	pipe(p);
	pid = fork();
	int status = -1;
	if (pid == 0) {
		dup2(p[1], 1);
		close(p[0]);
		close(p[1]);
		if (execve(argv[0], argv, env) == -1)
		{
			exit(1);
		}
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
				_status_code = 333;
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


//
//static std::string getQueryString(std::map <std::string, std::string> req)
//{
//
//}
//

//void HTTP::CGI::set_cgi_env()
//{
//	std::cout << __FILE__ << __LINE__ << std::endl;
//	std::map<std::string, std::string> cgi_vars;
//	//Server specific variables:
//	cgi_vars["SERVER_SOFTWARE"] =	"Webservuss";			// name/version of HTTP server.
//	cgi_vars["SERVER_NAME"] =		_server._server_name;		// host name of the server, may be dot-decimal IP address.
//	cgi_vars["GATEWAY_INTERFACE"] =	"CGI/1.1";			// CGI/version.
//	//Request specific variables:
//	cgi_vars["SERVER_PROTOCOL"] =	"HTTP/1.1";			// HTTP/version.
//	cgi_vars["SERVER_PORT"] =		_server._port;		// TCP port (decimal).
//	/* TODO request method is not  */
////	cgi_vars["REQUEST_METHOD"] =	reworkGetRequestMethod(_request);	// name of HTTP method (see above).
//	cgi_vars["PATH_INFO"] =			getFullPath(_path);	// path suffix, if appended to URL after program name and a slash.
//	cgi_vars["PATH_TRANSLATED"] =	getFullPath(_path);	// corresponding full path as supposed by server, if PATH_INFO is present.
//	// TODO help? I am now picking the first location block for the cgi path :(
//	cgi_vars["SCRIPT_NAME"] =		getCGIPath(_server._location_map[0]._cgi);				// relative path to the program, like /cgi-bin/script.cgi.
////	cgi_vars["QUERY_STRING"] =		getQueryString(_request);				// the part of URL after ? character. The query string may be composed of *name=value pairs separated with ampersands (such as var1=val1&var2=val2...) when used to submit form data transferred via GET method as defined by HTML application/x-www-form-urlencoded.
////	cgi_vars["REMOTE_HOST"] =						// host name of the client, unset if server did not perform such lookup.
////	cgi_vars["REMOTE_ADDR"] =						// IP address of the client (dot-decimal).
////	cgi_vars["AUTH_TYPE"] =							// identification type, if applicable.
////	cgi_vars["REMOTE_USER"] = 						// used for certain AUTH_TYPEs.
////	cgi_vars["REMOTE_IDENT"] =						// see ident, only if server performed such lookup.
////	cgi_vars["CONTENT_TYPE"] =						// Internet media type of input data if PUT or POST method are used, as provided via HTTP header.
////	cgi_vars["CONTENT_LENGTH"] =					// similarly, size of input data (decimal, in octets) if provided via HTTP header.
//	//Variables passed by user agent (HTTP_ACCEPT, HTTP_ACCEPT_LANGUAGE, HTTP_USER_AGENT, HTTP_COOKIE and possibly others) contain values of corresponding HTTP headers and therefore have the same sense.
//
//	std::cout << __FILE__ << __LINE__ << std::endl;
//	_env = (char **)malloc(sizeof(char *) * (cgi_vars.size() + 1));
//	if (!_env) {
//		std::string err =  "Malloc error ";
//		error_exit(err, 1);
//		//perror("Malloc error");
//		//exit(1);
//	}
//	std::map<std::string, std::string>::iterator it;
//	int i = 0;
//
//	std::cout << __FILE__ << __LINE__ << " cgi_vars.size(): " << cgi_vars.size() << ", sizeof(_env): " << sizeof(_env) << std::endl;
//	std::cout << MAGENTA;
//	for (it = cgi_vars.begin(); it != cgi_vars.end(); ++it)
//	{
//		std::cout << it->first    // string (key)
//				  << ':'
//				  << it->second   // string's value
//				  << std::endl;
//		std::string tmp = it->first + "=" + it->second;
//		_env[i] = (char *)malloc(sizeof(char *) * tmp.length() + 1);
//		if (!_env[i]) {
//			std::string err =  "Malloc error ";
//			error_exit(err, 1);
//			//perror("Malloc error");
//			//exit(1);
//		}
//		// was first strcopy  but because of buffer overflow changed it.
//		//strcpy(_env[i], tmp.c_str());
//		strncpy(_env[i], tmp.c_str(),i);
//		i++;
//	}
//	_env[i] = NULL;
//	std::cout << RESET;
//	std::cout << __FILE__ << __LINE__ << std::endl;
//}


//static std::string getFullPath(const char *symlinkpath)
//{
//	char *ptr;
//	char actualpath[PATH_MAX];
//
//	ptr = realpath(symlinkpath, actualpath);
//	return (std::string(ptr));
//}
//
//
//
//
//static std::string getCGIPath(std::string &cgi_path)
//{
//	if (cgi_path.empty())
//		return ("/Users/avan-dam/Documents/webserver/bin/php-cgi_macos");
//	else
//		return cgi_path;
//}