/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eyohn <sopka13@mail.ru>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/23 09:29:57 by eyohn             #+#    #+#             */
/*   Updated: 2021/08/24 01:51:13 by eyohn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

// Server::Server()
// {}

// Server::Server(const Server &other)
// {}

// Server&		Server::operator= (const Server &other)
// {}

Server::~Server()
{}

static int		setListen(t_server *server_data, std::string &str)
{
	// step 1: Init data
	std::string::iterator	start = str.begin();
	std::string				temp;

	// step 2: Get ip
	while (str.length() && *start != ':')
	{
		server_data->ip += *start;
		str.erase(start);
		start = str.begin();
	}

	// step 3: Check errors
	if (!str.length() || server_data->ip.length() > 15)
		return (1);

	// step 4: Get port
	while (str.length() && (*start != ' ' && *start != '\t' && *start != ';'))
	{
		temp += *start;
		str.erase(start);
		start = str.begin();
	}
	server_data->port = atoi(temp.c_str());

	// step 5: Trim space and tabs
	while (*start == ' ' || *start == '\t' || *start == ';')
	{
		str.erase(start);
		start = str.begin();
	}

	// step 6: Check errors
	if (server_data->port <= 0 || server_data->port > 65535)
		return (1);

	return (0);
}

static int		setName(t_server *server_data, std::string &str)
{
	// step 1: Init data
	std::string::iterator	start = str.begin();
	std::string				temp;

	// step 2: Get server_name
	while (str.length() && *start != ';')
	{
		temp += *start;
		if (*start == ' ' || *start == '\t')
		{
			server_data->server_name.push_back(temp);
			temp.clear();
		}
		str.erase(start);
		start = str.begin();
	}

	// step 3: Trim ';' character and check error
	if (*start == ';')
		str.erase(start);
	else
		return (1);

	return (0);
}

static int		setLocation(t_server *server_data, std::string &str)
{
	/// Не до конца обрабатывает кусок кода в фигурных скобках
#ifdef DEBUG
	std::cout << "setLocation start; str = " << str << std::endl;
#endif
	std::string::iterator	start = str.begin();
	std::string				temp;
	std::string				temp_key;
	std::string				temp_value;
	std::string				temp_user;
	int						i = 0;

	// step 1: Get first value
	while (str.length() && *start != ' ' && *start != '\t' && *start != '{')
	{
		temp_key += *start;
		str.erase(start);
		start = str.begin();
	}
	// std::cout << temp_key << std::endl;
	// std::cout << str << std::endl;

	// step 2: Trim spaces and tabs
	while (str.length() && (*start == ' ' || *start == '\t'))
	{
		str.erase(start);
		start = str.begin();
	}
	// std::cout << temp_key << std::endl;
	// std::cout << str << std::endl;

	// step 3: Set start value for get server section
	if (*start == '{')
	{
		i++;
		str.erase(start);
		start = str.begin();
	}
	else
		return (1);

	// step 4: Start write server section and trim it from the str_sum
	while (i > 0 && str.length())
	{
		if (*start == '{')
			i++;
		if (*start == '}')
			i--;
		temp += *start;
		str.erase(start);
		start = str.begin();
	}
	if (i)
		return (1);
	else
	{
		std::string::iterator	end = temp.end();
		end--;
		temp.erase(end);
	}
	// std::cout << temp << std::endl;

	// step 5: Get user
	start = temp.begin();
	while (temp.length() && *start != ' ' && *start != '\t' && *start != ';')
	{
		temp_user += *start;
		temp.erase(start);
		start = temp.begin();
	}

	// step 6: Trim spaces and tabs
	while (temp.length() && (*start == ' ' || *start == '\t'))
	{
		temp.erase(start);
		start = temp.begin();
	}

	// step 7: Get value
	while (temp.length() && *start != ' ' && *start != '\t' && *start != ';')
	{
		temp_value += *start;
		temp.erase(start);
		start = temp.begin();
	}

	// step 8: Set value settings
	if (temp_user == "redirect")
	{
		server_data->redirect = true;
		server_data->redirect_location = temp_key;
		server_data->redirect_adress = temp_value;
	}
	else if (temp_user == "autoindex")
	{
		if (temp_value == "on")
			server_data->autoindex = true;
		else if (temp_value == "off")
			server_data->autoindex = false;
		else
			return (1);
	}
	else if (temp_user == "root")
	{
		server_data->locations.insert({temp_key, temp_value});
	// std::cout << temp_user << "|||" << temp_key << "|||" << temp_value << std::endl;
	}
	else
		return (1);
	// std::cout << "fine" << std::endl;
#ifdef DEBUG
	std::cout << "setLocation end" << std::endl;
#endif
	return (0);
}

Server::Server(std::string &str)
{
#ifdef DEBUG
	std::cout << "Server ctor start" << std::endl;
#endif
	// step 0: Init data
	std::map<std::string, int (*)(t_server*, std::string &)> functions = {
		{"listen", setListen},
		{"server_name", setName},
		{"location", setLocation}
	};
	std::string::iterator	start = str.begin();
	std::string				temp;
	int						i = 0;

	// step 1: set start value for get server section
	if (*start == '{')
	{
		i++;
		str.erase(start);
		start = str.begin();
	}
	else
		throw Error();

	// step 2: start write server section and trim it from the str_sum
	while (i > 0 && str.length())
	{
		if (*start == '{')
			i++;
		if (*start == '}')
			i--;
		temp += *start;
		str.erase(start);
		start = str.begin();
	}
	if (i)
		throw Error();
	else
	{
		std::string::iterator	end = temp.end();
		end--;
		temp.erase(end);
	}
	// std::cout << "Temp = " << temp << "str = " << str << std::endl;

	// step 3: start handle all settings from server section
	while (temp.length())
	{
		if (temp[0] == ' ' || temp[0] == '\t')
		{
			temp.erase(0);
			continue ;
		}
		(*functions[ft_get_name_conf(temp)])(&server_data, temp);
		// std::cout << "iteration 1      " << temp << std::endl;
	}

	// step 4: Set port number and ip for socket data
	server_data.sock_data.sock_len = sizeof(server_data.sock_data.serv_addr);
	
	server_data.sock_data.serv_addr.sin_family = AF_INET;
	server_data.sock_data.serv_addr.sin_port = htons(server_data.port);
	server_data.sock_data.serv_addr.sin_addr.s_addr = inet_addr(server_data.ip.c_str());// FORBIDDEN
#ifdef DEBUG
	std::cout << "Server ctor end" << std::endl;
#endif
}

const std::string*	Server::getName() const
{
	if (!server_data.server_name.empty())
		return (&server_data.server_name[0]);
	return (NULL);
}

const std::string*	Server::getName(std::string &str) const
{
	if (!server_data.server_name.empty())
	{
		for (long unsigned int i = 0; i < server_data.server_name.size(); ++i)
			if (str == server_data.server_name[i])
				return (&server_data.server_name[i]);
		return (NULL);
	}
	return (NULL);
}

const std::string&	Server::getIp() const
{
	return (server_data.ip);
}

int					Server::getPort() const
{
	return (server_data.port);
}

bool				Server::getRedirect() const
{
	return (server_data.redirect);
}

const std::string&	Server::getRedirectLocation() const
{
	return (server_data.redirect_location);
}

const std::string&	Server::getRedirectAdress() const
{
	return (server_data.redirect_adress);
}

bool			Server::getAutoindex() const
{
	return (server_data.autoindex);
}

std::string&	Server::getLocations(std::string str)
{
	return (server_data.locations[str]);
}

socklen_t*		Server::getSockLen()
{
	return (&(server_data.sock_data.sock_len));
}

sockaddr_in*	Server::getServAddr()
{
	return (&(server_data.sock_data.serv_addr));
}