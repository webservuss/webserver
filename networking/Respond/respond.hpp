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
#include "CGI.hpp"
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


    class respond  {
       // respond(){}

    private:
        std::string _statusline;
        std::string _contentlen;
        std::string _lastmodified;
        std::string _connection;
        std::string _date;
        std::string _host;
        std::string _language;
        std::string _body;
        t_server    _pars_server;
        int         _status_code;
        std::string _postheader;
        std::string _deleteheader;
        std::string _postbodylen;
        std::string _deletefile;
        
        
        std::map <std::string, std::string> _map_req;
        std::string       _contentype;
        int _status;


        int         filefd;
//        const char  *_path;
       // const char  *_pathpost;
        // t_req_n_config  _req_n_conf;


    public:
        const std::string &getTotalheader() const;

    private:
        std::string _totalheader;
        std::map<std::string, std::string> _totalrespond;


    public:
        respond(s_req_n_config req_n_conf);
        ~respond();
        // respond(std::map < std::string, std::string > mapHeader);
        void status_line            (std::string getkey);
        void setLanguage            (std::string contentlanguage);
        void setHost                (std::string host);
        void setContentlen          (std::string s);
        void setDate();
        void setmodified            (int fileFD);
        void setconnection          (const std::string connection);
        void setbody();
		void cgi_php();
        void                        appendheader();
        std::string                 find_total_file_path();
        void                        set_status_code(int code);
        void                        startres();
        void                        postmethod();
        void                        deletemethod();
        void                        getmethod();
        void                        setcontenttype(const std::string &contentype);
        int         setstatus;

    };
}


#endif


