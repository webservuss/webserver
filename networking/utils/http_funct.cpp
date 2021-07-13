#include "http_funct.hpp"

/* set non-blocking: to set a specific flag and leave the other flags as-is, 
then you must F_GETFL the old flags, | the new flag in, and then F_SETFL the result
 as two separate system calls; */
void                HTTP::set_non_blocking(int sock)
{
    int opts;

	opts = fcntl(sock,F_GETFL);
	if (opts < 0) {
		perror("fcntl(F_GETFL)");
		exit(EXIT_FAILURE);
	}
	opts = (opts | O_NONBLOCK);
	if (fcntl(sock,F_SETFL,opts) < 0) {
		perror("fcntl(F_SETFL)");
		exit(EXIT_FAILURE);
	}
	return;
}
