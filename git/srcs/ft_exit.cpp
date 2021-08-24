/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eyohn <sopka13@mail.ru>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/16 09:09:14 by eyohn             #+#    #+#             */
/*   Updated: 2021/08/24 09:04:05 by eyohn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** This function ends the programm
*/

#include "../includes/headers.hpp"

void	ft_exit(t_vars *vars)
{
	vars->log_file->close();
	delete vars->log_file;
	delete vars->servers;
	delete vars->sockets;
	exit(0);
}