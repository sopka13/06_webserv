/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eyohn <sopka13@mail.ru>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/13 08:05:16 by eyohn             #+#    #+#             */
/*   Updated: 2021/10/13 08:13:24 by eyohn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <unistd.h>

int		main(void)
{
	char*	path = "/home/sergey/My_prog/21_school/06_webserv/git/sites_avaliable/test/ubuntu_cgi_tester";
	char*	argv[] = {
		"/home/sergey/My_prog/21_school/06_webserv/git/sites_avaliable/test/ubuntu_cgi_tester",
		"/home/sergey/My_prog/21_school/06_webserv/git/sites_avaliable/test/YoupiBanane/youpi.bla",
		NULL
	};
	char*	envp[] = {
		"REQUEST_METHOD=POST",
		"SERVER_PROTOCOL=HTTP/1.1",
		"PATH_INFO=/home/sergey/My_prog/21_school/06_webserv/git/sites_avaliable/test/YoupiBanane/youpi.bla",
		NULL
	};

	execve(path, argv, envp);
	return (0);
}