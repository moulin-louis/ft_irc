//
// Created by loumouli on 4/1/23.
//

#include "Banbot.hpp"
#include <pthread.h>

void *foo(void *truc) {
	return truc;
}


int main(void) {
	Banbot bot;

	try {
		bot.parse_conf_file();
		bot.initial_connection();
		bot.routine();
		pthread_join(bot.id_thread, NULL);
	}
	catch (exception& x) {
		cout << RED << x.what() << RESET << endl;
	}
}
