/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response_2.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eyohn <sopka13@mail.ru>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/09 08:56:56 by eyohn             #+#    #+#             */
/*   Updated: 2021/09/29 01:23:24 by eyohn            ###   ########.fr       */
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

std::string		Response_2::setVariables(std::string &str)
{
	// Need handle getter vars from body. If have POST method with vars in body.
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

void			Response_2::sendFile(std::string full_path)
{
	// This function send file
#ifdef DEBUG
	std::cout	<< "Response_2::sendFile start" << std::endl;
#endif
	// step 1: Init data
	int		ret = 0;
	int		fd_from;

	// step 2: Get file_info
	struct stat	info;
	ret = stat(full_path.c_str(), &info);
	if (ret == -1)
	{
		Headliners resp(std::string("HTTP/1.1"), std::string("500"));
		resp.sendHeadliners(_fd);

		std::string str("ERROR in Response_2::sendFile: File not found");
		throw Exeption(str);
	}

	// step 3: Open file
	fd_from = open((_server->getErrPage()).c_str(), O_RDONLY);
	if (ret == -1)
	{
		Headliners resp(std::string("HTTP/1.1"), std::string("500"));
		resp.sendHeadliners(_fd);

		std::string str("ERROR in Response_2::sendFile: Open file error");
		throw Exeption(str);
	}

	// step 4: Send file
	ret = sendfile(_fd, fd_from, NULL, info.st_size);
	if (ret == -1)
	{
		Headliners resp(std::string("HTTP/1.1"), std::string("500"));
		resp.sendHeadliners(_fd);

		std::string str("ERROR in Response_2::sendFile: Send file error");
		throw Exeption(str);
	}

	// step 5: Close file
	close(fd_from);

// #ifdef DEBUG
	std::cout	<< "Response_2::sendFile end; ret = " << ret << std::endl;
// #endif
}

void			Response_2::postHandle(Response *response)
{
	// This function handle POST method and has the following logic:
	// 	1. if target_file doesn't exist
	// 		- create file and return 201 - created, or error if creation impossible
	// 	2. else
	// 		2.1. if target_file is a CGI:
	// 			a. get vars from body
	// 			b. execute CGI with received vars
	// 			c. send response 204 - if no content, 200 - if have content, or error if impossible
	// 		2.2. if target_file is not CGI but exist
	// 			a. If it's a directory
	// 				- create new file from body
	// 				- send response 201 - created, or error if creation impossible
	// 			b. If it's a file
	// 				- return 400 - Bad request
#ifdef DEBUG
	std::cout	<< "Response_2::postHandle start" << std::endl;
#endif
	// step 1: Init data
	std::string		path(response->getPath());		// original path from request
	std::string		full_path;						// path with locations
	std::string		method("POST");
	int				ret = 0;
	_variables = setVariables(path);
	std::cout << "path = " << path << std::endl;

	// step 2: Get full_path with locations
	std::string::iterator	end = path.end();
	std::string				temp;
	end--;
	while (path.size() && (_server->getLocations(path) == ""))
	{
		temp = full_path;
		full_path = *end;
		full_path += temp;
		path.erase(end);
		end--;
	}

	// step 3: Check error - if no path, and if have path get full_path with locations
	if (path == "")
	{
		Headliners resp(std::string("HTTP/1.1"), std::string("404"));
		resp.setCloseConnection(false);
		resp.sendHeadliners(_fd);

		sendFile(_server->getErrPage());

		std::string str("ERROR in ft_post_handle: Bad path");
		throw Exeption(str);
	}
	else
	{
		temp = _server->getLocations(path) + full_path;
		full_path = temp;
		full_path += getIndexFileName(full_path);
	}

	// step 4: Check error - if method no supported
	if (!_server->getMethods(path, method))
	{
		Headliners resp(std::string("HTTP/1.1"), std::string("403"));
		resp.setCloseConnection(false);
		resp.sendHeadliners(_fd);

		return ;
	}

	// step 5: Check target file/dir for exist
	struct stat	info;
	ret = stat(full_path.c_str(), &info);
	if (ret == -1)							//file/dir doesn't exist
	{
		// step 1: Create new file and write body
		std::ofstream	new_file;
		new_file.open(full_path);
		new_file << response->getBody();
		new_file.close();

		// step 2: Send headliners
		Headliners resp(std::string("HTTP/1.1"), std::string("201"));
		resp.setCloseConnection(false);
		resp.sendHeadliners(_fd);

		return ;
	}
	else
	{
		if (haveCGI(full_path))				//target_file is a CGI
		{
			// step 1: Add body in vars
			_variables += response->getBody();

			// step 2: Execute CGI file
			std::string		target_file;
			target_file = handleCGI(full_path);

			// step 3: Send headliners
			struct stat	info_2;
			stat(target_file.c_str(), &info_2);
			if (info_2.st_size == 0)
			{
				Headliners resp(std::string("HTTP/1.1"), std::string("204"));
				resp.setCloseConnection(false);
				resp.sendHeadliners(_fd);
			}
			else
			{
				Headliners resp(std::string("HTTP/1.1"), std::string("200"));
				resp.setCloseConnection(false);
				resp.sendHeadliners(_fd);
			}

			// step 4: Send file
			std::cout << "target_file = " << target_file << std::endl;
			sendFile(target_file);

			// step 5: Remove temp file
			// if (target_file.size() && target_file.find(".temp", 0) == (target_file.size() - 5))
			// 	remove(target_file.c_str());
			return ;
		}
		else								//target_file is not CGI
		{
			if (S_ISDIR(info.st_mode))	//it's a directory
			{
				// step 1: Create new file and write body
				std::ofstream	new_file;
				new_file.open(full_path);
				new_file << response->getBody();
				new_file.close();
				
				// step 2: Send headliners
				Headliners resp(std::string("HTTP/1.1"), std::string("201"));
				resp.setCloseConnection(false);
				resp.sendHeadliners(_fd);

				return ;
			}
			else		//it's a file
			{
				Headliners resp(std::string("HTTP/1.1"), std::string("400"));
				resp.setCloseConnection(false);
				resp.sendHeadliners(_fd);

				return ;
			}
		}
	}


#ifdef DEBUG
	std::cout	<< "Response_2::postHandle end" << std::endl;
#endif
	return ;
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
	Response response(_requests.operator[](0), _fd);
	_requests.pop_front();

	// step 4: If have close connection - return
	_close_flag = response.getClose();
	if (_close_flag)
		return (2);

	// step x: Handle POST method
	if (response.getMetod() == 2)
	{
		postHandle(&response);
		return (0);
	}

	// step 5: Write data for client
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
		ret = sendingResponseGet(full_path, is_a_dir, path);
	}

	m = "PUT";
	if (response.getMetod() == 3 &&
		(_server->getLocations(path) != "") &&
		_server->getMethods(path, m))
	{
		std::string full_path = _server->getLocations(path) + tile;
		int i = lstat(full_path.c_str(), &is_a_dir);
		std::ofstream file;
		file.open(full_path);
		if (!file.is_open()){
			Headliners resp(std::string("HTTP/1.1"), std::string("500"));
			resp.sendHeadliners(_fd);
			return (-1);
		}
		file << response.getBody();
		file.close();
		std::string	buff_1 = response.getHttp(); 
		if (response.getBody() == ""){
			Headliners resp(std::string("HTTP/1.1"), std::string("204"));
			resp.sendHeadliners(_fd);
		}
			
		else if (i < 0){
			Headliners resp(std::string("HTTP/1.1"), std::string("201"));
			resp.sendHeadliners(_fd);
		}
		else{
			Headliners resp(std::string("HTTP/1.1"), std::string("200"));
			resp.sendHeadliners(_fd);

		}
		// ret = send(_fd, buff_1.c_str(), buff_1.length(), 0);
		// std::cout << "\n RESPONS PUT: " << buff_1 << std::endl;
	}


	if (ret > 0)
		std::cout << "Respons " << ret  << std::endl;

#ifdef DEBUG
	std::cout	<< "Response_2::sendResponse end: fd = " << _fd << "; size container = " << _requests.size() << std::endl;
#endif
	return (0);
}

void			Response_2::readRequest()
{
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
			Headliners resp(std::string("HTTP/1.1"), std::string("403"));
			resp.sendHeadliners(_fd);
			std::string str("ERROR in get response: read fail");
			throw Exeption(str);
		}
		else if (ret == 0) // if no data in fd
		{
			Headliners resp(std::string("HTTP/1.1"), std::string("100"));
			resp.sendHeadliners(_fd);
		}

		// step 2.2: Accumulate received data and clean _buff
		data += _buff;
		ft_bzero(&_buff, sizeof(_buff));

		// step 2.3: Clear data for select
		FD_ZERO(&rfd);
		FD_SET(_fd, &rfd);

		// step 2.4: Check action on client fd
		ret = select(1, &rfd, 0, 0, &tv);
		//		if error
		if (ret < 0)
		{
			Headliners resp(std::string("HTTP/1.1"), std::string("500"));
			resp.sendHeadliners(_fd);
			std::string str("ERROR in ft_handle_epoll_fd: select fall");
			throw Exeption(str);
		}
		//		if no actions
		else if (ret == 0)
			break ;
		//		if have actions
		else
		{
			if (FD_ISSET(_fd, &rfd))
				continue ;
			break ;
		}
	}

	// step 3: Add request in container if have any data from fd
	if (data.size())
		_requests.push_back(data);
	// std::cout << "\n" << data << "\n" << std::endl;

#ifdef DEBUG
	std::cout	<< "Response_2::readRequest end: fd = "
				<< _fd
				<< "; size container = "
				<< _requests.size()
				<< std::endl;
#endif
	return ;
}

std::string		Response_2::ft_get_dir_list(std::string& full_path)
{
#ifdef DEBUG
	std::cout	<< "Response_2::ft_get_dir_list start" << std::endl;
#endif
	DIR*			dirp;
	struct dirent*	dp;
	std::string		dir_content("<html>\n\t<body>\n");

	// std::cout << "full_path = " << full_path << std::endl;

	dirp = opendir(full_path.c_str());
	while ((dp = readdir(dirp)) != NULL)
	{
		// std::cout	<< "xxx = " << dp->d_name
		// 			<< "; " << dp->d_ino
		// 			<< "; " << dp->d_off
		// 			<< "; " << dp->d_reclen
		// 			<< "; " << static_cast<int>(dp->d_type)
		// 			<< std::endl;
		// step x: If hidden file or dir
		if (dp->d_name[0] == '.')
			continue ;
		// step x: If dir
		else if (static_cast<int>(dp->d_type) == 4)
		{
			dir_content += "\t\t<p>";
			dir_content += dp->d_name;
			dir_content += '/';
			dir_content += "</p>\n";
		}
		else
		{
			dir_content += "\t\t<p>";
			dir_content += dp->d_name;
			dir_content += "</p>\n";
		}
	}
	(void)closedir(dirp);

	dir_content += "\t</body>\n</html>\n";

	// step 7: Create file and clean it
	std::ofstream	temp_file;
	std::string		cur_dir(full_path);
	cur_dir += "dir_content.temp";
	temp_file.open(cur_dir, std::ofstream::trunc);
	if (!temp_file.is_open())
		throw Exeption("ERROR in response_2: create temp_file error!");
	temp_file << dir_content;
	temp_file.close();

		// if (dp->d_reclen == len && !strcmp(dp->d_name, full_path.c_str())) {
		// 		(void)closedir(dirp);
		// 		return FOUND;
		// }


#ifdef DEBUG
	std::cout	<< "Response_2::ft_get_dir_list end; cur_dir = " << cur_dir
				<< "; ret data = " << dir_content
				<< std::endl;
#endif
	return (cur_dir);
}

int				Response_2::sendingResponseGet(std::string full_path, struct stat is_a_dir, std::string path)
{
#ifdef DEBUG
	std::cout	<< "Response_2::sendingResponseGet start: fd = " << _fd << std::endl;
#endif
	// step 1: Init data
	int ret;

	// step 2: Check it's file or dir
	lstat(full_path.c_str(), &is_a_dir);
	std::string rezult_path;
	if (S_ISDIR(is_a_dir.st_mode))
	{
		// step 2.1: Get index file
		std::string index_name = getIndexFileName(full_path);

		// step 2.2: If index file is not specified
		if (index_name == "")
		{
			// step 2.2.1: Check autoindex
			if (!_server->getAutoindex(path))
			{
				std::cerr << "this; " << path << "|" << std::endl;
				Headliners resp(std::string("HTTP/1.1"), std::string("403"));
				resp.sendHeadliners(_fd);
				return (-1);
			}
			// step 2.2.2: If autoindex on - get directory list
			else
			{
				rezult_path = ft_get_dir_list(full_path);
				// handler for create dirrect_list
			}
		}

		else
			rezult_path = full_path + index_name;
	}
	else
		rezult_path = full_path;

	// step 3: If have cgi go handle
	if (haveCGI(rezult_path))
		rezult_path = handleCGI(rezult_path);
	
	// step x: Get info about target file
	struct stat	info;
	stat(rezult_path.c_str(), &info);

	// step 4: Open the requested file and read in buffer
	std::ifstream	fileIndex;
	fileIndex.open(rezult_path);
	if (!fileIndex.is_open()){
		Headliners resp(std::string("HTTP/1.1"), std::string("404"));
		resp.sendHeadliners(_fd);

		std::ifstream err_404(_server->getErrPage());
		std::string str;
		while(std::getline(err_404, str))
			send(_fd, str.c_str(), str.size(), 0);

		std::cerr	<< "ERROR in sendingResponseGet: Target file open error" << std::endl;
		return (-1);
	}

	// step 5: Send headers "200 OK"
	Headliners resp(std::string("HTTP/1.1"), std::string("200"));
	resp.setCloseConnection(false);
	// resp.setContentLeigth(info.st_size);
	resp.sendHeadliners(_fd);

	// step 6: Send body
	fileIndex.close();
	int		fd_from;
	fd_from = open(rezult_path.c_str(), O_RDONLY);
	ret = sendfile(_fd, fd_from, NULL, info.st_size);
	// std::string str;
	// while(std::getline(fileIndex, str))
	// {
	// 	ret = send(_fd, str.c_str(), str.length(), 0);
	// 	std::cout << "send:" << ret << std::endl;
	// }
	
	// step 7: Remove temp file
	if (rezult_path.size() && rezult_path.find(".temp", 0) == (rezult_path.size() - 5))
		remove(rezult_path.c_str());

	// step 8: Close target file
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
// #ifdef DEBUG
	std::cout	<< "Response_2::haveCGI start; path = " << result_path << std::endl;
// #endif
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
	std::vector<char *> envp;

	// step 4: Add adress script_file in argv
	char		dir[100];
	getcwd(dir, 100);
	std::string cur_dir(dir);
	result_path.erase(0, 1);
	cur_dir += result_path;
	char *str = new char[cur_dir.size() + 1];
	std::copy(cur_dir.begin(), cur_dir.end(), str);
	str[cur_dir.size()] = '\0';
	// argv.push_back(str);

	// step 3: Add "-f" flag for php scripts
	char temp[] = "-f";
	if (_server->getCGI_format() == ".php" || _server->getCGI_format() == ".py")
	{
		// argv.push_back(temp);
		argv = { temp, str, NULL };
	}
	else
	{
		argv = { str, NULL };
	}

	// step 5: Create envp and add env vars
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
		envp.push_back(NULL);
	}
	else
		envp = { NULL };

	// step 6: Construct file_name for result CGI handler
	std::string::iterator	end = cur_dir.end();
	end--;
	while (cur_dir.size() && *end != '.')
		cur_dir.erase(end--);
	cur_dir.erase(end);
	cur_dir += ".html";

	// step 7: Create file and clean it
	// std::ofstream	temp_file;
	// temp_file.open(cur_dir, std::ofstream::trunc);
	// if (!temp_file.is_open())
	// 	throw Exeption("ERROR in response_2: create temp_file error!");
	// temp_file.close();

	
	// step 8: execute handler
	int		id = 0;
	id = fork();
	if (id == -1)
	{
		Headliners resp(std::string("HTTP/1.1"), std::string("500"));
		resp.sendHeadliners(_fd);
		throw Exeption("ERROR in response_2: create process for CGI handlerr error");
	}
	else if (id == 0)
	{
		// step x: Init data
		int ret = 0;
		FILE	*rek;

		// step x: Redirect stdout in file
		rek = freopen(cur_dir.c_str(), "w+", stdout);
		if (!rek)
		{
			Headliners resp(std::string("HTTP/1.1"), std::string("500"));
			resp.sendHeadliners(_fd);
			std::cerr	<< "ERROR CGI: Redirection stdout faill" << std::endl;
			exit(0);
		}

		// step x: Execute script
		if ((ret = execve((_server->getCGI_handler()).c_str(), &(*argv.begin()), &(*envp.begin()))) == -1)
		{
			Headliners resp(std::string("HTTP/1.1"), std::string("500"));
			resp.sendHeadliners(_fd);

			std::cerr	<< "ERROR CGI: execute CGI handler error" << std::endl;
		}

		// step x: If hane errors close and exit
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