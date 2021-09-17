/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response_2.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eyohn <sopka13@mail.ru>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/09 08:56:56 by eyohn             #+#    #+#             */
/*   Updated: 2021/09/15 12:03:14 by eyohn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Response_2.hpp"
#include <fstream>

Response_2::Response_2(Server *server, int fd):
	_server(server),
	_fd(fd),
	_close_flag(false)
{
#ifdef DEBUG
	std::cout	<< "Response_2::Response_2 start: fd = " << _fd << std::endl;
#endif
	// step 1: Clear buff
	bzero(_buff, sizeof(_buff));

#ifdef DEBUG
	std::cout	<< "Response_2::Response_2 end: fd = " << _fd << std::endl;
#endif
}

Response_2::~Response_2()
{
#ifdef DEBUG
	std::cout	<< "Response_2::~Response_2 start: fd = " << _fd << std::endl;
#endif

#ifdef DEBUG
	std::cout	<< "Response_2::~Response_2 end: fd = " << _fd << std::endl;
#endif
}

std::string Response_2::setVariables(std::string &str){
	size_t i = str.find("?");
	std::string var = "";
	if (i != std::string::npos){
		std::string::iterator it = str.begin();
		it += i + 1;
		while (it != str.end()){
			var += *it;
			++it;
		}
		str.erase(i, str.length());
	}
	return (var);
}

int				Response_2::sendResponse()
{
#ifdef DEBUG
	std::cout	<< "Response_2::sendResponse start: fd = " << _fd << "; size container = " << _requests.size() << std::endl;
#endif
	// step 1: Init data
	int						ret = 0;
	std::string				path;
	std::string				tile;
	std::string::iterator	slesh;

	// step 2: Check errors - if no unhandled request
	if (!_requests.size())
		return (1);

	// // step 1: Cycle of send response ??? do we need

	// step 3: Parse response
	Response response(_requests.operator[](0));
	_requests.pop_front();
	_close_flag = response.getClose();
	if (_close_flag)
		return (2);

	// step 4: Write data for client
	path = response.getPath();
	_variables = setVariables(path);
	// std::cout	<< "Response_2::_variables =R" << _variables << "R" << std::endl;
	// std::cout	<< "Response_2::path =R" << path << "R" << std::endl;
	tile = "";
	slesh = path.end() - 1;
	while (_server->getLocations(path) == "" && path.length() > 1){
		while (*slesh != '/' && slesh != path.begin()){
			tile += *slesh;
			path.erase(slesh, path.end());
			--slesh;
		}
		if (path.length() > 1){
			tile += *slesh;
			path.erase(slesh, path.end());
			--slesh;
		}
	}
	
	std::reverse(tile.begin(), tile.end());
	std::string m = "GET";
	struct stat is_a_dir;
	if (response.getMetod() == 1 &&
		(_server->getLocations(path) != "") &&
		_server->getMethods(path, m))
	{
		std::string full_path = _server->getLocations(path) + tile;
		ret = sendingResponseGet(full_path, is_a_dir, response);
	}

	m = "PUT";
	if (response.getMetod() == 3 &&
		(_server->getLocations(path) != "") &&
		_server->getMethods(path, m))
	{
		std::string full_path = _server->getLocations(path) + tile;
		lstat(full_path.c_str(), &is_a_dir);
		std::string	buff_1 = response.getHttp() + " 200 OK\n\n";//  Content-Type: text/html; charset=UTF-8\n Content-Length: 88\n\n";
		std::ofstream file;
		file.open("../put_rezalt.txt");
		file << response.getBody();
		file.close();
		ret = send(_fd, buff_1.c_str(), buff_1.length(), 0);
	}

	if (ret > 0)
		std::cout << "Respons " << ret << std::endl;

#ifdef DEBUG
	std::cout	<< "Response_2::sendResponse end: fd = " << _fd << "; size container = " << _requests.size() << std::endl;
#endif
	return (0);
}

void			Response_2::readRequest()
{
	// Need checker errors: 1)if no data in fd
#ifdef DEBUG
	std::cout	<< "Response_2::readRequest start: fd = " << _fd << "; size container = " << _requests.size() << std::endl;
#endif
	// step 1: Init data
	int				ret = 0;
	fd_set			rfd;
	std::string		data;
	struct timeval	tv;
	tv.tv_sec = 0;
	tv.tv_usec = 0;

	// step 2: Cycle for read data from fd in _buff
	while (1)
	{
		// step 2.1: Read
		ret = recv(_fd, _buff, sizeof(_buff), 0);
		if (ret < 0)
		{
			std::string str("ERROR in get response: read fail");
			throw Exeption(str);
		}

		// step 2.2: Accumelate received data
		data += _buff;

		// step 2.3: Clear data for select
		FD_ZERO(&rfd);
		FD_SET(_fd, &rfd);

		// step 2.4: Need to update the need for this item 
		ret = select(1, &rfd, 0, 0, &tv);
		if (ret < 0)
		{
			std::cerr << "ERROR in ft_handle_epoll_fd: select fall" << std::endl;
			return ;
		}
		else if (ret == 0)
			break ;
		else
		{
			if (FD_ISSET(_fd, &rfd))
				continue ;
			break ;
		}
	}

	// step 1: Read data from client
	// while ((ret = recv(_fd, _buff, sizeof(_buff), 0)))
	// {
	// 	if (ret < 0)
	// 	{
	// 		std::string str("ERROR in get response: read fail");
	// 		throw Exeption(str);
	// 	}
	// 	data += _buff;
	// 	ft_bzero(&_buff, sizeof(_buff));
	// }
	// std::cout << "step 1 ok; data = " << _buff << std::endl;

	// step 2: Add request in container
	_requests.push_back(data);

#ifdef DEBUG
	std::cout	<< "Response_2::readRequest end: fd = "
				<< _fd
				<< "; size container = "
				<< _requests.size()
				<< std::endl;
#endif
	return ;
}

int				Response_2::sendingResponseGet(std::string full_path, struct stat is_a_dir, Response &response){
#ifdef DEBUG
	std::cout	<< "Response_2::sendingResponseGet start: fd = " << _fd << std::endl;
#endif
	int ret;
	lstat(full_path.c_str(), &is_a_dir);
	std::string	buff_1 = response.getHttp() + " 200 OK\n\n";//  Content-Type: text/html; charset=UTF-8\n Content-Length: 88\n\n";
	std::string rezult_path;
	if(S_ISDIR(is_a_dir.st_mode)){
		std::string index_name = getIndexFileName(full_path);
		if (index_name == "")
			rezult_path = _server->getErrPage();
		else
			rezult_path = full_path + index_name;
	}
	else
		rezult_path = full_path;
	std::ifstream	fileIndex(rezult_path);
	if (!fileIndex.is_open()){
		std::cerr	<< "ERROR: Config file open error" << std::endl;
		return (-1);
	}
	std::string str;
	while(std::getline(fileIndex, str))
	{
		buff_1 += str;
	}
	ret = send(_fd, buff_1.c_str(), buff_1.length(), 0);
	fileIndex.close();

#ifdef DEBUG
	std::cout	<< "Response_2::sendingResponseGet end: fd = " << _fd << std::endl;
#endif
	return (ret);
}

std::string		Response_2::getIndexFileName(std::string path){
#ifdef DEBUG
	std::cout	<< "Response_2::getIndexFileName start: fd = " << _fd << std::endl;
#endif
	std::vector<std::string> ind = *(_server->getIndexName());
	std::string name;
	std::vector<std::string>::iterator n = ind.begin();
	while (n != ind.end()){
		name = *n;
		std::ifstream	file(path + name);
		if (file.is_open()){
			file.close();
			return (name);
		}
		++n;
	}

#ifdef DEBUG
	std::cout	<< "Response_2::getIndexFileName end: fd = " << _fd << std::endl;
#endif
	return ("");
}

bool			Response_2::getCloseFlag()
{
	return (_close_flag);
}