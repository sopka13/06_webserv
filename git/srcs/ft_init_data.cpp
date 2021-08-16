/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_init_data.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eyohn <sopka13@mail.ru>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/16 08:02:30 by eyohn             #+#    #+#             */
/*   Updated: 2021/08/16 09:08:41 by eyohn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/headers.hpp"

void		ft_init_data(t_vars *vars, int argc, char** argv, char** envp)
{
	// step 1: Clear struct
	ft_bzero(vars, sizeof(t_vars));

	// step 2: argc, argv, envp
	vars->argc = argc;
	vars->argv = argv;
	vars->envp = envp;

	// step 3: Length of serv_addr
	vars->sock_data.sock_len = sizeof(vars->sock_data.serv_addr);

	// step 4: Parse config file
	// if (ft_parse_config(vars))
	// 	ft_exit();

	// step 5: Set family socket, portno, ip address
	vars->sock_data.serv_addr.sin_family = AF_INET;
	vars->sock_data.serv_addr.sin_port = htons(PORT);
	vars->sock_data.serv_addr.sin_addr.s_addr = inet_addr(IP_ADDRESS);		//FORBIDDEN
}