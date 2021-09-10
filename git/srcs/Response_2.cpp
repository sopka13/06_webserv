/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response_2.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eyohn <sopka13@mail.ru>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/09 08:56:56 by eyohn             #+#    #+#             */
/*   Updated: 2021/09/10 10:01:56 by eyohn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Response_2.hpp"

Response_2::Response_2(Server *server, int fd):
	_server(server),
	_fd(fd),
	_close_flag(false)
{
// #ifdef DEBUG
	std::cout	<< "Response_2::Response_2 start: fd = " << _fd << std::endl;
// #endif
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

// #ifdef DEBUG
	std::cout	<< "Response_2::~Response_2 end: fd = " << _fd << std::endl;
// #endif
}

void			Response_2::handleRequest()
{
#ifdef DEBUG
	std::cout	<< "Response_2::handleRequest start: fd = " << _fd << std::endl;
#endif
	// step 1: Read data from client
	int ret = recv(_fd, _buff, sizeof(_buff), 0);
	if (ret < 0)
	{
		std::string str("ERROR in get response: read fail");
		throw Exeption(str);
	}
	std::cout << _buff << std::endl;

	// step 2: Parse response
	Response response(static_cast<std::string>(_buff));
	_close_flag = response.getClose();
	// std::cout << "step 2 ok" << std::endl;

	// step 3: Write data for client
	std::string path = response.getPath();
	std::string tile = "";
	std::string::iterator slesh = path.end() - 1;
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
	// if (vars->ret < 0)
	// {
	// 	std::cout << "ERROR Response fail: " << strerror(errno) << std::endl;
	// 	return (0);
	// }
	// step 3: close fc
	// ret = close(_fd);
	// std::cout << "ret_close = " << ret << std::endl;
	// return (0);
#ifdef DEBUG
	std::cout	<< "Response_2::handleRequest end: fd = " << _fd << std::endl;
#endif
}

int		Response_2::sendingResponseGet(std::string full_path, struct stat is_a_dir, Response &response){
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
	return (ret);
}

std::string Response_2::getIndexFileName(std::string path){
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
	return ("");
	//страница не найдена
}

bool			Response_2::getCloseFlag()
{
	return (_close_flag);
}