/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_epoll_action.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eyohn <sopka13@mail.ru>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/07 17:13:43 by eyohn             #+#    #+#             */
/*   Updated: 2021/09/10 09:56:16 by eyohn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** This function execute 2 function depending on the type of descriptor 
*/

#include "../includes/headers.hpp"

void		ft_handle_epoll_action(t_vars *vars, int fd)
{
#ifdef DEBUG
	std::cout	<< "ft_handle_epoll_action start: fd = " << fd << std::endl;
#endif
	// step 1: Start handle function in thread
	for (int i = 0; i < static_cast<int>(vars->sockets->size()); ++i)
	{
		if (fd == vars->sockets->operator[](i).getTcp_sockfd())
		{
			vars->sockets->operator[](i).setFd();
			vars->threads.emplace_back(std::thread(ft_handle_epoll_fd, std::ref(vars), vars->sockets->operator[](i).getFd(), i));
			// ft_handle_epoll_socket(vars, fd);
			#ifdef DEBUG
				std::cout	<< "ft_handle_epoll_action end: fd = " << fd << std::endl;
			#endif
			return ;
		}
		// std::cout << i << std::endl;
	}

	// step 2: Delete fd from queue epoll
	if (epoll_ctl(vars->epoll_fd, EPOLL_CTL_DEL, fd, &vars->ev) == -1)
	{
		std::cout << "ERROR in ft_handle_epoll_action: Epoll_ctl del error" << std::endl;
		ft_exit(vars);
	}
	vars->threads.emplace_back(std::thread(ft_handle_epoll_fd, std::ref(vars), fd, -1));
	// ft_handle_epoll_fd(vars, fd);

#ifdef DEBUG
	std::cout	<< "ft_handle_epoll_action end: fd = " << fd << std::endl;
#endif
	return ;
}