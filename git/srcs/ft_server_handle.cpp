/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_server_handle.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eyohn <sopka13@mail.ru>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/19 10:57:36 by eyohn             #+#    #+#             */
/*   Updated: 2021/08/20 15:23:42 by eyohn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/headers.hpp"

int			ft_server_handle(t_vars* vars, std::string &str)
{
	std::cout << "ft_server_handle start" << std::endl;
	std::map<std::string, int (*)(t_vars*, std::string&)> functions = {
		{"listen", ft_listen_handle},
		{"server_name", ft_server_name_handle},
		{"location", ft_location_handle}
	};
	std::string::iterator	start = str.begin();
	std::string				temp;
	int						i = 0;

	// step 1: set start value for get server section
	if (*start == '{')
	{
		i++;
		str.erase(start);
		start = str.begin();
	}
	else
	{
		std::cout << "ERROR in config file (server): Struct error" << std::endl;
		return (1);
	}

	// step 2: start write server section and trim it from the str_sum
	while (i > 0 && str.length())
	{
		if (*start == '{')
			i++;
		if (*start == '}')
			i--;
		temp += *start;
		str.erase(start);
		start = str.begin();
	}
	if (i)
	{
		std::cout << "ERROR in config file (server): Struct error" << std::endl;
		return (1);
	}
	else
	{
		std::string::iterator	end = temp.end();
		end--;
		temp.erase(end);
	}
	// std::cout << "Temp = " << temp << "str = " << str << std::endl;

	// step 3: start handle all settings from server section
	while (temp.length())
	{
		if (temp[0] == ' ' || temp[0] == '\t')
		{
			temp.erase(0);
			continue ;
		}
		if ((*functions[ft_get_name_conf(temp)])(vars, temp))
			return (1);
		// std::cout << "iteration 1      " << temp << std::endl;
	}


	// std::cout << "!!!!" << str << std::endl;

	std::cout << "ft_server_handle end" << std::endl;
	return (0);
}