/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_signal_handler.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eyohn <sopka13@mail.ru>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/26 23:23:19 by eyohn             #+#    #+#             */
/*   Updated: 2021/08/26 23:44:07 by eyohn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** This function handle SIGINT signal
*/

#include "../includes/headers.hpp"

void		ft_signal_handler(int signal_num)
{
	std::cout << "Get signal " << signal_num << std::endl;
	exit_flag = true;
}