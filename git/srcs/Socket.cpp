#include "../includes/Socket.hpp"
#include <sstream>
#include <ctime>

//Socket::Socket(){};
Socket::Socket(){
    // step 3: Length of serv_addr
	_sock_len = sizeof(sockaddr_in);

	// step 4: Parse config file
	// if (ft_parse_config(vars))
	// 	ft_exit();

	// step 5: Set family socket, portno, ip address
	_serv_addr.sin_family = AF_INET;
	_serv_addr.sin_port = htons(PORT);
	_serv_addr.sin_addr.s_addr = inet_addr(IP_ADDRESS);	

    _tcp_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (_tcp_sockfd < 0)
	{
		std::cout << "ERROR opening socket 1: " << strerror(errno) << std::endl;
		return;
	}

	// step 2: Assigning a name to a socket
	int ret = bind(_tcp_sockfd, (struct sockaddr *)(&_serv_addr), _sock_len);
	// if (vars->ret < 0)
	// {
	// 	std::cout << "ERROR Assigning name to a socket fail: " << strerror(errno) << std::endl;
	// 	return (1);
	// }
    // step 3: Create queue connection (очередь)
	ret = listen(_tcp_sockfd, SOMAXCONN);
	// if (vars->ret < 0)
	// {
	// 	std::cout << "ERROR Listening fail: " << strerror(errno) << std::endl;
	// 	return (1);
	// }
    ++ret; //без этого ругается компилятор
}
Socket::~Socket(){}
Socket::Socket(const Socket& soc){
    _sock_len = soc._sock_len;
	_serv_addr.sin_family = soc._serv_addr.sin_family;
	_serv_addr.sin_port = soc._serv_addr.sin_port;
	_serv_addr.sin_addr.s_addr = soc._serv_addr.sin_addr.s_addr;
    _tcp_sockfd = soc._tcp_sockfd;

	// int ret = bind(_tcp_sockfd, (struct sockaddr *)(&_serv_addr), _sock_len);
    // ret = listen(_tcp_sockfd, SOMAXCONN);
}
Socket& Socket::operator= (const Socket& soc){
     _sock_len = soc._sock_len;
	_serv_addr.sin_family = soc._serv_addr.sin_family;
	_serv_addr.sin_port = soc._serv_addr.sin_port;
	_serv_addr.sin_addr.s_addr = soc._serv_addr.sin_addr.s_addr;
    _tcp_sockfd = soc._tcp_sockfd;
    return (*this);
}

static std::string toString(int a)
{
	std::ostringstream oss;
	oss << a;
	return (oss.str());
}

int			Socket::ft_handle_request()
{
	bzero(_buff, sizeof(_buff));
    // step 1: Read data from client
	int ret = recv(_fd, _buff, sizeof(_buff), 0);
	// if (vars->ret < 0)
	// {
	// 	std::cout << "ERROR Read fail: " << strerror(errno) << std::endl;
	// 	return (0);
	// }
	write(1, _buff, sizeof(_buff));
	//parsing_of_sock_buff(_buff);

	// step 2: Write data for client
	std::string	buff_1 = "Request accepted! Time: " + toString(static_cast<int>(clock()));
	ret = send(_fd, buff_1.c_str(), sizeof(buff_1), 0);
	// if (vars->ret < 0)
	// {
	// 	std::cout << "ERROR Response fail: " << strerror(errno) << std::endl;
	// 	return (0);
	// }
    ++ret; //без этого ругается компилятор
	// step 3: close fc
	close(_fd);	//FORBIDDEN
	return (0);
}

int Socket::getFd(){
    return (_fd);
}

int Socket::getTcp_sockfd(){
    return (_tcp_sockfd);
}

struct sockaddr_in Socket::getServ_addr(){
    return (_serv_addr);
}

int Socket::getSock_len(){
    return (_sock_len);
}

void Socket::setFd(){
    _fd = accept(_tcp_sockfd, (struct sockaddr *)(&_serv_addr), &_sock_len);
}
