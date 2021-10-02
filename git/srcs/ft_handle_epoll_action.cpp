/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_epoll_action.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eyohn <sopka13@mail.ru>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/07 17:13:43 by eyohn             #+#    #+#             */
/*   Updated: 2021/10/02 23:11:03 by eyohn            ###   ########.fr       */
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
		// If it's socket
		if (fd == vars->sockets->operator[](i).getTcp_sockfd())
		{
			// step 1.1: Set fd
			vars->sockets->operator[](i).setFd();

			// step 1.2: Get request and send response
			try
			{
				// int ret = 0;
				Response_2		resp(&(vars->servers->operator[](i)), vars->sockets->operator[](i).getFd());
				resp.readRequest();
				resp.sendResponse();
				// if ((ret = resp.sendResponse()))
				// {
				// 	if (ret == 2)	// if close connection
				// 	{
				// 		close(vars->sockets->operator[](i).getFd());
				// 		return ;
				// 	}
				// 	std::cerr << "ERROR ERROR in ft_handle_epoll_action: no unhandled request (or empty request) !!!" << std::endl;
				// 	return ;
				// }
			}
			catch(const std::exception& e)
			{
				std::cerr << e.what() << "2" << '\n';
			}

			// step 1.3: Add fd in epoll queue
			vars->ev.events = EPOLLIN | EPOLLOUT;
			vars->ev.data.fd = vars->sockets->operator[](i).getFd();
			if (epoll_ctl(vars->epoll_fd, EPOLL_CTL_ADD, vars->sockets->operator[](i).getFd(), &vars->ev) == -1)
			{
				std::cerr << "ERROR in ft_handle_epoll_action: Epoll_ctl add error" << std::endl;
				ft_exit(vars);
			}

			// step 1.4: Add socket number for fd for next handle
			vars->fd_identify_socket->insert({vars->sockets->operator[](i).getFd(), i});

			#ifdef DEBUG
				std::cout	<< "ft_handle_epoll_action end: fd = " << fd << std::endl;
			#endif
			return ;
		}
	}

	// step 2: If handle fd (no socket)
	try
	{
		Response_2		resp(&(vars->servers->operator[](vars->fd_identify_socket->operator[](fd))), fd);
		resp.readRequest();
		resp.sendResponse();
		// if (resp.sendResponse())
		// {
		// 	std::cerr << "ERROR ERROR in ft_handle_epoll_action: no unhandled request !!!" << std::endl;
		// 	return ;
		// }
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	

	// if want new thread
	// step 2: Delete fd from queue epoll
	// if (epoll_ctl(vars->epoll_fd, EPOLL_CTL_DEL, fd, &vars->ev) == -1)
	// {
	// 	std::cerr << "ERROR in ft_handle_epoll_action: Epoll_ctl del error" << std::endl;
	// 	ft_exit(vars);
	// }
	// vars->threads.emplace_back(std::thread(ft_handle_epoll_fd, std::ref(vars), fd, -1));

#ifdef DEBUG
	std::cout	<< "ft_handle_epoll_action end: fd = " << fd << std::endl;
#endif
	return ;
}