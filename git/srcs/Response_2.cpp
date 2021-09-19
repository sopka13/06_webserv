/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response_2.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eyohn <sopka13@mail.ru>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/09 08:56:56 by eyohn             #+#    #+#             */
/*   Updated: 2021/09/19 16:47:52 by eyohn            ###   ########.fr       */
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

	// step x: If have cgi go handle
	// std::cout << "RESULT = " << rezult_path << std::endl;
	if (haveCGI(rezult_path))
		rezult_path = handleCGI(rezult_path);

	// std::cerr << "result returned = " << rezult_path << std::endl;

	std::ifstream	fileIndex;
	fileIndex.open(rezult_path);
	if (!fileIndex.is_open()){
		std::cerr	<< "ERROR: Config file open error" << std::endl;
		return (-1);
	}
	std::string str;
	while(std::getline(fileIndex, str))
	{
		// std::cout << "str = " << str << std::endl;
		buff_1 += str;
	}
	// std::cout << buff_1 << std::endl;
	ret = send(_fd, buff_1.c_str(), buff_1.length(), 0);

	// step x: Remove temp file
	if (rezult_path.size() && rezult_path.find(".temp", 0) == (rezult_path.size() - 5))
		remove(rezult_path.c_str());

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

int				Response_2::haveCGI(std::string &result_path)
{
#ifdef DEBUG
	std::cout	<< "Response_2::haveCGI start" << std::endl;
#endif
	// step 1: If CGI format not defined
	if (_server->getCGI_format() == "")
		return (0);

	// step 2: If str length enough and last chars compare CGI format
	if (result_path.size() > (_server->getCGI_format()).size() &&
		result_path.find(_server->getCGI_format(), 0) == (result_path.size() - (_server->getCGI_format()).size()))
		return (1);

#ifdef DEBUG
	std::cout	<< "Response_2::haveCGI end" << std::endl;
#endif
	return (0);
}

std::string		Response_2::handleCGI(std::string &result_path)
{
#ifdef DEBUG
	std::cout	<< "Response_2::handleCGI start" << std::endl;
#endif
	// step 1: Init data

	// step 2: Create argv
	std::vector<char *> argv;

	// step 3: Add "-f" flag for php scripts
	char temp[] = "-f";
	if (_server->getCGI_format() == ".php")
		argv.push_back(temp);

	// step 4: Add adress script_file in argv
	char		dir[100];
	getcwd(dir, 100);
	std::string cur_dir(dir);
	result_path.erase(0, 1);
	cur_dir += result_path;
	char *str = new char[cur_dir.size() + 1];
	std::copy(cur_dir.begin(), cur_dir.end(), str);
	str[cur_dir.size()] = '\0';
	argv.push_back(str);

	// step 5: Create envp and add env vars
	std::vector<char *> envp;
	if (_variables.size())
	{
		std::string				temp;
		std::string::iterator	start = _variables.begin();
		std::string::iterator	end = _variables.end();
		while (start != end)
		{
			if (*start == '&' || start + 1 == end)
			{
				if (start + 1 == end && *start != '&')
					temp += *start;
				if (temp.size())
				{
					char *str_1 = new char[temp.size() + 1];
					std::copy(temp.begin(), temp.end(), str_1);
					str_1[temp.size()] = '\0';
					envp.push_back(str_1);
				}
				temp.clear();
				start++;
				continue ;
			}
			temp += *start;
			start++;
		}
	}

	// step 6: Construct file_name for result CGI handler
	std::string::iterator	end = cur_dir.end();
	end--;
	while (cur_dir.size() && *end != '.')
		cur_dir.erase(end--);
	cur_dir.erase(end);
	cur_dir += ".temp";

	// step 7: Create file and clean it
	std::ofstream	temp_file;
	temp_file.open(cur_dir, std::ofstream::trunc);
	if (!temp_file.is_open())
		throw Exeption("ERROR in response_2: create temp_file error!");
	temp_file.close();

	
	// step 8: execute handler
	int		id = 0;
	id = fork();
	if (id == -1)
		throw Exeption("ERROR in response_2: create process for CGI handlerr error");
	else if (id == 0)
	{
		int ret = 0;
		freopen(cur_dir.c_str(), "w", stdout);
		if ((ret = execve((_server->getCGI_handler()).c_str(), &(*argv.begin()), &(*envp.begin()))) == -1)
			std::cerr << "ERROR CGI: execute CGI handler error" << std::endl;
		fclose(stdout);
		exit(0);
	}

	// step 9: Wait child process
	int		status;
	waitpid(id, &status, 0);

	// step 10: Free memory
	delete[] str;
	for (int i = 0; i < static_cast<int>(envp.size()); ++i)
		delete[] envp.operator[](i);

#ifdef DEBUG
	std::cout	<< "Response_2::handleCGI end" << std::endl;
#endif
	return (cur_dir);
}