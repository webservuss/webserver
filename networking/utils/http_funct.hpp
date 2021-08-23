#ifndef http_funct_hpp
#define http_funct_hpp

#include "../Servers/select_server.hpp"

namespace HTTP
{
    /* test non-blocking */
    void    set_non_blocking(int sock);
	int 	post_expected_body(t_client_select &client, char * &buffer, int &length);
	int		post_handle_request(t_client_select &client, std::map <std::string, std::string> reqmap, std::string stringbuff, char * &buffer, int valread);
}

#endif
