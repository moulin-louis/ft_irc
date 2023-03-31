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
        string _name;
        string _topic;
    public:
        //vector of client's fd
        vector<Socket> clients;

        //canonical form
        Channel();
        Channel(string &, Client& );
        Channel(const Channel &);
        ~Channel();
        Channel &operator=(const Channel &);

		//getter/setter
        const string    &getName() const;
        const string    &getTopic() const;
        void            setTopic(string &);
        void            setName(string &);

        //checks
        bool	user_in_chan(Client&);

        //actions
        void    addClient(Client &);
        void    removeClient(Client &client);
};

#endif