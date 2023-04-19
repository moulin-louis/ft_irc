/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: armendi <armendi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 16:33:26 by mpignet           #+#    #+#             */
/*   Updated: 2023/03/30 15:51:37 by armendi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "irc.hpp"

class Channel
{
    private:
        string      _name;
        string      _topic;
		Mode        _modeChannel;
    public:
		vector<string> ban;
        //vector of client's fd
        vector<Socket> clients;

        //canonical form
        Channel();
        Channel(const string&, const Client& );
        Channel(const Channel&);
        ~Channel();
        Channel &operator=(const Channel &);

		//getter/setter
        const string    &getName() const;
        const string    &getTopic() const;
        void            setTopic(const string&);

        //checks
        bool	user_in_chan(const Client&);

        //actions
        void    addClient(const Client &);
        void    removeClient(const Client &);
		Mode    getMode() const;
		void    setMode(Mode input);
		void    unSetMode(Mode input);
};

#endif