#ifndef respond_hpp
#define respond_hpp

#include <sys/stat.h>
#include "../../http.hpp"
#include "../Request/re_HTTP.hpp"
#include "../utils/utils.hpp"
#include "CGI.hpp"
#include "../Servers/select_server.hpp"
#include "../utils/req_n_conf.hpp"
#include "../utils/colors.hpp"
#include "../Respond/respond.hpp"

namespace HTTP{
    class statuscode  {
    void     set_status_line();
    };
}


#endif
