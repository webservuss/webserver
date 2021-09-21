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

		void set_cgi_body();
		int get_status_code();
		void set_cgi_env();
		const std::string &get_cgi_body() const;

		void print_all_cgi_vars();

	private:
		int _status_code;

		std::string _cgi_body;
		t_server _server;
		std::map <std::string, std::string> _request;

		void set_script_filename();
		void set_path_info();
		void set_request_method();
		void set_redirect_status();
		void set_port();
		void set_server_software();
		void set_cgi_location();
		std::string _port;
		std::string _path;
		std::string _request_method;
		std::string _redirect_status;
		std::string _path_info;
		std::string _server_software;
		std::string _script_filename;
		std::string _cgi_location;

		char **_env;
	};
}

#endif //INC_12AUG_CGI_HPP
