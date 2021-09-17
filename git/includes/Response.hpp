#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include "../includes/headers.hpp"
#include <cstddef>

class Response{
public:
	class	Exeption : public std::runtime_error {
		public:
			Exeption(const std::string& message) : std::runtime_error(message) {}
	};
	Response(std::string &str);
	~Response();
	//Response(const Response& resp);
	//Response& operator= (const Response& resp);
	int				getMetod();
	std::string		getHttp();
	std::string		getPath();
	bool			getClose();
	std::string 	setBody(std::string str);
	void 			setBodySize();
	std::string 	getBody();
	size_t 			getBodySize();
	size_t 			getConLen();
	
private:
	Response();
	int				_metod;
	std::string		_http;
	std::string		_path;
	bool			_flag_connect;
	std::string		_body;
	size_t			_body_size;
	size_t			_con_len;

};
#endif