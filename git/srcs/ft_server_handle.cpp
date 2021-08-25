/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_server_handle.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eyohn <sopka13@mail.ru>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/19 10:57:36 by eyohn             #+#    #+#             */
/*   Updated: 2021/08/25 08:46:14 by eyohn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/headers.hpp"

int			ft_server_handle(t_vars* vars, std::string &str)
{
#ifdef DEBUG
	std::cout << "ft_server_handle start: size servers = " << vars->servers->size() << std::endl;
#endif
	vars->servers->emplace_back(Server(str));

#ifdef DEBUG
	std::cout << "ft_server_handle end: size servers = " << vars->servers->size() << std::endl;
#endif
	return (0);
}