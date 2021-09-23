/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Headliners.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eyohn <sopka13@mail.ru>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/23 09:00:15 by eyohn             #+#    #+#             */
/*   Updated: 2021/09/23 09:44:46 by eyohn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Headliners.hpp"

Headliners::Headliners(std::string& http_version, std::string& return_code):
	_headliners(http_version),
	_end_headliners("\n\n")
{
#ifdef DEBUG
	std::cout	<< "Headliners c_tor start: http_version = |" << http_version
				<< "| return_code = |" << return_code << "|"
				<< std::endl;
#endif
	std::map<std::string, std::string>	status_code = {
		{"100", " Continue"},
		{"200", " OK"},
		{"404", " Not Found"}
	};
	_headliners += ' ';
	_headliners += return_code;

	std::map<std::string, std::string>::iterator it = status_code.find(return_code);
	if (it != status_code.end())
		_headliners += status_code.operator[](return_code);
	else
		std::cerr << "ERROR in Headliners: This return_code not supported, need update Headliners class" << std::endl;

#ifdef DEBUG
	std::cout	<< "Headliners c_tor end: _headliners = |" << _headliners << "|"
				<< std::endl;
#endif
	return ;
}

Headliners::~Headliners()
{}

std::string		Headliners::getHeadliners()
{
	return (_headliners + _end_headliners);
}

void			Headliners::setCloseConnection(bool status)
{
	if (status)
		_headliners += "\n Connection: keep_alive";
	else
		_headliners += "\n Connection: close";
	return ;
}