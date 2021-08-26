/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_server_handle.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eyohn <sopka13@mail.ru>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/19 10:57:36 by eyohn             #+#    #+#             */
/*   Updated: 2021/08/26 08:39:30 by eyohn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** This function handle the server section
**	return (0) - Success
**	return (1) - Error
*/

#include "../includes/headers.hpp"

int			ft_server_handle(t_vars* vars, std::string &str)
{
#ifdef DEBUG
	std::cout << "ft_server_handle start: size servers = " << vars->servers->size() << std::endl;
#endif
	try
	{
		vars->servers->emplace_back(Server(str));
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return (1);
	}
	

#ifdef DEBUG
	std::cout << "ft_server_handle end: size servers = " << vars->servers->size() << std::endl;
#endif
	return (0);
}