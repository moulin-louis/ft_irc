//
// Created by loumouli on 4/1/23.
//

#include "Banbot.hpp"

int main( void ) {
	Banbot bot;

	try {
		bot.parse_conf_file();
		bot.initial_connection();
		bot.routine();
	}
	catch (exception& x) {
		cout << x.what() << endl;
	}
}
