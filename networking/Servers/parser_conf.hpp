#ifndef __PARSER_CONF_HPP__
#define __PARSER_CONF_HPP__

#include <iostream>
#include <vector>
#include <map>
#include "../utils/utils.hpp"

namespace HTTP {

	typedef struct s_location {
		std::vector<std::string>			_methods;
		std::string _root;
		std::string _cgi;
		std::string _index;
		std::string _autoindex;
		int 		_client_body_size;
		std::string	_redir;
	}				t_location;

	typedef struct							s_server {
		std::string							_server_name;
		int        							_port;
		std::string							_host;
		std::vector<std::string>			_error_page;
		int        							_auto_index;
		std::string							_root;
		std::string							_index;
		std::map<std::string, t_location>	_location_map;
	}										t_server;

	class parse_conf
	{
		private:
			std::vector<t_server>	_server;

			/* setters */
			void set_values_server(std::string s, t_server &server, char **argv);
			void set_values_location(std::string s, t_location &location);

		public:
			/* constructors and destructors */
			parse_conf(const char *path, char **argv);
			parse_conf(const parse_conf& x);
			~parse_conf();

			/*assignment operators */
       		parse_conf& operator=(const parse_conf& x);

			/* GETTERS */
			int							get_server_port(const t_server &server);
			const std::vector<t_server>	&get_server() const;
			std::vector<int>			get_ports();
	};
}
#endif //__PARSER_CONF_HPP__
