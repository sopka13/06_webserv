/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   headers.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eyohn <sopka13@mail.ru>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/13 17:08:32 by eyohn             #+#    #+#             */
/*   Updated: 2021/08/20 15:23:40 by eyohn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

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
#include <iterator>

#define IP_ADDRESS "127.0.0.10"
#define PORT 8080
#define BUF_FOR_RESP 1024							// buff for response to client
#define DEF_ADR_CONF_FILE "./conf/webserv.conf"		// default config file

typedef struct		s_socket
{
	int						tcp_sockfd;				// socket fd
	struct sockaddr_in		serv_addr;				// name for ipv4
	socklen_t				sock_len;				// length of serv_addr
	int						fd;						// fd received after the acept call
	char					buff[BUF_FOR_RESP];		// buffer for read from client
}					t_socket;

typedef struct 		s_server
{
	std::string							ip;					// ip adress
	unsigned short						port;				// 1 : 65535
	std::string							*server_name;		// server name
	std::map<std::string, std::string>	locations;			// locations
	bool								autoindex;			// autoindex
	bool								redirect;			// redirect (on / off)
	std::string							redirect_adress;	// adress for redirect
}					t_server;


typedef struct		s_vars
{
	int				argc;
	char			**argv;
	char			**envp;
	t_socket		sock_data;						// data for socket
	std::ofstream	*log_file;						// logfile
	std::string		max_body_size;					// client max body size

	int				ret;							// return value
}					t_vars;




int			ft_autoindex_handle(t_vars* vars, std::string &str);
void		ft_bzero(void *s, size_t n);
int			ft_client_max_body_size_handle(t_vars* vars, std::string &str);
int			ft_create_socket(t_vars* vars);
void		ft_exit(t_vars *vars);
std::string	ft_get_name_conf(std::string &str);
int			ft_handle_request(t_vars* vars);
int			ft_http_handle(t_vars* vars, std::string &str);
void		ft_init_data(t_vars *vars, int argc, char** argv, char** envp);
int			ft_listen_handle(t_vars* vars, std::string &str);
int			ft_location_handle(t_vars* vars, std::string &str);
int			ft_log_file_handle(t_vars* vars, std::string &str);
void		*ft_memset(void *s, int c, size_t n);
int			ft_parse_config(t_vars* vars);
int			ft_redirect_handle(t_vars* vars, std::string &str);
int			ft_root_handle(t_vars* vars, std::string &str);
int			ft_server_handle(t_vars* vars, std::string &str);
int			ft_server_name_handle(t_vars* vars, std::string &str);
char		*ft_strcpy(char *dest, char *src);
size_t		ft_strlen(const char *s);
int			ft_strtrim(std::string &str, std::string chars);
int			main(int argc, char **argv, char **envp);
