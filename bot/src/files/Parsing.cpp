//
// Created by loumouli on 4/1/23.
//

#include "Banbot.hpp"

void Banbot::parse_conf_file() {
	fstream conf_file;
	string  file_read;
	conf_file.open("./banbot.conf", ios::in);
	if (!conf_file.is_open()) {
		throw runtime_error(string("open conf file: ") + strerror(errno));
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
	parse_port(file_read);
	parse_address(file_read);
	parse_admin_pass(file_read);
	parse_botname(file_read);
	parse_password(file_read);
    parse_api();
	parse_banfile();
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
	this->sin.sin_port = htons(atoi(temp.c_str()));
	cout << BOLD_PURPLE <<  "port is [" << ntohs(this->sin.sin_port) << "]" << RESET << endl;
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
	this->sin.sin_addr.s_addr = inet_addr(temp.c_str());
	cout << BOLD_PURPLE <<  "address is [" << inet_ntoa(this->sin.sin_addr) << "]" << RESET << endl;
}

void Banbot::parse_admin_pass( string& file ) {
	unsigned long tok_pos = file.find("admin_password");
	if (tok_pos == string::npos ) {
		throw runtime_error("open conf file: cant find admin_password key");
	}
	tok_pos += strlen("admin_password=");
	unsigned long nl_pos = file.find('\n', tok_pos);
	if ( nl_pos == string::npos ) {
		throw runtime_error("open conf file: cant find newline for admin_password key");
	}
	string temp = file.substr(tok_pos, nl_pos - tok_pos);
	this->admin_pasword = temp;
	cout << BOLD_PURPLE <<  "admin_password is [" << this->admin_pasword << "]" << RESET << endl;
}

void Banbot::parse_botname( string& file) {
	unsigned long tok_pos = file.find("bot_name");
	if (tok_pos == string::npos ) {
		throw runtime_error("open conf file: cant find bot_name key");
	}
	tok_pos += strlen("bot_name=");
	unsigned long nl_pos = file.find('\n', tok_pos);
	if ( nl_pos == string::npos ) {
		throw runtime_error("open conf file: cant find newline for bot_name key");
	}
	string temp = file.substr(tok_pos, nl_pos - tok_pos);
	this->bot_nickname = temp;
	cout << BOLD_PURPLE <<  "bot_name is [" << this->bot_nickname << "]" << RESET << endl;
}

void Banbot::parse_password( string& file) {
	unsigned long tok_pos = file.find("serv_passowrd");
	if (tok_pos == string::npos ) {
		throw runtime_error("open conf file: cant find serv_passowrd key");
	}
	tok_pos += strlen("serv_passowrd=");
	unsigned long nl_pos = file.find('\n', tok_pos);
	if ( nl_pos == string::npos ) {
		throw runtime_error("open conf file: cant find newline for serv_passowrd key");
	}
	string temp = file.substr(tok_pos, nl_pos - tok_pos);
	this->serv_pass = temp;
	cout << BOLD_PURPLE <<  "serv_passowrd is [" << this->serv_pass << "]" << RESET << endl;
}

void    Banbot::parse_api( )
{
    ifstream    file;

    file.open("./.env");
    if (file && file.is_open())
    {
        string	line;
        while (getline(file, line))
        {
            unsigned long tok = line.find("API_KEY=");
            if (tok != string::npos)
            {
                this->_api = line.substr(tok + ::strlen("API_KEY="));
                cout << BOLD_PURPLE <<  "api is [" << this->_api << "]" << RESET << endl;
                file.close();
                return ;
            }
        }
        file.close();
        throw runtime_error(string(".env file: no API_KEY var found, use API_KEY=your_key"));
    }
    else
    {
        throw runtime_error(string("open .env file: ") + strerror(errno));
    }
}

void Banbot::parse_banfile( ) {
	//opening the file and reading it into a string
	fstream file;
	file.open("./banword_file", ios::in);
	if (!file.is_open()) {
		throw runtime_error(string("open banword_file: ") + strerror(errno));
	}
	string result;
	while(true) {
		string temp_str;
		file >> temp_str;
		result += temp_str;
		if (file.eof()) {
			break ;
		}
		result += ",";
	}
	//parsing the file into a vector of banword string
	string	buff;
	size_t	pos;
	while ((pos = result.find(',')) != string::npos) {
		buff = result.substr(0, pos);
		this->ban_word.push_back(buff);
		result.erase(0, pos + 1);
	}
	this->ban_word.push_back(result);

	cout << BOLD_PURPLE << "banword list is [";
	for ( vector<string>::iterator it = this->ban_word.begin(); it != this->ban_word.end(); it++ ) {
		cout << *it;
		if ((it + 1) == this->ban_word.end()) {
			continue ;
		}
		cout << "-";
	}
	cout << "]" << RESET << endl;
    file.close();
}