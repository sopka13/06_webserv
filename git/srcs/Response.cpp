#include "../includes/Response.hpp"

static int			setMetod(std::string &sock_buff)
{
	int		ret = 0;

	if (sock_buff.compare(0, 3, "GET") == 0)
	{
		sock_buff.erase(0, 3);
		ret = 1;
	}
	else if (sock_buff.compare(0, 4, "POST") == 0)
	{
		sock_buff.erase(0, 4);
		ret = 2;
	}
	else if (sock_buff.compare(0, 4, "HEAD") == 0)
	{
		sock_buff.erase(0, 4);
		ret = 3;
	}
	else if (sock_buff.compare(0, 6, "DELETE") == 0)
	{
		sock_buff.erase(0, 6);
		ret = 4;
	}
	return (ret);
}

static std::string	erase_back(std::string &str){
	int i = 0;
	while (str[i] && (str[i] == ' ' || str[i] == '\n' || str[i] == '\r' || str[i] == '\t'))
		++i;
	str.erase(0, i);
	return (str);
}

static std::string	setPath(std::string &str){
	std::string path = "";
	int i = 0;
	while (str[i] && str[i] != ' '&& str[i] != '\t' && str[i] != '\n' && str[i] != '\r'){
		path += str[i];
		++i;
	}
	str.erase(0, i);
	return (path);
}

Response::Response(){}

Response::~Response(){}

//Response::Response(const Response& resp){}

//Response& Response::operator= (const Response& resp){}

Response::Response(std::string &str):
	_metod(0),
	_flag_connect(false)
{
	// step 1: Get method
	this->_metod = setMetod(str);
	if (!_metod)
	{
		std::string str("ERROR in response: method not supported");
		throw Exeption(str);
	}
	str = erase_back(str);

	// step 2: Get path
	this->_path = setPath(str);
	if (!_path.size())
	{
		std::string str("ERROR in response: path missed");
		throw Exeption(str);
	}
	str = erase_back(str);

	// step 3: Get http
	_http = setPath(str);
	if (!_path.size())
	{
		std::string str("ERROR in response: http version missed");
		throw Exeption(str);
	}

	// step 4: If close connection set flag
	size_t connection_pos = str.find("Connection: close");
	if (connection_pos != std::string::npos)
		_flag_connect = true;
}

int					Response::getMetod(){
	return (_metod);
}

std::string			Response::getHttp(){
	return (_http);
}

std::string			Response::getPath(){
	return (_path);
}

bool				Response::getClose(){
	return (_flag_connect);
}