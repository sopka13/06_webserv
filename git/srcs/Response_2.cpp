/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response_2.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eyohn <sopka13@mail.ru>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/09 08:56:56 by eyohn             #+#    #+#             */
/*   Updated: 2021/10/13 13:36:33 by eyohn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Response_2.hpp"

static std::string	ft_remove_underscore(std::string path, std::string title)
{
#ifdef DEBUG
	std::cout	<< "ft_remove_underscore start: path = " << path << "; title = " << title << std::endl;
#endif
	while (path.size() && *(path.end() - 1) == '/')
		path.erase((path.end() - 1));
	while (title.size() && *(title.begin()) == '/')
		title.erase(title.begin());

#ifdef DEBUG
	std::cout	<< "ft_remove_underscore end: result = " << path + '/' + title << std::endl;
#endif
	return (path + '/' + title);
}

void		Response_2::getBlaCgiResult(Response *response, std::string full_path)
{
	// This function handle .bla CGI file
#ifdef DEBUG
	std::cout	<< "Response_2::getBlaCgiResult start; " << std::endl;
#endif
	// step 1: Init data
	int		ret = 0;		// return value
	int		id;				// if child CGI script process
	int		pip[2];			// pipe for send body for script
	int		pop[2];			// pipe for send response from CGI

	// step x: Create pipe
	if ((ret = pipe(pip)) == -1 || (ret = pipe(pop)) == -1)
	{
		Headliners resp(std::string("HTTP/1.1"), std::string("500"));
		resp.sendHeadliners(_fd);
		throw Exeption("ERROR in response_2: create pipe for CGI handlerr error");
	}

	// step x: Fork
	id = fork();
	if (id == -1)			// error
	{
		Headliners resp(std::string("HTTP/1.1"), std::string("500"));
		resp.sendHeadliners(_fd);
		throw Exeption("ERROR in response_2: create process for CGI handlerr error");
	}
	else if (id == 0)		// child
	{
		// step x: Close pipe
		if ((ret = close(pip[1])) == -1 || (ret = close(pop[0])) == -1)
		{
			Headliners resp(std::string("HTTP/1.1"), std::string("500"));
			resp.sendHeadliners(_fd);
			throw Exeption("ERROR in response_2: close pipe[1] for CGI handlerr error");
		}

		// step x: Change fd
		if ((ret = dup2(pip[0], STDIN_FILENO)) == -1 || (ret = dup2(pop[1], STDOUT_FILENO)) == -1)
		{
			Headliners resp(std::string("HTTP/1.1"), std::string("500"));
			resp.sendHeadliners(_fd);
			throw Exeption("ERROR in response_2: close pipe[1] for CGI handlerr error");
		}

		// step x: Close fd
		close(pip[0]);
		close(pop[1]);

		// step x: Create env vars
		char*	argv[] = {
			// (_server->getCGI_handler()).c_str(),
			NULL
		};
		std::vector<char *> envp;
		char	tmp[] = "REDIRECT_STATUS=200"; 
		envp.push_back(tmp);
			// "CONTENT_LENGTH"]	=	iToString(this->_req.body_size);	// content-length de la requete
			// "CONTENT_TYPE"]		=	headers["content-type"];	// content-type de la requete (POST)
		char	tmp1[] = "GATEWAY_INTERFACE=CGI/1.1";	// version du CGI qu'utilise le server
		envp.push_back(tmp1);
		char	tmp2[] = "PATH_INFO=/home/sergey/My_prog/21_school/06_webserv/git/sites_avaliable/test/ubuntu_cgi_tester";	// derniere partie de l'URI apres le host
		envp.push_back(tmp2);
			// "PATH_TRANSLATED"]	=	this->_res.getTarget();	// adresse reelle du script (idem PATH_INFO pour nous)
			// "QUERY_STRING"]		=	this->_req.req_line.query_string;	// Contient tout ce qui suit le « ? » dans l'URL envoyée par le client.
			// "REMOTE_ADDR"]		=	this->_req.host_uri;;	// adress ip du client
		char	tmp3[] = "REQUEST_METHOD=GET;POST";	// GET ou POST ou ...
		envp.push_back(tmp3);
			// "REQUEST_URI"]		=	this->_req.req_line.target; // --> For the 42 tester
			// "SCRIPT_NAME"]		=	this->_res.getTarget();	// full path du fichier de script
			// "SCRIPT_FILENAME"]	=	this->_res.getTarget();	// full path du fichier de script
			// "SERVER_NAME"`]		=	this->_req.host_uri;	// DNS ou IP du server (hostname)
			// "SERVER_PORT"]		=	this->_req.host_port;	// port ayant reçu la requête
		char	tmp4[] = "SERVER_PROTOCOL=HTTP/1.1";	// protocol HTTP (toujours HTTP/1.1 ?)
		envp.push_back(tmp4);
			// "SERVER_SOFTWARE"]	=	"webserv";
			// "UPLOAD_DIR"]		=	this->_req.config.upload_dir;
		envp.push_back(NULL);

		// step x: Execute script
		if ((ret = execve((_server->getCGI_handler()).c_str(), argv, &(*envp.begin()))))
		{
			Headliners resp(std::string("HTTP/1.1"), std::string("500"));
			resp.sendHeadliners(_fd);
			throw Exeption("ERROR in response_2: execute script error");
		}

	}

	// step x: Close pipe
	if ((ret = close(pip[0])) == -1 || (ret = close(pop[1])) == -1)
	{
		Headliners resp(std::string("HTTP/1.1"), std::string("500"));
		resp.sendHeadliners(_fd);
		throw Exeption("ERROR in response_2: close pipe[0] for CGI handlerr error");
	}

	// step x: Send body
	if ((ret = write(pip[1], (response->getBody()).c_str(), response->getBodySize())) == -1)
	{
		Headliners resp(std::string("HTTP/1.1"), std::string("500"));
		resp.sendHeadliners(_fd);
		throw Exeption("ERROR in response_2: write in pipe for CGI handlerr error");
	}

	// step x: Wait child process
	int		status;
	waitpid(id, &status, 0);

	// step x: Create file
	std::string		file_name(full_path + ".temp");
	std::ofstream	temp_file;
	temp_file.open(file_name.c_str());
	if (!temp_file.is_open())
	if ((ret = write(pip[1], (response->getBody()).c_str(), response->getBodySize())) == -1)
	{
		Headliners resp(std::string("HTTP/1.1"), std::string("500"));
		resp.sendHeadliners(_fd);
		throw Exeption("ERROR in response_2: create file for CGI handlerr error");
	}

	// step x: Read result execute CGI script
	char		bufer[BUF_FOR_RESP];
	ret = BUF_FOR_RESP;
	while (ret == BUF_FOR_RESP)
	{
		ft_bzero(bufer, BUF_FOR_RESP);
		ret = read(pop[0], bufer, BUF_FOR_RESP);
		temp_file << bufer;
		std::cerr << "buffer = " << bufer << std::endl;
	}

	// step x: Close file and pipes
	temp_file.close();
	close(pip[1]);
	close(pop[0]);

	// step x: Send file
	sendFile(file_name);

#ifdef DEBUG
	std::cout	<< "Response_2::getBlaCgiResult end" << std::endl;
#endif
}

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
	// std::cerr << "dctor" << std::endl;
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
// #ifdef DEBUG
	std::cout	<< "Response_2::sendFile start; path = " << full_path << std::endl;
// #endif
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
	return ;
}

void			Response_2::postHandle(Response *response)
{
	// This function handle POST method and has the following logic:
	// 	-- 0. if body size larger than the allowed one return error
	//	If have .bla go to getBlaCgiResult
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
	// // step 0: Check body size
	// if (static_cast<int>(response->getBodySize()) > _server->getMaxBodySize())
	// {
	// 	Headliners resp(std::string("HTTP/1.1"), std::string("400"));
	// 	resp.setCloseConnection(false);
	// 	resp.sendHeadliners(_fd);

	// 	std::string str("ERROR in ft_post_handle: Body size too long");
	// 	throw Exeption(str);
	// }

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

	// step x: If .bla
	if (haveCGI(full_path) && _server->getCGI_format() == ".bla")
	{
		getBlaCgiResult(response, full_path);
		return ;
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
		temp = ft_remove_underscore(_server->getLocations(path), full_path);
		full_path = temp;
		full_path += getIndexFileName(full_path);
		// std::cerr << "Full path = " << full_path << std::endl;
	}

	// step 4: Check error - if method no supported
	// std::string		temp_1(response->getPath());
	if (!_server->getMethods(path, method)/* && !(haveCGI(temp_1) && _server->getCGI_format() == ".bla")*/)
	{
		Headliners resp(std::string("HTTP/1.1"), std::string("405"));
		resp.setCloseConnection(false);
		resp.sendHeadliners(_fd);

		return ;
	}

	// step 5: Check target file/dir for exist
	struct stat	info;
	ret = stat(full_path.c_str(), &info);
	if (ret == -1 || (haveCGI(full_path) && _server->getCGI_format() == ".bla"))							//file/dir doesn't exist
	{
		// step 1: Create new file and write body
		std::ofstream	new_file;
		new_file.open(full_path.c_str());
		new_file << response->getBody();
		new_file.close();
		ret = stat(full_path.c_str(), &info);

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
			if (target_file.size() && target_file.find(".temp", 0) == (target_file.size() - 5))
				remove(target_file.c_str());
			return ;
		}
		else								//target_file is not CGI
		{
			if (S_ISDIR(info.st_mode))	//it's a directory
			{
				// step 1: Create new file and write body
				std::ofstream	new_file;
				new_file.open(full_path.c_str());
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
// #ifdef DEBUG
	std::cout	<< "Response_2::sendResponse start: fd = " << _fd << "; size container = " << _requests.size() << std::endl;
// #endif
	// step 1: Init data
	int						ret = 0;
	std::string				path;
	std::string				tile;
	std::string::iterator	slesh;

	// step 2: If no unhandled request
	if (!_requests.size())
	{
		// send(_fd, "\0\0", 2, 0);
		// std::cerr << "end" << std::endl;
		// Headliners resp(std::string("HTTP/1.1"), std::string("200"));
		// resp.setCloseConnection(false);
		// resp.sendHeadliners(_fd);
		// sendFile(_server->getWelcomePage());

		// have data monitoring only
		// (_server->getEpollEvent())->events = EPOLLIN;
		// (_server->getEpollEvent())->data.fd = _fd;
		// ret = epoll_ctl(_server->getEpollFd(), EPOLL_CTL_MOD, _fd, _server->getEpollEvent());
		// std::cerr << "ret = " << ret << " err = " << errno << " fd = " << _fd << std::endl;

		if (epoll_ctl(_server->getEpollFd(), EPOLL_CTL_DEL, _fd, _server->getEpollEvent()) == -1)
		{
			throw Exeption("ERROR in Response_2::sendResponse: Epoll_ctl del error");
		}
		if ((ret = close(_fd)) == -1)
			std::cerr << "FAIL!!!" << std::endl;

		// delete element from request container
		((_server->getRequestContainerPointer())->operator[](_fd))->~Response_2();
		(_server->getRequestContainerPointer())->erase(_fd);
		// if (ret == -1) // error handle

		return (0);
	}
		// return (1); 

	// step 1: Cycle of send response ??? do we need

	// step 3: Parse response
	int		flag = 0;
	try
	{
		Response response(_requests.operator[](0), _fd, _server->getMaxBodySize());
		flag = 1;
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
			std::string full_path = ft_remove_underscore(_server->getLocations(path), tile);
			ret = sendingResponseGet(&response, full_path, is_a_dir, path);
		}

		m = "PUT";
		if (response.getMetod() == 3 &&
			(_server->getLocations(path) != "") &&
			_server->getMethods(path, m))
		{
			std::string full_path = ft_remove_underscore(_server->getLocations(path), tile);
			int i = lstat(full_path.c_str(), &is_a_dir);
			std::ofstream file;

			std::cerr << "BAD FILE = " << full_path << std::endl;

			file.open(full_path.c_str());
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
		m = "DELETE";
		if (response.getMetod() == 4 &&
			(_server->getLocations(path) != "") &&
			_server->getMethods(path, m))
		{
			std::string full_path = ft_remove_underscore(_server->getLocations(path), tile);
			int rez = remove(full_path.c_str());
			if (rez >= 0){
				Headliners resp(std::string("HTTP/1.1"), std::string("200"));
				resp.sendHeadliners(_fd);
			}
			else {
				std::cout << "rez " << rez << std::endl;
				Headliners resp(std::string("HTTP/1.1"), std::string("500"));
				resp.sendHeadliners(_fd);
			}
		}
	}
	catch(const std::exception& e)
	{
		if (_requests.size() && flag == 0)
		{
			_requests.pop_front();
		}
		std::cerr << e.what() << '\n';
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
	// fd_set			rfd;
	// fd_set			wfd;
	std::string		data;
	// struct timeval	tv;
	// tv.tv_sec = 0;
	// tv.tv_usec = 150;

	fcntl(_fd, F_SETFL, O_NONBLOCK);

	// step 2: Cycle for read data from fd in _buff
	while (1)
	{
		if (TEST)
			usleep(200);
		// step 2.1: Read
		// ret = read(_fd, _buff, sizeof(_buff));
		ret = recv(_fd, _buff, sizeof(_buff), 0);
		std::cerr << "ret = " << ret << "; fd = " << _fd << std::endl;
		// if (ret < 0)
		// {
		// 	Headliners resp(std::string("HTTP/1.1"), std::string("403"));
		// 	resp.sendHeadliners(_fd);
		// 	std::string str("ERROR in get response: read fail");
		// 	throw Exeption(str);
		// }

		// If first cycle and ret == 0
		if (ret <= 0 && data.size() == 0)// ||
			//(data.size() > (_server->getMaxBodySize() + sizeof(_buff)) && data.find("POST", 0) == 0))
		{
			std::cerr << "EPOLLERR: fd = " << _fd << std::endl;
			if ((_server->getRequestContainerPointer())->size() &&
				(_server->getRequestContainerPointer())->find(_fd) != (_server->getRequestContainerPointer())->end())
			{
				if (epoll_ctl(_server->getEpollFd(), EPOLL_CTL_DEL, _fd, _server->getEpollEvent()) == -1)
				{
					// std::cerr << "strerror = " << strerror(errno) << "; errno = " << errno << std::endl;
					throw Exeption("ERROR in Response_2::readRequest: Epoll_ctl del error");
				}
				((_server->getRequestContainerPointer())->operator[](_fd))->~Response_2();
				(_server->getRequestContainerPointer())->erase(_fd);
			}

			// if (data.size() > (_server->getMaxBodySize() + sizeof(_buff)))
			// {
			// 	break ;
			// 	Headliners resp(std::string("HTTP/1.1"), std::string("200"));
			// 	resp.setCloseConnection(true);
			// 	resp.sendHeadliners(_fd);

			// 	if ((ret = close(_fd)) == -1)
			// 		std::cerr << "FAIL!!!" << std::endl;

			// 	std::cerr << "--\n" << data << "\n--" << std::endl;

			// 	std::string str("ERROR in ft_post_handle: Body size too long");
			// 	throw Exeption(str);
			// }

			if ((ret = close(_fd)) == -1)
				std::cerr << "FAIL!!!" << std::endl;
			break ;
			// delete element from request container
			// std::cerr << "Close fine (Response_2)" << std::endl;
		}


		if (ret <= 0) // if no data in fd
		{
			break ;
			// Headliners resp(std::string("HTTP/1.1"), std::string("100"));
			// resp.sendHeadliners(_fd);
		}



		// step 2.2: Accumulate received data and clean _buff
		// else
		// {
			data += _buff;
			ft_bzero(&_buff, sizeof(_buff));
		// 	continue ;
		// }

		// // step 2.3: Clear data for select
		// FD_ZERO(&rfd);
		// // FD_ZERO(&wfd);
		// FD_SET(_fd, &rfd);
		// // FD_SET(_fd, &wfd);


		// // step 2.4: Check action on client fd
		// ret = select(_fd + 1, &rfd, 0, 0, &tv);
		// //		if error
		// if (ret < 0)
		// {
		// 	Headliners resp(std::string("HTTP/1.1"), std::string("500"));
		// 	resp.sendHeadliners(_fd);
		// 	std::string str("ERROR in ft_handle_epoll_fd: select fall");
		// 	throw Exeption(str);
		// }
		// //		if no actions
		// else if (ret == 0)
		// 	break ;
		// //		if have actions
		// else
		// {
		// 	if (FD_ISSET(_fd, &rfd))
		// 		continue ;
		// 	// if (FD_ISSET(_fd, &wfd))
		// 	break ;
		// }
	}

	// step 3: Add request in container if have any data from fd
	if (data.size())
	{
		_requests.push_back(data);

		(_server->getEpollEvent())->events = EPOLLIN | EPOLLOUT;
		(_server->getEpollEvent())->data.fd = _fd;
		ret = epoll_ctl(_server->getEpollFd(), EPOLL_CTL_MOD, _fd, _server->getEpollEvent());
	}
	std::cerr << "--\n" << data << "\n--" << std::endl;

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
	std::cout	<< "Response_2::ft_get_dir_list start; full path = " << full_path << std::endl;
#endif
	// step 1: Init data
	DIR*			dirp;
	struct dirent*	dp;
	std::string		dir_content("<html>\n\t<body>\n");
	std::cerr << "step 1 ok" << std::endl;


	// step 2: Construct html
	dirp = opendir(full_path.c_str());
	std::cerr << "step 2 ok" << std::endl;
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

	// step 3: Create file and write data
	std::ofstream	temp_file;
	std::string		cur_dir(full_path);
	cur_dir += "/dir_content.temp";
	temp_file.open(cur_dir.c_str(), std::ofstream::trunc);
	if (!temp_file.is_open())
		throw Exeption("ERROR in response_2: create temp_file error!");
	temp_file << dir_content;
	temp_file.close();
	std::cerr << "step 3 ok" << std::endl;

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

int				Response_2::sendingResponseGet(Response *response, std::string full_path, struct stat is_a_dir, std::string path)
{
// #ifdef DEBUG
	std::cout	<< "Response_2::sendingResponseGet start: fd = " << _fd << "; full path = " << full_path << std::endl;
// #endif
	// step 1: Init data
	int ret;

	// step 2: Check it's file or dir
	lstat(full_path.c_str(), &is_a_dir);
	std::string rezult_path;
	if (S_ISDIR(is_a_dir.st_mode))
	{
		// step x: Add '/' character to the end of the line
		if (*(full_path.end() - 1) != '/')
			full_path += '/';

		// step 2.1: Get index file
		std::string index_name = getIndexFileName(full_path);

		// step 2.2: If index file is not specified
		if (index_name == "")
		{
			// step 2.2.1: Check autoindex
			if (!_server->getAutoindex(path))
			{
				// std::cerr << "this; " << path << "|" << std::endl;
				if (TEST)
				{
					Headliners resp(std::string("HTTP/1.1"), std::string("404")); 
					resp.sendHeadliners(_fd);
				}
				else
				{
					Headliners resp(std::string("HTTP/1.1"), std::string("403")); 
					resp.sendHeadliners(_fd);
				}
				return (-1);
			}
			// step 2.2.2: If autoindex on - get directory list
			else
			{
				// if (TEST)	// if start test
				// {
				// 	rezult_path = _server->getErrPage();
				// 	Headliners resp(std::string("HTTP/1.1"), std::string("404"));
				// 	resp.setCloseConnection(false);

				// 	// step x: Get info about target file
				// 	struct stat	info;
				// 	stat(rezult_path.c_str(), &info);

				// 	resp.setContentLeigth(info.st_size);
				// 	resp.sendHeadliners(_fd);

				// 	// step 6: Send body
				// 	int		fd_from;
				// 	fd_from = open(rezult_path.c_str(), O_RDONLY);
				// 	ret = sendfile(_fd, fd_from, NULL, info.st_size);
				// 	return (ret);
				// }
				// else		// else
					rezult_path = ft_get_dir_list(full_path);
					std::cerr << "; full path 1 = " << full_path << std::endl;
				// handler for create dirrect_list
			}
		}

		else
		{
			rezult_path = full_path + index_name;
			std::cerr << "; full path 2 = " << full_path << std::endl;
		}
	}
	else
	{
		rezult_path = full_path;
		std::cerr << "; full path 3 = " << full_path << std::endl;
	}

	// step 3: If have cgi go handle
	if (haveCGI(rezult_path) && !TEST)
		rezult_path = handleCGI(rezult_path);

	// step x: If have CGI .bla
	if (haveCGI(rezult_path) && _server->getCGI_format() == ".bla")
	{
		getBlaCgiResult(response, rezult_path);
		return (0);
	}
	
	// step x: Get info about target file
	struct stat	info;
	stat(rezult_path.c_str(), &info);

	// step 4: Open the requested file and read in buffer
	std::ifstream	fileIndex;

	std::cerr << "result path 4 = " << rezult_path << std::endl;

	fileIndex.open(rezult_path.c_str());
	if (!fileIndex.is_open()){
		Headliners resp(std::string("HTTP/1.1"), std::string("404"));
		resp.sendHeadliners(_fd);

		std::ifstream err_404((_server->getErrPage()).c_str());
		std::string str;
		while(std::getline(err_404, str))
			send(_fd, str.c_str(), str.size(), 0);

		std::cerr	<< "ERROR in sendingResponseGet: Target file open error" << std::endl;
		return (-1);
	}

	// step 5: Send headers "200 OK"
	Headliners resp(std::string("HTTP/1.1"), std::string("200"));
	resp.setCloseConnection(false);
	resp.setContentLeigth(info.st_size);
	resp.sendHeadliners(_fd);

	// step 6: Send body
	fileIndex.close();
	int		fd_from;
	fd_from = open(rezult_path.c_str(), O_RDONLY);

	std::cerr << "rezult_path = " << rezult_path << std::endl;

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

// #ifdef DEBUG
	std::cout	<< "Response_2::sendingResponseGet end: rezult_path = " << rezult_path << std::endl;
// #endif
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
		std::ifstream	file((path + name).c_str());
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
	std::cout	<< "Response_2::haveCGI start; path = " << result_path << std::endl;
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
// #ifdef DEBUG
	std::cout	<< "Response_2::handleCGI start; path = " << result_path << std::endl;
// #endif
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
	// cur_dir += '/';
	char *str = new char[cur_dir.size() + 1];
	std::copy(cur_dir.begin(), cur_dir.end(), str);
	str[cur_dir.size()] = '\0';
	// argv.push_back(str);

	// step 3: Add "-f" flag for php scripts
	char temp[] = "-f";
	if (_server->getCGI_format() == ".php" || _server->getCGI_format() == ".py")
	{
		// argv.push_back(temp);
		argv.push_back(temp);
		argv.push_back(str);
		argv.push_back(NULL);
	}
	else
	{
		argv.push_back(str);
		argv.push_back(NULL);
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
		envp.push_back(NULL);

	// step 6: Construct file_name for result CGI handler
	std::string::iterator	end = cur_dir.end();
	end--;
	while (cur_dir.size() && *end != '.')
		cur_dir.erase(end--);
	cur_dir.erase(end);
	cur_dir += ".temp";

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

int				Response_2::getRequestContainerSize()
{
	return (_requests.size());
}
