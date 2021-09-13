#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include "../includes/headers.hpp"
#include <string>

class Response{
public:
	Response(std::string &str);
	~Response();
	//Response(const Response& resp);
	//Response& operator= (const Response& resp);
	int				getMetod();
	std::string		getHttp();
	std::string		getPath();
	bool			getClose();
	
private:
	Response();
	int _metod;
	std::string _http;
	std::string _path;
	bool _flag_connect = false;

};
#endif