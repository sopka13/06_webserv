/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_server_handle.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eyohn <sopka13@mail.ru>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/19 10:57:36 by eyohn             #+#    #+#             */
/*   Updated: 2021/08/19 15:50:26 by eyohn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/headers.hpp"

int			ft_server_handle(t_vars* vars, std::string &str)
{
	std::map<std::string, int (*)(t_vars*, std::string&)> functions = {
		{"listen", ft_listen_handle},
		{"server_name", ft_server_name_handle},
		{"location", ft_location_handle}
	};

	return (0);
}