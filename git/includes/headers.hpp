/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   headers.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eyohn <sopka13@mail.ru>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/13 17:08:32 by eyohn             #+#    #+#             */
/*   Updated: 2021/08/25 21:43:45 by eyohn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#define DEBUG 1

#define IP_ADDRESS "127.0.0.10"
#define PORT 8080
#define BUF_FOR_RESP 1024							// buff for response to client
#define DEF_ADR_CONF_FILE "./conf/webserv.conf"		// default config file

#include <iostream>
#include <fstream>			//ifstream
#include <sys/types.h>
#include <sys/socket.h>		//socket
#include <netdb.h>			//gethostbyname
#include <stdlib.h>			//exit		FORBIDDEN
#include <arpa/inet.h>		//inet_addr	FORBIDDEN
#include <unistd.h>			//write		FORBIDDEN
#include <errno.h>			//errno		FORBIDDEN
#include <string.h>			//strerror	FORBIDDEN
#include <map>				//map
#include <iterator>			//iterator
#include <deque>			//deque
#include <vector>			//vector
#include <string>			

class Socket;
class Server;


typedef struct		s_request
{
	int				metod;
	int				version;
	std::string		host;
}					t_request;

typedef struct		s_socket
{
	int						tcp_sockfd;				// socket fd
	struct sockaddr_in		serv_addr;				// name for ipv4
	socklen_t				sock_len;				// length of serv_addr
}					t_socket;

// typedef struct		s_location
// {
// 	/* data */
// }					t_location;


typedef struct 		s_server
{
	std::string							ip;					// ip adress
	int									port;				// 1 : 65535
	std::vector<std::string>			server_name;		// server name
	bool								default_server;		// default flag
	bool								autoindex;			// autoindex
	bool								redirect;			// redirect (on / off)
	std::string							redirect_location;	// redirect location
	std::string							redirect_adress;	// adress for redirect
	t_socket							sock_data;			// data for socket
}					t_server;


typedef struct		s_vars
{
	int					argc;
	char				**argv;
	char				**envp;
	std::ofstream		*log_file;						// logfile
	std::string			config_file_name;				// configuration file name
	std::string			max_body_size;					// client max body size
	int					ret;							// return value
	std::deque<Server>	*servers;						// all supported servers
	std::vector<Socket>	*sockets;						// all listen sockets
}					t_vars;


#include "../includes/Socket.hpp"
#include "../includes/Server.hpp"


void		ft_bzero(void *s, size_t n);
int			ft_check_args_files(t_vars *vars);
int			ft_client_max_body_size_handle(t_vars* vars, std::string &str);
void		ft_exit(t_vars *vars);
std::string	ft_get_name_conf(std::string &str);
int			ft_http_handle(t_vars* vars, std::string &str);
void		ft_init_data(t_vars *vars, int argc, char** argv, char** envp);
int			ft_log_file_handle(t_vars* vars, std::string &str);
void		*ft_memset(void *s, int c, size_t n);
int			ft_parse_config(t_vars* vars);
int			ft_server_handle(t_vars* vars, std::string &str);
char		*ft_strcpy(char *dest, char *src);
size_t		ft_strlen(const char *s);
int			ft_strtrim(std::string &str, std::string chars);
int			main(int argc, char **argv, char **envp);
