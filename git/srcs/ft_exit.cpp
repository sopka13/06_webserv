/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eyohn <sopka13@mail.ru>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/16 09:09:14 by eyohn             #+#    #+#             */
/*   Updated: 2021/10/27 23:05:41 by eyohn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** This function ends the programm
*/

#include "../includes/headers.hpp"

void	ft_exit(t_vars *vars)
{
#ifdef DEBUG
	std::cout	<< "ft_exit start" << std::endl;
#endif
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
	// if (vars->fd_identify_socket) {
	// 	delete vars->fd_identify_socket;
	// }
	if (vars->request_container->size()) {			// need clean all Response_2 pointer memeory
		for (std::map<int, Response_2*>::iterator i = vars->request_container->begin(); i != vars->request_container->end(); ++i)
		{
			delete ((*i).second);
		}
		delete vars->request_container;
	}
	if (vars->CGI) {
		delete vars->CGI;
	}
	std::cerr << "Server stop" << std::endl;
#ifdef DEBUG
	std::cout	<< "ft_exit end" << std::endl;
#endif
	exit(0);
}
