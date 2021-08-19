#ifndef respond_hpp
#define respond_hpp

#include "../../http.hpp"
#include "../Request/re_HTTP.hpp"
#include "../utils/utils.hpp"
#include "CGI.hpp"
#include "../Servers/select_server.hpp"
#include "../utils/req_n_conf.hpp"
#include "../utils/colors.hpp"

namespace HTTP{
    class respond  {
    private:
        std::string _statusline;
        std::string _contentlen;
        std::string _lastmodified;
        std::string _connection;
        std::string _contentype;
        std::string _date;
        std::string _host;
        std::string _language;
        std::string _body;
        t_server    _pars_server;
        int         _status_code;
        std::string _totalpath;
        std::string _relativepath;
        std::string _totalheader;
        std::map<std::string, std::string> _totalrespond;
        std::map <std::string, std::string> _map_req;
        char const * _stat_cha;
        // std::string _postheader;
        // std::string _deleteheader;
        // std::string _postbodylen;
        // std::string _deletefile;
        int         filefd; /// i think we can take out

    public:
        /* constructors & destructors */
        respond(s_req_n_config req_n_conf);
        respond(const respond& x);
        ~respond();

        /* assignment opperator */
        respond& operator=(const respond& x);

        /* methods */
        void                getmethod();
        void                postmethod();
        void                deletemethod();
		void                cgi_php();

        /* setters */
        void                setcontenttype(const std::string &contentype);
        void                set_status_code(int code);
        void                setbody();
        void                setLanguage(std::string contentlanguage);
        void                setHost(std::string host);
        void                setContentlen(std::string s);
        void                setDate();
        void                setmodified();
        void                setconnection(const std::string connection);
        void                set_status_line();
        void                set_total_response();
        void                set_no_config404(std::string root);

        /* getters */
        const std::string   &getTotalheader() const;

        /* check pathname */
        void                find_total_file_path();
    };
}


#endif


