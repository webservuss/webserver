#ifndef select_server_hpp
#define select_server_hpp

#include "simple_server.hpp"
#include "../../http.hpp"
#include "../utils/http_funct.hpp"
#include "../Request/re_HTTP.hpp"

#define BACKLOG 10

namespace HTTP
{
        class select_server : public simple_server
        {
            private:
                /* variables needed for select()*/
                int             _highsock;
                fd_set          _socks;
                int             _connectlist[BACKLOG];
            public:
                /* constructor */
                select_server();
                /*copy constructor */
                select_server(const select_server& x);
                // /*assignment operator */
                select_server& operator=(const select_server& x);
                // /*destructor */

                ~select_server();
                /* call select with the correct FD_SET */
                int             selecter();
                /* implement the virtual void functions from simple_server
                to accept, handle respond and launch */
                void    		accepter();
                void    		handeler();
                void    		launch();
        };
}
#endif