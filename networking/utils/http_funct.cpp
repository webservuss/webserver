#include <stdlib.h>
#include <fcntl.h>
#include <fstream>
#include "http_funct.hpp"
#include "../utils/utils.hpp"
#include "../Respond/respond.hpp"

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

	std::cout << "filename: " << client._filename << std::endl;
	std::cout << "cont_length: " << client._content_length << std::endl;
	std::cout << "valread: " << length << std::endl;
	std::cout << "tot body lenght: " << client._total_body_length << std::endl;
	//std::cout << "i and j: " << i << " " << j << std::endl;
	std::cout << "c_sock: " << client._c_sock << std::endl;
	if (length == 0 || (client._total_body_length == client._content_length)) {
		client._expect_body = false;
		client._post_done = true;
		std::cout << __FILE_NAME__ << "ook hier?" << std::endl;
		return 1;
	}
	return 0;
}

int HTTP::post_handle_request(t_client_select &client, t_req_n_config r_n_c, std::string stringbuff, char * &buffer, int valread)
{
	(void)valread;
	// TODO directory has to be taken from config file? Also: ofstream does not create a directory
	client._filename = "www/html_pages/uploads/" + r_n_c._req_map["URI"]; // relative path of the server executable (don't start with a '/' !)
	std::ofstream out_file(client._filename.c_str(), std::ios::binary);
	client._content_length = ft_stoi(r_n_c._req_map["Content-Length:"]);
	// TODO: request method valid?

	// TODO: client body size valid?

	if (!(r_n_c._req_map.count("Expect:")))
	{
		int position_of_body = stringbuff.find("\r\n\r\n") + 4;
		out_file.write(&buffer[position_of_body], client._content_length);
		out_file.close();
		client._expect_body = false;
		client._post_done = true;
		std::string body(&buffer[position_of_body]);

		HTTP::respond::post_response(client, client._content_length, body);
		return 0;
	}
	else
	{
		out_file.close();
		/* send a brief response to the client */
		client._header = "HTTP/1.1 100 Continue\r\n\r\n";
		client._expect_body = true;
		client._post_done= false;
		return 1;
	}
}