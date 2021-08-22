#ifndef SOCKET_HPP
# define SOCKET_HPP

#include "../includes/headers.hpp"
class Socket{
public:
    Socket();
    //Socket(t_vars *vars);
    ~Socket();
    Socket(const Socket& soc);
    Socket& operator= (const Socket& soc);
    int ft_handle_request();
    int getFd();
    void setFd();
    int getTcp_sockfd();
    struct sockaddr_in getServ_addr();
    int getSock_len();
    


private:
    int						_tcp_sockfd;				// socket fd
	struct sockaddr_in		_serv_addr;				// name for ipv4
	socklen_t				_sock_len;				// length of serv_addr
	int						_fd;						// fd received after the acept call
	char					_buff[BUF_FOR_RESP];		// buffer for read from client
};
#endif