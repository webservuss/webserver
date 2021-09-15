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



HTTP::CGI::CGI(std::map <std::string, std::string> request, const t_server server, const std::string &path)
{

	_request = request;
	_server = server;
	std::cout << RED << __LINE__ << __FILE__ << " >>> " << path.length() << std::endl;
	_path = (char *)malloc(sizeof(char) * path.length() + 1);
	if (!_path)
	{
		//perror("Malloc error");
		//exit(1);
		std::string err =  "Malloc error "; 
    	error_exit(err, 1);
	}

	strcpy(_path, path.c_str());
	//set_cgi_env();
	set_cgi_body();
}

HTTP::CGI::~CGI()
{
	// check for leaks: https://stackoverflow.com/questions/67124700/why-is-address-sanitizer-not-detecting-this-simple-memory-leak
//	if (_path)
//	{
//		free(_path);
//		_path = NULL;
//	}
//	for (int i = 0; _env[i] != NULL; i++)
//	{
//		free(_env[i]);
//		_env[i] = NULL;
//	}
//	free(_env);
//	_env = NULL;
}

static std::string getFullPath(const char *symlinkpath)
{
	char *ptr;
	char actualpath[PATH_MAX];

	ptr = realpath(symlinkpath, actualpath);
	return (std::string(ptr));
}

static std::string reworkGetRequestMethod(std::map<std::string, std::string> req)
{
	if (!(req.find("GET") == req.end()))
		return "GET";
	if (!(req.find("POST") == req.end()))
		return "POST";
	if (!(req.find("DELETE") == req.end()))
		return "DELETE";
	else
		return "NO_METHOD_FOUND";
}

	static std::string getCGIPath(std::string &cgi_path)
{
	if (cgi_path.empty())
		return ("/usr/bin/php-cgi");
	else
		return cgi_path;
}
//
//static std::string getQueryString(std::map <std::string, std::string> req)
//{
//
//}
//

void HTTP::CGI::set_cgi_env()
{
	std::cout << __FILE__ << __LINE__ << std::endl;
	std::map<std::string, std::string> cgi_vars;
	//Server specific variables:
	cgi_vars["SERVER_SOFTWARE"] =	"Webservuss";			// name/version of HTTP server.
	cgi_vars["SERVER_NAME"] =		_server._server_name;		// host name of the server, may be dot-decimal IP address.
	cgi_vars["GATEWAY_INTERFACE"] =	"CGI/1.1";			// CGI/version.
	//Request specific variables:
	cgi_vars["SERVER_PROTOCOL"] =	"HTTP/1.1";			// HTTP/version.
	cgi_vars["SERVER_PORT"] =		_server._port;		// TCP port (decimal).
	/* TODO request method is not  */
	cgi_vars["REQUEST_METHOD"] =	reworkGetRequestMethod(_request);	// name of HTTP method (see above).
	cgi_vars["PATH_INFO"] =			getFullPath(_path);	// path suffix, if appended to URL after program name and a slash.
	cgi_vars["PATH_TRANSLATED"] =	getFullPath(_path);	// corresponding full path as supposed by server, if PATH_INFO is present.
	// TODO help? I am now picking the first location block for the cgi path :(
	cgi_vars["SCRIPT_NAME"] =		getCGIPath(_server._location_map[0]._cgi);				// relative path to the program, like /cgi-bin/script.cgi.
//	cgi_vars["QUERY_STRING"] =		getQueryString(_request);				// the part of URL after ? character. The query string may be composed of *name=value pairs separated with ampersands (such as var1=val1&var2=val2...) when used to submit form data transferred via GET method as defined by HTML application/x-www-form-urlencoded.
//	cgi_vars["REMOTE_HOST"] =						// host name of the client, unset if server did not perform such lookup.
//	cgi_vars["REMOTE_ADDR"] =						// IP address of the client (dot-decimal).
//	cgi_vars["AUTH_TYPE"] =							// identification type, if applicable.
//	cgi_vars["REMOTE_USER"] = 						// used for certain AUTH_TYPEs.
//	cgi_vars["REMOTE_IDENT"] =						// see ident, only if server performed such lookup.
//	cgi_vars["CONTENT_TYPE"] =						// Internet media type of input data if PUT or POST method are used, as provided via HTTP header.
//	cgi_vars["CONTENT_LENGTH"] =					// similarly, size of input data (decimal, in octets) if provided via HTTP header.
	//Variables passed by user agent (HTTP_ACCEPT, HTTP_ACCEPT_LANGUAGE, HTTP_USER_AGENT, HTTP_COOKIE and possibly others) contain values of corresponding HTTP headers and therefore have the same sense.

	std::cout << __FILE__ << __LINE__ << std::endl;
	_env = (char **)malloc(sizeof(char *) * (cgi_vars.size() + 1));
	if (!_env) {
		std::string err =  "Malloc error "; 
    	error_exit(err, 1);
		//perror("Malloc error");
		//exit(1);
	}
	std::map<std::string, std::string>::iterator it;
	int i = 0;

	std::cout << __FILE__ << __LINE__ << " cgi_vars.size(): " << cgi_vars.size() << ", sizeof(_env): " << sizeof(_env) << std::endl;
	std::cout << MAGENTA;
	for (it = cgi_vars.begin(); it != cgi_vars.end(); ++it)
	{
		std::cout << it->first    // string (key)
				  << ':'
				  << it->second   // string's value
				  << std::endl;
		std::string tmp = it->first + "=" + it->second;
		_env[i] = (char *)malloc(sizeof(char *) * tmp.length() + 1);
		if (!_env[i]) {
			std::string err =  "Malloc error "; 
    		error_exit(err, 1);
			//perror("Malloc error");
			//exit(1);
		}
		// was first strcopy  but because of buffer overflow changed it. 
		//strcpy(_env[i], tmp.c_str());
		strncpy(_env[i], tmp.c_str(),i);
		i++;
	}
	_env[i] = NULL;
	std::cout << RESET;
	std::cout << __FILE__ << __LINE__ << std::endl;
}

void HTTP::CGI::set_cgi_body()
{

	std::string cgi_location = "/usr/bin/php-cgi";

	char *argv[] = {(char *)cgi_location.c_str(), _path, NULL };
	char *env[] = {(char *)"SERVER_PORT=1000", (char *)"environment", NULL};
	pid_t pid;
	char *buffer;
	int p[2];

	pipe(p);
	pid = fork();
	if (pid == 0) {
		dup2(p[1], 1);
		close(p[0]);
		close(p[1]);
		// maybe check if php file is valid 
		if (execve(argv[0], argv, env) == -1) {
			perror("Could not execve");
			// maybe exit 
		}
	}
	else {
		wait(NULL);
		buffer = (char *)malloc(sizeof(char *) * 4096 * 4096);
		close(p[1]);
		int bytes_read = read(p[0], buffer, 4096 *4096);
		std::cout << "BR: " << bytes_read << std::endl;
		for (int i = 0; i < bytes_read; ++i) {
			std::cout << buffer[i];
		}
		buffer[bytes_read] = 0;
		std::string tmp = std::string(buffer);

		std::cout << std::endl;
		std::cout << tmp << std::endl;
		int start = tmp.find("<html>");
		tmp = tmp.substr(start, tmp.length());
		_cgi_body = tmp;
		free(buffer);
	}

}

const std::string &HTTP::CGI::get_cgi_body() const
{
	return _cgi_body;
}
