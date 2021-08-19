/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_config.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eyohn <sopka13@mail.ru>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/16 08:37:55 by eyohn             #+#    #+#             */
/*   Updated: 2021/08/19 15:50:27 by eyohn            ###   ########.fr       */
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
	std::cout	<< "ft_parse_config start"
				<< vars->ret
				<< std::endl;

	// step 1: Init data
	std::ifstream	configFile(DEF_ADR_CONF_FILE);
	std::string		str;

	// step 2: Open config file
	if (!configFile.is_open())
	{
		std::cout	<< "ERROR: Config file open error" << std::endl;
		ft_exit(vars);
	}

	// step 3: Init data for read config file
	std::string		str_sum;
	std::map<std::string, int (*)(t_vars*, std::string&)> functions = {
		{"http", ft_http_handle},
		{"log_file", ft_log_file_handle}
	};

	// step 4: Read config file
	while(std::getline(configFile, str))
	{
		// std::cout << str << std::endl;
		ft_strtrim(str, " \t\r\n");
		str_sum += str;
		std::cout << str_sum << std::endl;
	}

	while (str_sum.length())
	{
		if (str_sum[0] == ' ' || str_sum[0] == '\t')
		{
			str_sum.erase(0);
			continue ;
		}
		if ((*functions[ft_get_name_conf(str_sum)])(vars, str_sum))
			return (1);
		std::cout << "iteration      " << str_sum << std::endl;
	}

	// step : Close config file
	configFile.close();
	std::cout	<< "ft_parse_config end" << std::endl;
	return (0);
}