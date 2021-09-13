/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response_2.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eyohn <sopka13@mail.ru>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/09 08:56:56 by eyohn             #+#    #+#             */
/*   Updated: 2021/09/13 13:53:18 by eyohn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Response_2.hpp"

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

int				Response_2::sendResponse()
{
#ifdef DEBUG
	std::cout	<< "Response_2::sendResponse start: fd = " << _fd << "; size container = " << _requests.size() << std::endl;
#endif
	int						ret = 0;
	std::string				path;
	std::string				tile;
	std::string::iterator	slesh;

	// step x: Check errors - if no request
	if (!_requests.size())
		return (1);

	// // step 1: Cycle of send response
	// while (_requests.size())
	// {
		// step x: Parse response
		Response response(_requests.operator[](0));
		_requests.pop_front();
		_close_flag = response.getClose();
		if (_close_flag)
			return (2);
		// std::cout << "step 2 ok" << std::endl;

		// step 3: Write data for client
		path = response.getPath();
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
		// std::cout << "step 3 ok" << std::endl;
		
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
		// std::cout << "step 4 ok" << std::endl;

		if (ret > 0)
			std::cout << "Respons " << ret << std::endl;
	// }

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
	int				ret = 0;
	fd_set			rfd;
	std::string		data;
	struct timeval	tv;
	tv.tv_sec = 0;
	tv.tv_usec = 0;

	while (1)
	{
		ret = recv(_fd, _buff, sizeof(_buff), 0);
		if (ret < 0)
		{
			std::string str("ERROR in get response: read fail");
			throw Exeption(str);
		}
		data += _buff;
		FD_ZERO(&rfd);
		FD_SET(_fd, &rfd);
		ret = select(1, &rfd, 0, 0, &tv);
		if (ret < 0)
		{
			std::cout << "ERROR in ft_handle_epoll_fd: select fall" << std::endl;
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

int		Response_2::sendingResponseGet(std::string full_path, struct stat is_a_dir, Response &response){
#ifdef DEBUG
	std::cout	<< "Response_2::sendingResponseGet start: fd = " << _fd << std::endl;
#endif
	int ret;
	lstat(full_path.c_str(), &is_a_dir);
	std::string	buff_1 = response.getHttp() + " 200 OK\n  Content-Type: text/html; charset=UTF-8\n Content-Length: 88\n\n";
	std::string rezult_path;
	if(S_ISDIR(is_a_dir.st_mode)){
		std::string index_name = getIndexFileName(full_path);
		if (index_name == "")
			rezult_path = _server->getErrPage();
		else
			rezult_path = full_path + index_name;
	}
	else{
		rezult_path = full_path;
		// std::cout << "GET zahod 444" << std::endl;
	}
	// std::cout << "GET zahod" << rezult_path << std::endl;
	std::ifstream	fileIndex(rezult_path);
	if (!fileIndex.is_open()){
		std::cout	<< "ERROR: Config file open error" << std::endl;
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

std::string Response_2::getIndexFileName(std::string path){
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
		// else {
		// 	std::cout << "NAME" << path + name << std::endl;
		// }
		++n;
	}

#ifdef DEBUG
	std::cout	<< "Response_2::getIndexFileName end: fd = " << _fd << std::endl;
#endif
	return ("");
	//страница не найдена
}

bool			Response_2::getCloseFlag()
{
	return (_close_flag);
}