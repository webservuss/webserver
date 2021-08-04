#ifndef respond_hpp
#define respond_hpp

#include <stdio.h>
#include <string>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include <string>
#include <cstring>
#include <vector>
#include <map>
#include <list>
#include <iostream>
#include <unistd.h>
#include <exception>
#include <fcntl.h>


#include "../../http.hpp"
#include "../Request/re_HTTP.hpp"
#include "../utils/utils.hpp"
//#include "../Servers/simple_server.hpp"


#include "../Servers/select_server.hpp"
#include "../utils/req_n_conf.hpp"

#include "../Colors.hpp"


namespace HTTP{

    /* make an enum for GET DELETE POST */
//    enum method {
//
//        DELETE,
//        GET,
//        POST
//
//    };


    class respond{

    private:
        std::string _statusline;
        std::string _contentlen;
        std::string _server;
        std::string _lastmodified;
        std::string _contentype;
        std::string _contenttype;
        std::string _connection;
        std::string _date;
        std::string _host;
        std::string _language;
        std::string _body;
        t_server    _pars_server;
        int         _status_code;
        std::string _postheader;
        std::string _deleteheader;
        std::map <std::string, std::string> _map_req;
        int         filefd;
        const char  *_path;
        const char  *_pathpost;
        // t_req_n_config  _req_n_conf;

    public:
        const std::string &getTotalheader() const;

    private:
        std::string _totalheader;
        std::map<std::string, std::string> _totalrespond;


    public:
        const std::string &getStatusline() const;

    public:
        respond(s_req_n_config req_n_conf);
        // respond(std::map < std::string, std::string > mapHeader);
        void status_line(std::string getkey);
        void setLanguage(std::string contentlanguage);
        void setHost(std::string host);
        int content_len(std::string content_len);
        void setContentlen(std::string s);
        void setStatusline(const std::string &statusline);
        void setDate();
        void setmodified(int fileFD);
        void setcontenttype(const std::string &contentype);
        void setconnection(const std::string connection);
        void setserver(const std::string connection);
        void setbody();
        void                        appendheader();
        std::string                 find_total_file_path();
        void                        set_status_code(int code);
        void                        startres();
        void                        postmethod();
        void                        deletemethod();
    };
}


#endif


