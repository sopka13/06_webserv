/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eyohn <sopka13@mail.ru>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/16 09:09:14 by eyohn             #+#    #+#             */
/*   Updated: 2021/09/21 10:53:55 by eyohn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** This function ends the programm
*/

#include "../includes/headers.hpp"

void	ft_exit(t_vars *vars)
{
	ft_write_in_log_file(vars, "Server stop");
	vars->log_file->close();
	if (vars->log_file) {
		delete vars->log_file;
	}
	if (vars->servers) {
		delete vars->servers;
	}
	if (vars->sockets) {
		delete vars->sockets;
	}
	if (vars->fd_identify_socket) {
		delete vars->fd_identify_socket;
	}
	if (vars->CGI) {
		delete vars->CGI;
	}
	exit(0);
}