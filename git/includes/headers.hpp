/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   headers.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eyohn <sopka13@mail.ru>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/13 17:08:32 by eyohn             #+#    #+#             */
/*   Updated: 2021/08/16 09:32:39 by eyohn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>		//socket
#include <netdb.h>			//gethostbyname
#include <stdlib.h>			//exit
#include <arpa/inet.h>		//inet_addr	FORBIDDEN
#include <unistd.h>			//write		FORBIDDEN
#include <errno.h>			//errno		FORBIDDEN
#include <string.h>			//strerror	FORBIDDEN

#define IP_ADDRESS "127.0.0.10"
#define PORT 8080
#define BUF_FOR_RESP 1024							// buff for response to client

typedef struct		s_socket
{
	int						tcp_sockfd;				// socket fd
	struct sockaddr_in		serv_addr;				// name for ipv4
	socklen_t				sock_len;				// length of serv_addr
	int						fd;						// fd received after the acept call
	char					buff[BUF_FOR_RESP];		// buffer for read from client
}					t_socket;

typedef struct		s_vars
{
	int				argc;
	char			**argv;
	char			**envp;
	t_socket		sock_data;						// data for socket
	int				ret;							// return value
}					t_vars;




int			main(int argc, char **argv, char **envp);
void		*ft_memset(void *s, int c, size_t n);
char		*ft_strcpy(char *dest, char *src);
size_t		ft_strlen(const char *s);
void		ft_bzero(void *s, size_t n);
void		ft_init_data(t_vars *vars, int argc, char** argv, char** envp);
int			ft_parse_config(t_vars* vars);
int			ft_create_socket(t_vars* vars);
int			ft_handle_request(t_vars* vars);
void		ft_exit();