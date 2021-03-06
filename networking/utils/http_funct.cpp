#include <stdlib.h>
#include <fcntl.h>
#include <fstream>
#include "http_funct.hpp"

/* set non-blocking: to set a specific flag and leave the other flags as-is,
then you must F_GETFL the old flags, | the new flag in, and then F_SETFL the result
as two separate system calls; */
void                HTTP::set_non_blocking(int sock)
{
//    int opts;
	if (fcntl(sock, F_SETFL, O_NONBLOCK) < 0)
		exit(1);
	else
		return;
}

int HTTP::post_expected_body(t_client_select &client, char * &buffer, int &length)
{

	std::ofstream existing_file;
	existing_file.open(client._filename.c_str(), std::ios::binary | std::ios::app);
	existing_file.write(&buffer[0], length);
	client._total_body_length += length;

	if (length == 0 || (client._total_body_length == client._content_length)) {
		client._expect_body = false;
		client._post_done = true;
		return 1;
	}
	return 0;
}
