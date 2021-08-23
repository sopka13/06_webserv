/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_init_data.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eyohn <sopka13@mail.ru>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/16 08:02:30 by eyohn             #+#    #+#             */
/*   Updated: 2021/08/23 19:47:44 by eyohn            ###   ########.fr       */
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

	// step 3: Parse config file
	if (ft_parse_config(vars))
		ft_exit(vars);

	// step 4: Set family socket, portno, ip address
	// vars->servers.sock_data.serv_addr.sin_family = AF_INET;							//const
	// vars->servers.sock_data.serv_addr.sin_port = htons(PORT);						//from config file
	// vars->servers.sock_data.serv_addr.sin_addr.s_addr = inet_addr(IP_ADDRESS);		//FORBIDDEN

#ifdef DEBUG
	std::cout << "ft_init_data end" << std::endl;
#endif
	return ;
}