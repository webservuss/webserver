//
// Created by ruben on 12-08-21.
//

#ifndef INC_12AUG_CGI_HPP
#define INC_12AUG_CGI_HPP

#include <map>
#include <iostream>
#include "../Servers/parser_conf.hpp"

namespace HTTP
{
	class CGI
	{
	public:
		// TODO: map_req?
		CGI(std::map <std::string, std::string> request, const t_server server, const std::string &path);
		~CGI();
		class	cgi_error : public std::exception {
		public:
			virtual const char*	what() const throw();
		};

		void set_cgi_body();
		void set_cgi_env();
		const std::string &get_cgi_body() const;
		int get_status_code() const;

	private:
		std::string _cgi_body;
		int 		_status_code;
		t_server _server;
		std::map <std::string, std::string> _request;
		char *_path;
		char **_env;
	};
}

#endif //INC_12AUG_CGI_HPP
