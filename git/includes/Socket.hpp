#ifndef SOCKET_HPP
# define SOCKET_HPP

#include "../includes/headers.hpp"
#include <sys/types.h>
#include <sys/stat.h>
class Socket{
public:
	// Socket();
	Socket(Server *server);
	//Socket(t_vars *vars);
	~Socket();
	// Socket(const Socket& soc);
	// Socket& operator= (const Socket& soc);
	int ft_handle_request();
	int getFd();
	void setFd();
	int getTcp_sockfd();
	std::string getLoc(std::string);

private:
	int						_tcp_sockfd;				// socket fd
	int						_fd;						// fd received after the acept call
	Server					*_server;					// data for server
};
#endif