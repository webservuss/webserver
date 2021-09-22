#ifndef http_funct_hpp
#define http_funct_hpp

#include "../Servers/select_server.hpp"
#include "../Respond/respond.hpp"

namespace HTTP
{
    /* test non-blocking */
    void    set_non_blocking(int sock);
	int 	post_expected_body(t_client_select &client, char * &buffer, int &length);
}

#endif
