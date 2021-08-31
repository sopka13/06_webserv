#include "../includes/Response.hpp"
#include <iostream>
#include <string>

int setMetod(std::string sock_buff){
    int metod = 0;
	if (sock_buff.compare(0, 3, "GET") == 0)
		metod = 1;
	else if (sock_buff.compare(0, 4, "POST") == 0)
		metod = 2;
	else if (sock_buff.compare(0, 4, "HEAD") == 0)
		metod = 3;
	else if (sock_buff.compare(0, 6, "DELETE") == 0)
		metod = 4;
	return (metod);
}
std::string erase_back(std::string str){
	int i = 0;
	while (str[i] && str[i] != ' ')
		++i;
	while (str[i] && (str[i] == ' ' || str[i] == '\n' || str[i] == '\r' || str[i] == '\t'))
		++i;
	str.erase(0, i);
	return (str);
}

std::string setPath(std::string str){
	std::string path = "";
	int i = 0;
	while(str[i] && str[i] != ' ' && str[i] != '\n' && str[i] != '\r'){
		path += str[i];
		++i;
	}
	return (path);
}

Response::Response(){}
Response::Response(std::string str){
    //std::string split_str = split(str, ' ')
    this->_metod = setMetod(str);
	str = erase_back(str);
	this->_path = setPath(str);
	str = erase_back(str);
	_http = setPath(str);
	str = erase_back(str);
	std::cout <<_metod << " " << _path << " " << _http << std::endl;
}
Response::~Response(){}
//Response::Response(const Response& resp){}
//Response& Response::operator= (const Response& resp){}
int Response::getMetod(){
	return (_metod);
}
std::string Response::getHttp(){
	return (_http);
}
std::string Response::getPath(){
	// std::cout << "Return path = " << _path << std::endl;
	return (_path);
}