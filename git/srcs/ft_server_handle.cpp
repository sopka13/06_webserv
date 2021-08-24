/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_server_handle.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eyohn <sopka13@mail.ru>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/19 10:57:36 by eyohn             #+#    #+#             */
/*   Updated: 2021/08/24 09:02:43 by eyohn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/headers.hpp"

int			ft_server_handle(t_vars* vars, std::string &str)
{
#ifdef DEBUG
	std::cout << "ft_server_handle start" << std::endl;
#endif
	vars->servers->push_back(Server(str));

#ifdef DEBUG
	std::cout << "ft_server_handle end" << std::endl;
#endif
	return (0);
}