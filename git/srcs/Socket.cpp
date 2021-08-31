#include "../includes/Socket.hpp"
#include <sstream>
#include <ctime>
#include <string>
#include "../includes/Response.hpp"

//Socket::Socket(){};
Socket::Socket(Server *server):
	_server(server)
{
	// step 1: Create socket
	_tcp_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (_tcp_sockfd < 0)
	{
		std::cout << "ERROR opening socket 1: " << strerror(errno) << std::endl;
		return;
	}

	// step 2: Assigning a name to a socket
	int ret = bind(_tcp_sockfd, (struct sockaddr *)(_server->getServAddr()), *_server->getSockLen());
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
// Socket::Socket(const Socket& soc){
// 	_sock_len = soc._sock_len;
// 	_serv_addr.sin_family = soc._serv_addr.sin_family;
// 	_serv_addr.sin_port = soc._serv_addr.sin_port;
// 	_serv_addr.sin_addr.s_addr = soc._serv_addr.sin_addr.s_addr;
// 	_tcp_sockfd = soc._tcp_sockfd;

	// int ret = bind(_tcp_sockfd, (struct sockaddr *)(&_serv_addr), _sock_len);
	// ret = listen(_tcp_sockfd, SOMAXCONN);
// }
// Socket& Socket::operator= (const Socket& soc){
// 	 _sock_len = soc._sock_len;
// 	_serv_addr.sin_family = soc._serv_addr.sin_family;
// 	_serv_addr.sin_port = soc._serv_addr.sin_port;
// 	_serv_addr.sin_addr.s_addr = soc._serv_addr.sin_addr.s_addr;
// 	_tcp_sockfd = soc._tcp_sockfd;
// 	return (*this);
// }

// static std::string toString(int a)
// {
// 	std::ostringstream oss;
// 	oss << a;
// 	return (oss.str());
// }
std::string Socket::getLoc(std::string path){
	return(_server->getLocations(path));
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
	
	Response response(static_cast<std::string>(_buff));
	if (response.getMetod() == 1 && (response.getPath() == "/" /*|| response.getPath() == "/favicon.ico")*/)){
		std::string	buff_1 = response.getHttp() + " 200 OK\n  Content-Type: text/html; charset=UTF-8\n Content-Length: 88\n\n";
		std::ifstream	fileIndex(getLoc(response.getPath()) + "index.html");																// файл может быть .html/.htm/.php
		if (!fileIndex.is_open()){
			std::cout	<< "ERROR: Config file open error" << std::endl;
			return (1);
		}
		std::string str;
		while(std::getline(fileIndex, str))
		{
			buff_1 += str;
			// std::cout << str_sum << std::endl;
		}
		ret = send(_fd, buff_1.c_str(), buff_1.length(), 0);
	}
	

	//std::string	buff_1 = "HTTP/1.1 200 OK\n Content-Type: text/html; charset=UTF-8\n Content-Length: 88\n\n<html><head><title>Tata</title></head><body><p>Hello world</p></body></html>\n";
	//ret = send(_fd, buff_1.c_str(), buff_1.length(), 0);
	if (ret > 0) std::cout << "Respons " << ret << std::endl;
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

void Socket::setFd(){
	_fd = accept(_tcp_sockfd, (struct sockaddr *)(_server->getServAddr()), _server->getSockLen());
}
