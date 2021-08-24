/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_client_max_body_size_handle.cpp                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eyohn <sopka13@mail.ru>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/19 10:53:15 by eyohn             #+#    #+#             */
/*   Updated: 2021/08/24 08:40:33 by eyohn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** This function set max_body_size value
**	return (0) - Success;
**	return (1) - Error;
*/

#include "../includes/headers.hpp"

int			ft_client_max_body_size_handle(t_vars* vars, std::string &str)
{
#ifdef DEBUG
	std::cout << "ft_client_max_body_size_handle start" << std::endl;
#endif

	// step 0: Init data
	std::string::iterator	start = str.begin();

	// step 1: Get first value
	while (str.length() && (*start != ';' && *start != ' ' && *start != '\t'))
	{
		vars->max_body_size += *start;
		str.erase(start);
		start = str.begin();
	}

	// step 2: Trim spaces and tabs
	while (str.length() && (*start == ' ' || *start == '\t'))
	{
		str.erase(start);
		start = str.begin();
	}

	// step 3: Check next character and trim it or return error
	if (*start == ';')
		str.erase(start);
	else
	{
		std::cout << "ERROR in config file (max_body_size): Struct error" << std::endl;
		return (1);
	}

#ifdef DEBUG
	std::cout << "ft_client_max_body_size_handle end" << std::endl;
#endif
	return (0);
}