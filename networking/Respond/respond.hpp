#ifndef respond_hpp
#define respond_hpp

#include <stdio.h>
#include <string>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>


#include "../../http.hpp"
#include "../Servers/simple_server.hpp"
#include "../Colors.hpp"


namespace HTTP{

    /* make an enum for GET DELETE POST */
    enum method {

        DELETE,
        GET,
        POST

    };


    class respond{

    private:

    public:
        respond_line(std::string respond_line);

    };
}
