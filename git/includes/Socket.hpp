#ifndef SOCKET_HPP
# define SOCKET_HPP

#include "../includes/headers.hpp"
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

private:
	int						_tcp_sockfd;				// socket fd
	int						_fd;						// fd received after the acept call
	char					_buff[BUF_FOR_RESP];		// buffer for read from client
	Server					*_server;					// data for server
};
#endif