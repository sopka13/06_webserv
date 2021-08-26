/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eyohn <sopka13@mail.ru>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/23 09:29:57 by eyohn             #+#    #+#             */
/*   Updated: 2021/08/26 08:50:26 by eyohn            ###   ########.fr       */
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

static int		setRoot(std::string &str, t_location *location)
{
#ifdef DEBUG
	std::cout << "setRoot start; str = |" << str << "|" << std::endl;
#endif
	// step 1: Init data
	std::string::iterator	start = str.begin();
	std::string				temp_value;

	// step 2: Get value
	while (str.length() && *start != ' ' && *start != '\t' && *start != ';')
	{
		temp_value += *start;
		str.erase(start);
		start = str.begin();
	}
	// std::cout << temp_value << std::endl;

	// step 3: Trim spaces and tabs
	while (str.length() && (*start == ' ' || *start == '\t'))
	{
		str.erase(start);
		start = str.begin();
	}
	// std::cout << str << std::endl;

	// step 4: Check errors and write data
	if (*start == ';')
	{
		str.erase(start);
		location->location_addr = temp_value;
	}
	else
		return (1);

#ifdef DEBUG
	std::cout	<< "setRoot end; str = |" << str << "|"
				<< " location addr = |" << location->location_addr << "|"
				<< std::endl;
#endif
	return (0);
}

static int		setAutoindex(std::string &str, t_location *location)
{
#ifdef DEBUG
	std::cout << "setAutoindex start; str = |" << str << "|" << std::endl;
#endif
	// step 1: Init data
	std::string::iterator	start = str.begin();
	std::string				temp_value;

	// step 2: Get value
	while (str.length() && *start != ' ' && *start != '\t' && *start != ';')
	{
		temp_value += *start;
		str.erase(start);
		start = str.begin();
	}
	// std::cout << temp_value << std::endl;

	// step 3: Trim spaces and tabs
	while (str.length() && (*start == ' ' || *start == '\t'))
	{
		str.erase(start);
		start = str.begin();
	}
	// std::cout << str << std::endl;

	// step 4: Check errors and write data
	if (*start == ';' && (temp_value == "on" || temp_value == "off"))
	{
		str.erase(start);
		if (temp_value == "yes")
			location->autoindex = true;
	}
	else
		return (1);

#ifdef DEBUG
	std::cout	<< "setAutoindex end; str = |" << str << "|"
				<< " Autoindex = |" << location->autoindex << "|"
				<< std::endl;
#endif
	return (0);
}

static int		setRedirect(std::string &str, t_location *location)
{
#ifdef DEBUG
	std::cout << "setRedirect start; str = |" << str << "|" << std::endl;
#endif
	// step 1: Init data
	std::string::iterator	start = str.begin();
	std::string				temp_value;

	// step 2: Get value
	while (str.length() && *start != ' ' && *start != '\t' && *start != ';')
	{
		temp_value += *start;
		str.erase(start);
		start = str.begin();
	}
	// std::cout << temp_value << std::endl;

	// step 3: Trim spaces and tabs
	while (str.length() && (*start == ' ' || *start == '\t'))
	{
		str.erase(start);
		start = str.begin();
	}
	// std::cout << str << std::endl;

	// step 4: Check errors and write data
	if (*start == ';')
	{
		str.erase(start);
		location->redirect = true;
		location->redirect_adress = temp_value;
	}
	else
		return (1);

#ifdef DEBUG
	std::cout	<< "setRedirect end; str = |" << str << "|"
				<< " redirect = |" << location->redirect << "|"
				<< " redirect addr = |" << location->redirect_adress << "|"
				<< std::endl;
#endif
	return (0);
}

static int		setListen(t_server *server_data, std::string &str, std::map<std::string, t_location> *locations)
{
#ifdef DEBUG
	std::cout << "setListen start; str = |" << str << "|" << std::endl;
#endif
	if (locations == NULL)
		std::cout << "Bad args in setListen" << std::endl;

	// step 1: Init data
	std::string::iterator	start = str.begin();
	std::string				temp;

	// step 2: Get ip
	while (str.length() && (isdigit(*start) || *start == '.') && server_data->ip.length() <= 15)
	{
		server_data->ip += *start;
		str.erase(start);
		start = str.begin();
	}

	// step 3: Check errors
	if (!str.length() || *start != ':')
		return (1);
	else
	{
		str.erase(start);
		start = str.begin();
	}

	// step 4: Get port
	while (str.length() && (isdigit(*start) && temp.length() <= 5))
	{
		temp += *start;
		str.erase(start);
		start = str.begin();
	}
	server_data->port = atoi(temp.c_str());									// FORBIDDEN ???

	// step 5: Trim space and tabs
	while (*start == ' ' || *start == '\t')
	{
		str.erase(start);
		start = str.begin();
	}

	// step 6: Check "default" server
	if (str.find("default") == 0) 
	{
		server_data->default_server = true;
		for(int i = 0; i < 7; ++i)
		{
			str.erase(start);
			start = str.begin();
		}
	}

	// step 7: Trim space and tabs
	while (*start == ' ' || *start == '\t')
	{
		str.erase(start);
		start = str.begin();
	}

	// step 8: Check errors
	if (server_data->port <= 0 || server_data->port > 65535 || !str.length() || *start != ';')
		return (1);
	else
		str.erase(start);

#ifdef DEBUG
	std::cout	<< "setListen end: ip = |" << server_data->ip << "|\n"
				<< "port = |" << server_data->port << "|\n"
				<< "default = |" << server_data->default_server << "|\n"
				<< std::endl;
#endif
	return (0);
}

static int		setName(t_server *server_data, std::string &str, std::map<std::string, t_location> *locations)
{
#ifdef DEBUG
	std::cout << "setName start; str = |" << str << "|" << std::endl;
#endif
	// step 1: Init data
	if (locations == NULL)
		std::cout << "Bad args in setName" << std::endl;

	std::string::iterator	start = str.begin();
	std::string				temp;

	// step 2: Get server_name
	while (str.length() && *start != ';')
	{
		if (*start == ' ' || *start == '\t')
		{
			server_data->server_name.push_back(temp);
			#ifdef DEBUG
				std::cout << "name = |" << temp << "|" << std::endl;
			#endif
			temp.clear();
			str.erase(start);
			start = str.begin();
			continue ;
		}
		temp += *start;
		str.erase(start);
		start = str.begin();
	}

	// step 3: Trim ';' character and check error
	if (*start == ';')
	{
		server_data->server_name.push_back(temp);
		#ifdef DEBUG
			std::cout << "name = |" << temp << "|" << std::endl;
		#endif
		str.erase(start);
	}
	else
		return (1);

#ifdef DEBUG
	std::cout << "setName end" << locations->size() << std::endl;
#endif
	return (0);
}

static int		setLocation(t_server *server_data, std::string &str, std::map<std::string, t_location> *locations)
{
#ifdef DEBUG
	std::cout << "setLocation start; str = |" << str << "|" << std::endl;
#endif
	if (server_data == NULL)
		std::cout << "Bad args in setLocation" << std::endl;

	std::map<std::string, int (*)(std::string&, t_location*)> functions = {
		{"root", setRoot},
		{"autoindex", setAutoindex},
		{"redirect", setRedirect}
	};
	t_location				location_flags;
	location_flags.autoindex = false;
	location_flags.redirect = false;

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

	// step 3: start handle all settings from fttp section
	while (temp.length())
	{
		if (temp[0] == ' ' || temp[0] == '\t')
		{
			temp.erase(0);
			continue ;
		}
		if ((*functions[ft_get_name_conf(temp)])(temp, &location_flags))
			return (1);
	}

	locations->insert({temp_key, location_flags});

#ifdef DEBUG
	std::cout	<< "setLocation end; Location size = " << locations->size()
				<< " server_name size = " << server_data->server_name.size()
				<< std::endl;
#endif
	return (0);
}

Server::Server(std::string &str)
{
#ifdef DEBUG
	std::cout << "Server ctor start" << std::endl;
#endif
	// step 0: Init data
	ft_bzero(&server_data, sizeof(t_server));
	// server_data.locations = new std::map<std::string, std::string>;
	std::map<std::string, int (*)(t_server*, std::string &, std::map<std::string, t_location>*)> functions = {
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
		if ((*functions[ft_get_name_conf(temp)])(&server_data, temp, &locations))
			throw Error();
	}

	// step 4: Set port number and ip for socket data
	server_data.sock_data.sock_len = sizeof(server_data.sock_data.serv_addr);
	server_data.sock_data.serv_addr.sin_family = AF_INET;
	server_data.sock_data.serv_addr.sin_port = htons(server_data.port);
	server_data.sock_data.serv_addr.sin_addr.s_addr = inet_addr(server_data.ip.c_str());// FORBIDDEN

	// step 5: Check errors
	if (server_data.sock_data.serv_addr.sin_addr.s_addr == INADDR_NONE)
		throw Error();
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

bool				Server::getRedirect(std::string &str)
{
	std::map<std::string, t_location>::iterator it = locations.find(str);
	if (it == locations.end())
		return (false);
	return ((locations[str]).redirect);
}

std::string			Server::getRedirectAdress(std::string &str)
{
	std::map<std::string, t_location>::iterator it = locations.find(str);
	if (it == locations.end())
		return ("");
	return ((locations[str]).redirect_adress);
}

bool				Server::getAutoindex(std::string &str)
{
	std::map<std::string, t_location>::iterator it = locations.find(str);
	if (it == locations.end())
		return (false);
	return ((locations[str]).autoindex);
}

std::string			Server::getLocations(std::string str)
{
	std::map<std::string, t_location>::iterator it = locations.find(str);
	if (it == locations.end())
		return ("");
	return ((locations[str]).location_addr);
}

socklen_t*			Server::getSockLen()
{
	return (&(server_data.sock_data.sock_len));
}

sockaddr_in*		Server::getServAddr()
{
	return (&(server_data.sock_data.serv_addr));
}