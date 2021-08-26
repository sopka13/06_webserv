/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_in_thread.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eyohn <sopka13@mail.ru>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/26 16:18:23 by eyohn             #+#    #+#             */
/*   Updated: 2021/08/26 23:26:11 by eyohn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** This function start in new thread
*/

#include "../includes/headers.hpp"

void		ft_in_thread(t_vars &vars, int i)
{
// #ifdef DEBUG
	std::cout << "ft_in_thread start; thread id = " << std::this_thread::get_id() << std::endl;
// #endif

	while (1)
	{
		vars.sockets->operator[](i).setFd();
			
		if (vars.sockets->operator[](i).ft_handle_request())
		{
			sem_post(vars.sema);
		 	ft_exit(&vars);
		}
		if (exit_flag)
			ft_exit(&vars);
	}
	// std::cout << "i = " << i << std::endl;

#ifdef DEBUG
	std::cout << "ft_in_thread end; thread id = " << std::this_thread::get_id() << std::endl;
#endif
	return ;
}