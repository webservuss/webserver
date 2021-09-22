#ifndef TEST_HPP
#define TEST_HPP

#include <iostream>
#include <vector>
#include <map>
#include "main.hpp"
#include <iostream>
#include <vector>
#include <netinet/in.h>
#include <sys/time.h>
#include "../Sockets/listen_n_bind.hpp"
#include "parser_conf.hpp"


namespace HTTP
{
    std::vector<std::string> config();
}

#endif