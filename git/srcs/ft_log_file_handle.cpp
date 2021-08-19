/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_log_file_handle.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eyohn <sopka13@mail.ru>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/19 10:56:07 by eyohn             #+#    #+#             */
/*   Updated: 2021/08/19 15:42:02 by eyohn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/headers.hpp"

/*
** This get log_file_name from str and open it
**	return (0) - Success
**	return (1) - Error
*/

int			ft_log_file_handle(t_vars* vars, std::string &str)
{
	std::cout << "ft_log_file_handle start" << std::endl;

	std::string::iterator	start = str.begin();
	std::string				log_file_name;

	while (str.length() && *start != ';')
	{
		if (*start == ' ' || *start == '\t')
			continue;
		log_file_name += *start;
		str.erase(start);
		start = str.begin();
	}

	if (*start == ';')
		str.erase(start);

	// vars->log_file.;
	vars->log_file = new std::ofstream;
	vars->log_file->open(log_file_name.c_str(), std::ios::out | std::ios::app);
	
	if(!(vars->log_file->is_open()))
	{
		std::cout << "ERROR in config file: Logfile open error" << std::endl;
		return (1);
	}

	std::cout << "ft_log_file_handle end" << std::endl;
	return (0);
}