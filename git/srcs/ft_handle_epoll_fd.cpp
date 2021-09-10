/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_epoll_fd.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eyohn <sopka13@mail.ru>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/07 17:24:01 by eyohn             #+#    #+#             */
/*   Updated: 2021/09/10 09:47:44 by eyohn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** This function handle fd for response
*/

#include "../includes/headers.hpp"

static int	ft_handle_request(t_vars *vars, int fd, int i)
{
// #ifdef DEBUG
	std::cout	<< "ft_handle_request start: fd = " << fd << std::endl;
// #endif
	// step 1: init obj for handle request
	Response_2	resp(&(vars->servers->operator[](i)), fd);

	// step 2: handle request
	resp.handleRequest();

	// step 3: If have close flag
	if (resp.getCloseFlag())
	{
		close(fd);
		return (1);
	}

#ifdef DEBUG
	std::cout	<< "ft_handle_request end: fd = " << fd << std::endl;
#endif
	return (0);
}

static int	ft_timing(int i)
{
	if (WAIT_CLIENT_SEC * (i + 1) >= WAIT_REQUEST_FROM_CLIENT_SEC ||
		(WAIT_CLIENT_SEC * (i + 1) + (WAIT_CLIENT_USEC/1000000) * (i + 1)) >= WAIT_REQUEST_FROM_CLIENT_SEC)
		return (0);
	return (1);
}

void		ft_handle_epoll_fd(t_vars *vars, int fd, int i)
{
// #ifdef DEBUG
	std::cout	<< "ft_handle_epoll_fd start: fd = " << fd << std::endl;
// #endif
	// step 1: Init data
	int				k = 0;
	int				ret = 0;
	fd_set			rfd;
	fd_set			wfd;
	struct timeval	tv;
	tv.tv_sec = WAIT_CLIENT_SEC;
	tv.tv_usec = WAIT_CLIENT_USEC;
	// std::cout << "step 1 ok" << std::endl;

	// step 2: Handle first request
	if ((ret = ft_handle_request(vars, fd, i)))
		return ;
	// std::cout << "step 2 ok" << std::endl;
	

	// step 3: create select
	while (ft_timing(k))
	{
		FD_ZERO(&rfd);
		FD_SET(fd, &rfd);
		FD_SET(fd, &wfd);
		switch (ret = select(1 + fd, &rfd, &wfd, 0, &tv))
		{
		case -1:
			std::cout << "ERROR in ft_handle_epoll_fd: select fall" << std::endl;
			return ;
		case 0:
			k++;
			continue ;
			break ;
		default:
			k = 0;
			if ((ret = ft_handle_request(vars, fd, i)))
				return ;
			break ;
		}
	}
	// std::cout << "step 3 ok" << std::endl;

	// step 4: Add fd in epoll queue
	vars->ev.events = EPOLLIN | EPOLLOUT;
	vars->ev.data.fd = fd;
	if (epoll_ctl(vars->epoll_fd, EPOLL_CTL_ADD, fd, &vars->ev) == -1)
	{
		std::cout << "ERROR in ft_handle_epoll_fd: Epoll_ctl add error" << std::endl;
		close(fd);
		return ;
	}
	// std::cout << "step 4 ok" << std::endl;

#ifdef DEBUG
	std::cout	<< "ft_handle_epoll_fd end: fd = " << fd << std::endl;
#endif
	return ;
}