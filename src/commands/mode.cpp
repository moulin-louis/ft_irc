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

string mode_to_str(const Mode &mode)
{
	string result = "+";
	if (mode & a)
		result += "a";
	if (mode & i)
		result += "i";
	if (mode & o)
		result += "o";
	if (mode & w)
		result += "w";
	if (mode & r)
		result += "r";
	if (mode & B)
		result += "B";
	if (mode & p)
		result += "p";
	if (mode & n)
		result += "n";
	return (result);
}

void	Server::handle_user(const vector<string>& params, Client& client, Client &target)
{
	if ( params.size() == 1 )
	{
		this->add_rply_from_server(mode_to_str(client.getMode()), target, "", RPL_UMODEIS);
		return ;
	}
	string input = params[1];
	if (input[0] != '+' && input[0] != '-')
	{
		this->add_rply_from_server(":Please use + or - with mode", client , "MODE", ERR_UMODEUNKNOWNFLAG);
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
						this->add_rply_from_server(":Please use AWAY to set your mode to away", client , "MODE", ERR_UMODEUNKNOWNFLAG);
					break ;
				case 'o':
					if (!client.isOperator)
						this->add_rply_from_server(":Permission Denied- You're not an IRC operator", client , "MODE", ERR_NOPRIVILEGES);
					else
					{
						target.isOperator = true;
						target.setMode(o);
					}
					break ;
				case 'i':
					target.setMode(i);
					break ;
				case 'w':
					target.setMode(w);
					break ;
				case 'r':
					target.setMode(r);
					break ;
				case 'B':
					if (!client.isOperator)
						this->add_rply_from_server(":Permission Denied- You're not an IRC operator", client, "MODE", ERR_NOPRIVILEGES);
					else
					{
						this->get_botList().push_back(target.getNickname());
						target.setMode(B);
					}
					break ;
				default:
					this->add_rply_from_server(":Please use known mode", client , "MODE", ERR_UMODEUNKNOWNFLAG);
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
					this->add_rply_from_server(":Please use AWAY to set your mode to away", client , "MODE", ERR_UMODEUNKNOWNFLAG);
					throw invalid_argument("mode: Please use AWAY to set your mode to away");
				case 'o':
					if (!client.isOperator)
						this->add_rply_from_server(":Permission Denied- You're not an IRC operator", client , "MODE", ERR_NOPRIVILEGES);
					else
					{
						target.isOperator = false;
						target.unSetMode(o);
					}
					break ;
				case 'i':
					target.unSetMode(i);
					break ;
				case 'w':
					target.unSetMode(w);
					break ;
				case 'r':
					target.unSetMode(r);
					break ;
				case 'B':
					if (!client.isOperator)
					{
						this->add_rply_from_server(":Permission Denied- You're not an IRC operator", client, "MODE", ERR_NOPRIVILEGES);
					}
					else
					{
						this->get_botList().erase(find(this->get_botList().begin(), this->get_botList().end(), target.getNickname()));
						target.unSetMode(B);
					}
					break ;
				default:
					this->add_rply_from_server(":Please use known mode", client , "MODE", ERR_UMODEUNKNOWNFLAG);
					throw invalid_argument("mode: Please use known mode");
			}
        }
    }
	this->add_rply_from_server(mode_to_str(target.getMode()), target, "", RPL_UMODEIS);
}

void    Server::handle_channel(const vector<string>& params, Client& client, Channel &dest)
{
	if (params.size() == 1)
	{
		this->add_rply_from_server(mode_to_str(dest.getMode()), client, dest, "", RPL_CHANNELMODEIS);
		return ;
	}
	string input = params[1];
	if (input[0] != '+' && input[0] != '-')
	{
		this->add_rply_from_server(":Please use + or - with mode", client, dest, "MODE", ERR_UNKNOWNMODE);
		throw invalid_argument("mode: Please use + or - with mode");
	}
	if (input[0] == '+')
	{
		input.erase(0, 1);
		for (string::iterator it = input.begin(); it != input.end(); ++it)
		{
			switch (*it)
			{
				case 'p':
					dest.setMode(p);
					break ;
				case 'n':
					dest.setMode(n);
					break ;
				default:
					this->add_rply_from_server(":Please use known mode", client, dest, "MODE", ERR_UNKNOWNMODE);
					throw invalid_argument("mode: Please use known mode");
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
				case 'p':
					dest.unSetMode(p);
					break ;
				case 'n':
					dest.unSetMode(n);
					break ;
				default:
					this->add_rply_from_server(":Please use known mode", client, dest, "MODE", ERR_UNKNOWNMODE);
					throw invalid_argument("mode: Please use known mode");
					break ;
			}
		}
	}
	this->add_rply_from_server(mode_to_str(dest.getMode()), client, dest, "", RPL_CHANNELMODEIS);
}

void	Server::mode( const vector<string>& params, Client &client)
{
	try
	{
		if (params.empty())
		{
			add_rply_from_server(":Not enough parameters", client , "MODE", ERR_NEEDMOREPARAMS);
			throw invalid_argument("mode: Not enough parameters");
		}
		else if (params[0][0] == '#')
		{
			Channel &dest = find_channel(params[0], client);
			if (dest.user_in_chan(client) || client.isOperator)
				handle_channel(params, client, dest);
			else
				add_rply_from_server(":" + dest.getName() + " :Cannot send to channel", client, "", ERR_CANNOTSENDTOCHAN);
		}
		else
		{
			if (client.getNickname() != params[0] && !client.isOperator)
			{
				add_rply_from_server(":Cannot change mode for other users", client, "MODE", ERR_USERSDONTMATCH);
				throw invalid_argument("mode: Cannot change mode for other users");
			}
			Client &target = this->find_user(params[0], client, "MODE");
			handle_user(params, client, target);
		}
	}
	catch ( exception& x)
	{
		cout << RED << x.what() << RESET << endl;
		return ;
	}
}
