/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_init_data.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eyohn <sopka13@mail.ru>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/16 08:02:30 by eyohn             #+#    #+#             */
/*   Updated: 2021/08/26 17:37:01 by eyohn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/headers.hpp"
#include <fcntl.h>

void		ft_init_data(t_vars *vars, int argc, char** argv, char** envp)
{
#ifdef DEBUG
	std::cout << "ft_init_data start" << std::endl;
#endif
	// step 1: Clear struct
	ft_bzero(vars, sizeof(t_vars));
	vars->exit = false;

	// step 2: argc, argv, envp
	vars->argc = argc;
	vars->argv = argv;
	vars->envp = envp;

	// step 3: Allocate memory
	vars->servers = new std::deque<Server>;
	vars->sockets = new std::vector<Socket>;

	// step 4: Parse config file
	if (ft_parse_config(vars))
		ft_exit(vars);

	// step 5: Create semaphores
	sem_unlink(SEM_NAME_1);
	vars->sema = sem_open(SEM_NAME_1, 0100, 0666, vars->sockets->capacity());
	if (vars->sema == SEM_FAILED)
	{
		std::cout << "ERROR: Semaphore create faill" << std::endl;
		ft_exit(vars);
	}

#ifdef DEBUG
	std::cout << "ft_init_data end" << std::endl;
#endif
	return ;
}