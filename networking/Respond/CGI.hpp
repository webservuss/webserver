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
		CGI(std::map <std::string, std::string> request, const t_server server, const std::string &path);
		~CGI();

		void set_cgi_body();
		void set_cgi_env();
		const std::string &get_cgi_body() const;

	private:
		std::string _cgi_body;
		t_server _server;
		std::map <std::string, std::string> _request;
		char *_path;
		char **_env;
	};
}

#endif //INC_12AUG_CGI_HPP
