//
// Created by loumouli on 4/1/23.
//

#include "Banbot.hpp"

Banbot::Banbot(  ) {
	this->port = 0;
	this->address = 0;
	this->socket = 0;
}

Banbot::~Banbot() {
	if (this->socket)
		close(this->socket);
}

void Banbot::parse_conf_file() {
	fstream conf_file;
	string  file_read;
	conf_file.open("./banbot.conf", ios::in);
	if (!conf_file.is_open()) {
		throw runtime_error(string("open conf file:") + strerror(errno));
	}
	while (true) {
		char	temp[10000];
		conf_file.getline(temp, 10000);
		file_read += temp;
		if (conf_file.eof()) {
			break ;
		}
		file_read += "\n";
	}
	if (file_read[file_read.size() - 1] != '\n') {
		file_read += "\n";
	}
	cout << "contenf of file is [" << file_read << "]" << endl;
	parse_port(file_read);
	parse_address(file_read);
	conf_file.close();
}


void Banbot::parse_port( string& file) {
	unsigned long tok_pos = file.find("port");
	if (tok_pos == string::npos ) {
		throw runtime_error("open conf file: cant find port key");
	}
	tok_pos += strlen("port=");
	unsigned long nl_pos = file.find('\n', tok_pos);
	if ( nl_pos == string::npos ) {
		throw runtime_error("open conf file: cant find newline for port key");
	}
	string temp = file.substr(tok_pos, nl_pos - tok_pos);
	this->port = atoi(temp.c_str());
	cout << BOLD_GREEN <<  "port is [" << this->port << "]" << RESET << endl;
}

void Banbot::parse_address( string& file) {
	unsigned long tok_pos = file.find("address");
	if (tok_pos == string::npos ) {
		throw runtime_error("open conf file: cant find address key");
	}
	tok_pos += strlen("address=");
	unsigned long nl_pos = file.find('\n', tok_pos);
	if ( nl_pos == string::npos ) {
		throw runtime_error("open conf file: cant find newline for address key");
	}
	string temp = file.substr(tok_pos, nl_pos - tok_pos);
	cout << "temp = " << temp << endl;
	this->port = inet_addr(temp.c_str());
	cout << BOLD_GREEN <<  "address is [" << this->port << "]" << RESET << endl;
}