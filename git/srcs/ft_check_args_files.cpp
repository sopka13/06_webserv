/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_check_args_files.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eyohn <sopka13@mail.ru>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/25 09:03:49 by eyohn             #+#    #+#             */
/*   Updated: 2021/09/14 13:05:30 by eyohn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** This function check files get from args
**	return (0) - Success
**	return (1) - Error
*/

#include "../includes/headers.hpp"

int			ft_check_args_files(t_vars *vars)
{
#ifdef DEBUG
	std::cout	<< "ft_check_args_files start" << std::endl;
#endif
	// step 1: Check valid file name
	if (vars->argc == 2)
	{
		std::string		file_1(vars->argv[1]);
		if (file_1.find(".conf") != std::string::npos &&
			file_1.find(".conf") == file_1.size() - 5 &&
			file_1.find(".conf") != 0)
			vars->config_file_name = file_1;
		else
		{
			std::cerr << "ERROR: Invalid configuration file name" << std::endl;
			return (1);
		}
	}
	else
	{
		std::string		file_1(vars->argv[1]);
		std::string		file_2(vars->argv[2]);
		if (file_1.find(".conf") != std::string::npos &&
			file_1.find(".conf") == file_1.size() - 5 &&
			file_1.find(".conf") != 0)
			vars->config_file_name = file_1;
		else if (file_2.find(".conf") != std::string::npos &&
			file_2.find(".conf") == file_2.size() - 5 &&
			file_2.find(".conf") != 0)
			vars->config_file_name = file_2;
		else
		{
			std::cerr << "ERROR: Invalid configuration file name" << std::endl;
			return (1);
		}
	}

#ifdef DEBUG
	std::cout	<< "ft_check_args_files end" << std::endl;
#endif
	return (0);
}