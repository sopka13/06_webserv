/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_config.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eyohn <sopka13@mail.ru>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/16 08:37:55 by eyohn             #+#    #+#             */
/*   Updated: 2021/08/26 16:55:56 by eyohn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** This function parse config file and add data for socket
**	return (0) - Success
**	return (1) - Error
*/

#include "../includes/headers.hpp"

int		ft_parse_config(t_vars *vars)
{
#ifdef DEBUG
	std::cout	<< "ft_parse_config start" << std::endl;
#endif
	// step 0: Check file get from args
	if (vars->argc == 2 || vars->argc == 3)
	{
		if (ft_check_args_files(vars))
			return (1);
	}
	else if (vars->argc > 3)
	{
		std::cout	<< "ERROR: Too many args" << std::endl;
		return (1);
	}

	// step 1: Init data
	std::ifstream	configFile(DEF_ADR_CONF_FILE);
	std::string		str;

	// step 2: Open config file
	if (!configFile.is_open())
	{
		std::cout	<< "ERROR: Config file open error" << std::endl;
		return (1);
	}

	// step 3: Init data for read config file
	std::string		str_sum;
	std::map<std::string, int (*)(t_vars*, std::string&)> functions = {
		{"http", ft_http_handle},
		{"log_file", ft_log_file_handle}
	};

	// step 4: Read config file in str_sum
	while(std::getline(configFile, str))
	{
		ft_strtrim(str, " \t\r\n");
		str_sum += str;
		// std::cout << str_sum << std::endl;
	}

	// step 5: Reserve memory for vector and map elements
	int i = 0;
	int pos = 0;
	for (long unsigned int k = 0; k < str_sum.length(); ++k)
	{
		pos = str_sum.find("server", k);
		if (pos)
		{
			i++;
			k += pos + 1;
		}
		else
			break ;
	}
	vars->sockets->reserve(i);
	// std::cout << vars->sockets->capacity() << std::endl;

	// step 5: Get config name and execute handle functions
	while (str_sum.length())
	{
		if (str_sum[0] == ' ' || str_sum[0] == '\t')
		{
			str_sum.erase(0);
			continue ;
		}
		if ((*functions[ft_get_name_conf(str_sum)])(vars, str_sum))
			return (1);
	}

	// step 6: Close config file
	configFile.close();
#ifdef DEBUG
	std::cout	<< "ft_parse_config end" << std::endl;
#endif
	return (0);
}