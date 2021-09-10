/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response_2.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eyohn <sopka13@mail.ru>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/09 08:55:00 by eyohn             #+#    #+#             */
/*   Updated: 2021/09/10 08:50:54 by eyohn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "../includes/headers.hpp"

class Response_2
{
	Server			*_server;				// data for server
	int				_fd;					// handle fd
	char			_buff[BUF_FOR_RESP];	// buffer for read from client
	bool			_close_flag;			// if have close header
	Response_2();
	
	public:
	class	Exeption : public std::runtime_error {
		public:
			Exeption(const std::string& message) : std::runtime_error(message) {}
	};
	Response_2(Server *server, int fd);
	~Response_2();

	void			handleRequest();
	int 			sendingResponseGet(std::string full_path, struct stat is_a_dir, Response &response);
	std::string		getIndexFileName(std::string path);
	bool			getCloseFlag();
};