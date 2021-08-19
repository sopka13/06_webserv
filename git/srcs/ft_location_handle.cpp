/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_location_handle.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eyohn <sopka13@mail.ru>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/19 10:55:52 by eyohn             #+#    #+#             */
/*   Updated: 2021/08/19 15:50:25 by eyohn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/headers.hpp"

int			ft_location_handle(t_vars* vars, std::string &str)
{
	std::map<std::string, int (*)(t_vars*, std::string&)> functions = {
		{"root", ft_root_handle},
		{"autoindex", ft_autoindex_handle},
		{"redirect", ft_redirect_handle}
	};

	return (0);
}