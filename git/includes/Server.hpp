/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eyohn <sopka13@mail.ru>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/23 09:29:47 by eyohn             #+#    #+#             */
/*   Updated: 2021/08/24 01:10:16 by eyohn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "../includes/headers.hpp"

class Server
{
	t_server	server_data;
	// Server();
	// Server(const Server &other);
	// Server&	operator= (const Server &other);

public:
	class		Error : public std::exception {
		const char* what() const throw(){
			return ("ERROR in config file (server): Struct error");
		}
	};
	Server(std::string &str);
	~Server();
	const std::string*	getName(std::string &name) const;	// Return Name server compare geter
	const std::string*	getName() const;					// Return Name server
	const std::string&	getIp() const;						// Return ip adress
	int					getPort() const;					// Return port
	bool				getRedirect() const;				// Return redirect
	const std::string&	getRedirectLocation() const;		// Return redirect location
	const std::string&	getRedirectAdress() const;			// Return redirect adress;
	bool				getAutoindex() const;				// Return autoindex
	std::string&		getLocations(std::string);			// Return locations
	socklen_t*			getSockLen();						// Return sock length
	sockaddr_in*		getServAddr();						// Return serv addr
};