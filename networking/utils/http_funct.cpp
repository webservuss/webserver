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

int HTTP::post_expected_body(const t_client_select &client, char * &buffer, int &length)
{
	std::cout << "filename: " << client._filename << std::endl;
	std::cout << "cont_length: " << client._content_length << std::endl;
	std::cout << "valread: " << length << std::endl;
	std::cout << "tot body lenght: " << client._total_body_length << std::endl;
	//std::cout << "i and j: " << i << " " << j << std::endl;
	std::cout << "c_sock: " << client._c_sock << std::endl;

	std::ofstream existing_file;
	existing_file.open(client._filename.c_str(), std::ios::binary | std::ios::app);
	existing_file.write(&buffer[0], length);


	return 0;
}

