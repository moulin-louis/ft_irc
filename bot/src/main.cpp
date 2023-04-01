//
// Created by loumouli on 4/1/23.
//

#include "Banbot.hpp"

int main( void ) {
	Banbot bot;

	try {
		bot.parse_conf_file();
	}
	catch (exception& x) {
		cout << x.what() << endl;
	}
}
