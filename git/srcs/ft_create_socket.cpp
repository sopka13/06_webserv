/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_create_socket.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eyohn <sopka13@mail.ru>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/16 08:59:19 by eyohn             #+#    #+#             */
/*   Updated: 2021/08/16 10:08:19 by eyohn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** This function create socket
**	return (0) - Success
**	return (1) - Error
*/

#include "../includes/headers.hpp"

int			ft_create_socket(t_vars* vars)
{
	// step 1: Create socket
	vars->sock_data.tcp_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (vars->sock_data.tcp_sockfd < 0)
	{
		std::cout << "ERROR opening socket 1: " << strerror(errno) << std::endl;
		return (1);
	}

	// step 2: Assigning a name to a socket
	vars->ret = bind(vars->sock_data.tcp_sockfd, (struct sockaddr *) &vars->sock_data.serv_addr, vars->sock_data.sock_len);
	if (vars->ret < 0)
	{
		std::cout << "ERROR Assigning name to a socket fail: " << strerror(errno) << std::endl;
		return (1);
	}

	// step 3: Create queue connection (очередь)
	vars->ret = listen(vars->sock_data.tcp_sockfd, SOMAXCONN);
	if (vars->ret < 0)
	{
		std::cout << "ERROR Listening fail: " << strerror(errno) << std::endl;
		return (1);
	}
	return (0);
}