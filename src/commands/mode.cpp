/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loumouli <mpignet@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 14:55:30 by loumouli          #+#    #+#             */
/*   Updated: 2023/03/24 15:19:40 by loumouli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

typedef enum e_modes
{
	a = 1 << 0,
	i = 1 << 1,
	o = 1 << 2,
	r = 1 << 3,
	w = 1 << 4
}	MODES;

string mode_to_str(const uint32_t &mode)
{
	string result = "+";
	if (mode & a)
		result += "a";
	if (mode & i)
		result += "i";
	if (mode & o)
		result += "o";
	if (mode & r)
		result += "r";
	if (mode & w)
		result += "w";
	return (result);
}

void	handle_user( Server* server, vector<string>& params, Client& client, Client &target)
{
	if ( params.size() == 1 )
	{
		server->add_rply_from_server(mode_to_str(client.modeUser), target, "", RPL_UMODEIS);
        return ;
	}
	string input = params[1];
	if (input[0] != '+' && input[0] != '-')
	{
		server->add_rply_from_server(":Please use + or - with mode", client , "MODE", ERR_UMODEUNKNOWNFLAG);
		throw invalid_argument("mode: Please use + or - with mode");
	}
    if (input[0] == '+')
	{
		input.erase(0, 1);
        for (string::iterator it = input.begin(); it != input.end(); ++it)
		{
            switch (*it)
			{
				case 'a':
					server->add_rply_from_server(":Please use AWAY to set your mode to away", client , "MODE", ERR_UMODEUNKNOWNFLAG);
					throw invalid_argument("mode: Please use AWAY to set your mode to away");
				case 'o':
					if (!client.isOperator)
					{
						server->add_rply_from_server(":Permission Denied- You're not an IRC operator", client , "MODE", ERR_NOPRIVILEGES);
						throw invalid_argument("mode: Permission Denied- You're not an IRC operator");
					}
					else
					{
						target.isOperator = true;
						target.modeUser |= o;
					}
					break ;
				case 'i':
					target.modeUser |= i;
					cout << CYAN << target.modeUser << RESET << endl;
					break ;
				case 'w':
					target.modeUser |= w;
					break ;
				case 'r':
					target.modeUser |= r;
					break ;
				default:
					server->add_rply_from_server(":Please use known mode", client , "MODE", ERR_UMODEUNKNOWNFLAG);
					break ;
			}
        }
    }
    else
	{
		input.erase(0, 1);
        for (string::iterator it = input.begin(); it != input.end(); ++it)
		{
			switch (*it)
			{
				case 'a':
					server->add_rply_from_server(":Please use AWAY to set your mode to away", client , "MODE", ERR_UMODEUNKNOWNFLAG);
					throw invalid_argument("mode: Please use AWAY to set your mode to away");
				case 'o':
					if (!client.isOperator)
					{
						server->add_rply_from_server(":Permission Denied- You're not an IRC operator", client , "MODE", ERR_NOPRIVILEGES);
						throw invalid_argument("mode: Permission Denied- You're not an IRC operator");
					}
					else
					{
						target.isOperator = false;
						target.modeUser &= ~o;
					}
					break ;
				case 'i':
					target.modeUser &= ~i;
					break ;
				case 'w':
					target.modeUser &= ~w;
					break ;
				case 'r':
					target.modeUser &= ~r;
					break ;
				default:
					server->add_rply_from_server(":Please use known mode", client , "MODE", ERR_UMODEUNKNOWNFLAG);
					throw invalid_argument("mode: Please use known mode");
			}
        }
    }
	server->add_rply_from_server(mode_to_str(target.modeUser), target, "", RPL_UMODEIS);
}

void	Server::mode(vector<string>& params, Client &client)
{

	try
	{
		if (params.empty())
		{
			add_rply_from_server(":Not enough parameters", client , "MODE", ERR_NEEDMOREPARAMS);
			throw invalid_argument("mode: Not enough parameters");
		}
		if ( client.getNickname() != params[0] && !client.isOperator)
		{
			add_rply_from_server(":Cannot change mode for other users", client , "MODE", ERR_USERSDONTMATCH);
			throw invalid_argument("mode: Cannot change mode for other users");
		}
		Client &target = this->find_user(params[0], client, "MODE");
		handle_user(this, params, client, target);
	}
	catch ( exception& x)
	{
		cout << RED << x.what() << RESET << endl;
		return ;
	}
}