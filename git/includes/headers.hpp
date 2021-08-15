/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   headers.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eyohn <sopka13@mail.ru>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/13 17:08:32 by eyohn             #+#    #+#             */
/*   Updated: 2021/08/15 15:21:54 by eyohn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
// #include <sys/types.h>
#include <sys/socket.h>		//socket
#include <netdb.h>			//gethostbyname
#include <stdlib.h>			//exit
#include <arpa/inet.h>		//inet_addr FORBIDDEN
#include <unistd.h>			//write FORBIDDEN
#include <errno.h>			//errno FORBIDDEN
#include <string.h>			//strerror FORBIDDEN

#define IP_ADDRESS "127.0.0.10"
#define PORT 8080

int			main(int argc, char **argv, char **envp);
void		*ft_memset(void *s, int c, size_t n);
char		*ft_strcpy(char *dest, char *src);
size_t		ft_strlen(const char *s);