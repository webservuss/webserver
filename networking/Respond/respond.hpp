#ifndef respond_hpp
#define respond_hpp

#include <iostream>
#include "../utils/req_n_conf.hpp"
#include "../Servers/select_server.hpp"

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
        std::string _servername;
        std::string _body;
        t_server    _pars_server;
        int         _status_code;
        std::string _totalpath;
        std::string _relativepath;
        std::string _totalheader;
        std::string _stat_cha;
        std::string _root;
        std::map<std::string, std::string> _totalrespond;
        std::map <std::string, std::string> _map_req;
        std::map<int, std::string> _status_errors;

    public:
        /* constructors & destructors */
        respond(t_req_n_config req_n_conf, t_client_select &client, char * &buffer);
        respond(const respond& x);
        ~respond();

        /* assignment opperator */
        respond& operator=(const respond& x);

        /* methods */
        void                getmethod();
        void                postmethod(t_client_select &client, char * &buffer);
        void                deletemethod(t_client_select &client);

		uint64_t find_client_body_size();
		void 				post_handle_request(t_client_select &client, char * &buffer, int valread);
		/* static method */
		static void 		post_response(t_client_select &client);

		/* setters */
        void                set_content_type(const std::string &contentype);
        void                set_status_code(int code);
        void                set_body();
        void                set_language(std::string contentlanguage);
        void                set_host(std::string host);
        void                set_content_len(std::string s);
        void                set_date();
        void                set_modified();
        void                set_connection(const std::string connection);
        void                set_status_line();
        void                set_total_response();
        void                set_no_config();
        void                set_server_name();

        /* getters */
        const std::string   &getTotalheader() const;

        /* check pathname */
        void                find_total_file_path();

        void                make_error_codes_map();
    	void                reset_body_error();
    };
}


#endif


