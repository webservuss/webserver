#ifndef http_funct_hpp
#define http_funct_hpp

#include <stdio.h>
#include <iostream>
#include <fcntl.h>
#include "../../all_libs.hpp"

namespace HTTP
{
    /* test non-blocking */
    void    set_non_blocking(int sock);
}

#endif