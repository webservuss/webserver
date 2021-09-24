
#ifndef re_HTTP_hpp
#define re_HTTP_hpp

#include <iostream>
#include <map>

namespace HTTP{
    class re_HTTP
    {
        private:
            std::string _headers;
            std::string _method;
            std::string  _uri;
            std::string  _totalBody;
            std::string _protocol;
            re_HTTP(){}
        public:
            std::map<std::string, std::string> _map_header;

        public:
            /*constructors & destructors */
            re_HTTP(std::string dataparser);
            re_HTTP(const re_HTTP& x);
            ~re_HTTP();

            /* assignment opperator */
            re_HTTP& operator=(const re_HTTP& x);
    
            /* setters */
            int         set_request_line(std::string &requestline);
            void        set_headers(std::string body);

    };

}





#endif