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
	else if (sock_buff.compare(0, 3, "PUT") == 0)
	{
		sock_buff.erase(0, 3);
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
	while (str[i] && str[i] != ' ' && str[i] != '\t' && str[i] != '\n' && str[i] != '\r'){
		path += str[i];
		++i;
	}
	str.erase(0, i);
	return (path);
}

std::string			Response::setBody(std::string str){
	std::string::iterator it = str.begin();
	size_t body_pos = str.find("\n\n");
	if (body_pos == std::string::npos)
		body_pos = str.find("\r\n\r\n");
	else
		it += body_pos + 2;
	if (body_pos == std::string::npos)
		std::cout << "no body" << std::endl;
	else
		it += body_pos + 4;
	std::string body = "";
	while (it != str.end()){
		body += *it;
		++it;
	}
	return (body);
}

void				Response::setBodySize(){
	_body_size = _body.length();
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
	if (_metod == 0)
	{
		std::string str_1("ERROR in response: method not supported"); // 21.09.21 need engine for response errors page
		std::cout << "fff " << str << std::endl;
		throw Exeption(str_1);
	}
	str = erase_back(str);

	// step 2: Get path
	this->_path = setPath(str);
	if (!_path.size())
	{
		std::string str_1("ERROR in response: path missed");
		throw Exeption(str_1);
	}
	str = erase_back(str);

	// step 3: Get http
	_http = setPath(str);
	if (!_path.size())
	{
		std::string str_1("ERROR in response: http version missed");
		throw Exeption(str_1);
	}

	// step 4: If close connection set flag
	size_t connection_pos = str.find("Connection: close");
	if (connection_pos != std::string::npos)
		_flag_connect = true;

	// step 5: Get connection_length
	connection_pos = str.find("Content-Length");
	if (connection_pos != std::string::npos){
		std::string::iterator it = str.begin();
		it += connection_pos + 16;
		std::string con_l = "";
		while (*it != ' ' && *it != '\n'){
			con_l += *it;
			++it;
		}
		_con_len = std::stoi(con_l);
		// std::cout << "CON =" << con_l << "R" << std::endl;
	}
	
	// step 6: Get body of reqvest
	_body = setBody(str);
	// std::cout << "body =  " << _body << std::endl;

	// step 7: Get body size
	setBodySize();
	// std::cout << "body_size =  " << _body_size << std::endl;
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

std::string			Response::getBody(){
	// std::cout << "BODY " << _body << std::endl;
	return (_body);
}

size_t				Response::getBodySize(){
	// std::cout << "SIZE " << _body_size << std::endl;
	return (_body_size);
}

size_t				Response::getConLen(){
	// std::cout << "CON_LEN " << _con_len << std::endl;
	return (_con_len);
}
