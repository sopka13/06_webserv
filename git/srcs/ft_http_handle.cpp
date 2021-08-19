/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_http_handle.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eyohn <sopka13@mail.ru>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/19 10:54:41 by eyohn             #+#    #+#             */
/*   Updated: 2021/08/19 15:50:25 by eyohn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/headers.hpp"

int			ft_http_handle(t_vars* vars, std::string &str)
{
	std::cout << "ft_http_handle start" << std::endl;

	std::map<std::string, int (*)(t_vars*, std::string&)> functions = {
		{"client_max_body_size", ft_client_max_body_size_handle},
		{"server", ft_server_handle}
	};
	// std::string::iterator	start = str.begin();
	
	// while (str.length() && *start)

	str.clear();

	std::cout << "fine" << std::endl;

	std::cout << "ft_http_handle end" << std::endl;
	return (1);
}
