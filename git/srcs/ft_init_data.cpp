/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_init_data.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eyohn <sopka13@mail.ru>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/16 08:02:30 by eyohn             #+#    #+#             */
/*   Updated: 2021/08/24 10:40:39 by eyohn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/headers.hpp"

void		ft_init_data(t_vars *vars, int argc, char** argv, char** envp)
{
#ifdef DEBUG
	std::cout << "ft_init_data start" << std::endl;
#endif
	// step 1: Clear struct
	ft_bzero(vars, sizeof(t_vars));

	// step 2: argc, argv, envp
	vars->argc = argc;
	vars->argv = argv;
	vars->envp = envp;

	// step 3: Allocate memory
	vars->servers = new std::deque<Server>;
	vars->sockets = new std::vector<Socket>;

	// step 3: Parse config file
	if (ft_parse_config(vars))
		ft_exit(vars);

#ifdef DEBUG
	std::cout << "ft_init_data end" << std::endl;
#endif
	return ;
}