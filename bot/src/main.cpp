//
// Created by loumouli on 4/1/23.
//

#include "Banbot.hpp"


int main(int argc, char **argv) {
	(void)argc;
	Banbot bot;

	try {
		bot.parse_conf_file();
		bot.initial_connection();
		bot.chatgpt(argv[1]);
		bot.routine();
	}
	catch (exception& x) {
		cout << RED << x.what() << RESET << endl;
	}
}
