# ft_IRC
This project is about creating a IRC server. Communication between client and server are done via TCP/IP.

##  Summary

The goal of the project is to create a IRC server. Internet Relay Chat or IRC is a text-based communication protocol on the Internet. It offers real-time messaging that can be either public or private. Users can exchange direct messages and join group channels.

The project had to be coded in **C++98**, however, we use the [libcurl](https://curl.se/download.html "libcurl") for the bot in the bonus section. **Libcurl** is used to send request to chatGPT and get the answer.

##  Usage

The reference client we used is **irssi**, so `/DCC` features like chat and file transfers are supported.

Run `make` in the root of the projet and launch as follows:

    ./ircserv <port> <password>

- `port` -- the default port for IRC is 6667.

Run `make bonus` to add the bot. The bot performs 2 tasks,
- it monitors channels and kicks users writting banwords,
- it forwards its private messages form other users to chatGPT using the **libcurl** and sends the answer back to the users in a private message:

    `./banbot`

 --- 

This project was awesomely carried out with [moulin-louis](https://github.com/moulin-louis) and [Maelp20](https://github.com/Maelp20) !
