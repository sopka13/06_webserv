/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_request.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eyohn <sopka13@mail.ru>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/16 09:04:39 by eyohn             #+#    #+#             */
/*   Updated: 2021/08/20 11:21:25 by eyohn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** This function handle request from client
**	return (0) - Success
**	return (1) - Error
*/

#include "../includes/headers.hpp"
#include <sstream>
#include <ctime>

static std::string toString(int a)
{
	std::ostringstream oss;
	oss << a;
	return (oss.str());
}

int			ft_handle_request(t_vars* vars)
{
	// step 1: Read data from client
	vars->ret = recv(vars->sock_data.fd, &vars->sock_data.buff, sizeof(vars->sock_data.buff), 0);
	if (vars->ret < 0)
	{
		std::cout << "ERROR Read fail: " << strerror(errno) << std::endl;
		return (0);
	}
	write(1, vars->sock_data.buff, sizeof(vars->sock_data.buff));

	// step 2: Write data for client
	std::string	buff_1 = "Request accepted! Time: " + toString(static_cast<int>(clock()));
	vars->ret = send(vars->sock_data.fd, buff_1.c_str(), sizeof(buff_1), 0);
	if (vars->ret < 0)
	{
		std::cout << "ERROR Response fail: " << strerror(errno) << std::endl;
		return (0);
	}

	// step 3: close fc
	close(vars->sock_data.fd);	//FORBIDDEN

	return (0);
}