//
// Created by loumouli on 4/1/23.
//

#include "Banbot.hpp"

bool server_up = true;

void handler_sigint(int sig) {
	(void) sig;
	server_up = false;
}

string requestify(string const &str) {
	string res;
	res = "{\"model\": \"gpt-3.5-turbo\", \"messages\": [{\"role\": \"user\", \"content\": \"" + str + "\"}]}";
	return (res);
}

string composeAnswer(string &msg, string &user)
{
    return ("PRIVMSG " + user + " :" + msg + endmsg);
}

size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
	size_t realsize = size * nmemb;
	t_ms *mem;

	mem = (t_ms *)userp;
	char *ptr = (char *)realloc(mem->memory, mem->size + realsize + 1);
	if (ptr == NULL)
	{
		free(mem->memory);
		throw runtime_error(string("WriteMemoryCallback: enomem"));
	}
	mem->memory = ptr;
	memcpy(&(mem->memory[mem->size]), contents, realsize);
	mem->size += realsize;
	mem->memory[mem->size] = '\0';
	return (realsize);
}

string	Banbot::chatgpt(string const &str) {
	CURL *curl;
	CURLcode res;
	string request;
	string api;
	t_ms chunk;
	struct curl_slist *headers = NULL;

	if (str.find("PRIVMSG") == string::npos)
        return ("");
    unsigned long tok1 = str.find(':', 0);
    unsigned long tok2 = str.find(':', tok1 + 1);
    if (str.size() > 426)
        return ("I am sorry but your demand exceeds the limit set for basic request");
    chunk.size = 0;
	chunk.memory = (char *)malloc(1);
    if (!chunk.memory)
    {
        throw runtime_error(string("chatGpt: enomem"));
    }
	api = "Authorization: Bearer " + this->getApi();
	request = requestify(str.substr(tok2 + 1));
	curl = curl_easy_init();
	if (curl)
    {
		try
        {
            cout << CYAN << "Request sent: " << request << RESET << endl;
			headers = curl_slist_append(headers, "Content-Type: application/json");
			headers = curl_slist_append(headers, api.c_str());
			curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
			curl_easy_setopt(curl, CURLOPT_URL, "https://api.openai.com/v1/chat/completions");
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, request.c_str());
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) &chunk);
			res = curl_easy_perform(curl);
			if (res != CURLE_OK)
				cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << endl;
			curl_easy_cleanup(curl);
			curl_slist_free_all(headers);
            string ret(chunk.memory);
            free(chunk.memory);
            return (ret);
		}
		catch (exception &x)
        {
			cout << RED << x.what() << RESET << endl;
			curl_easy_cleanup(curl);
			curl_slist_free_all(headers);
			throw runtime_error(string("chatgpt error:") + ::strerror(errno));
		}
	}
	return ("");
}

void Banbot::search_chan(string &str) const {
	if (str.find("End of LIST") != string::npos) {
		throw invalid_argument("end of list");
	}
	size_t pos_hastag = str.find('#');
	if (pos_hastag == string::npos) {
		throw invalid_argument("wrong chan name");
	}
	str.erase(0, pos_hastag + 1);
	size_t pos_space = str.find(' ');
	if (pos_space == string::npos) {
		throw invalid_argument("cant find space in chan respond");
	}
	str.erase(pos_space, str.size());
	string msg = "JOIN #" + str + endmsg;
	ssize_t ret_val = send_msg(msg);
	if (ret_val == -1) {
		throw runtime_error(string("send: ") + strerror(errno));
	}
	clear_resize(msg);
	ret_val = recv_msg(msg);
	if (ret_val == -1) {
		throw runtime_error(string("recv: ") + strerror(errno));
	}
}

void Banbot::parse_recv_msg(string &str) {
	while (true) {
		size_t pos = str.find(endmsg);
		if (pos != string::npos) {
			string temp = str.substr(0, pos);
			try {
				search_chan(temp);
			}
			catch (invalid_argument &x) {
				return;
			}
			str.erase(0, pos + 2);
		} else
			break;
	}
}

void Banbot::initial_chan_join() {
	string msg = string("LIST ") + endmsg;
	ssize_t ret_val = send_msg(msg);
	if (ret_val == -1) {
		throw runtime_error(string("send: ") + strerror(errno));
	}
	clear_resize(msg);
	ret_val = recv_msg(msg);
	if (ret_val == -1) {
		throw runtime_error(string("recv: ") + strerror(errno));
	}
	msg.resize(ret_val);
	parse_recv_msg(msg);
}

int Banbot::search_word(string &msg) {
	cout << "OG msg = [" << msg << "]" << endl;
	string saving = msg;
	size_t pos = msg.find("PRIVMSG");
	if (pos == string::npos)
		return 0;
	pos += strlen("PRIVMSG") + 1;
	msg.erase(0, pos);
	string token = msg.substr(0, msg.find(' '));
	if (token == this->bot_nickname) {
		msg = saving;
		return 0;
	}
	token.erase(0, 1);
	string content = msg.substr(msg.find(':') + 1, msg.length());
	string user = saving.substr(saving.find(':') + 1, saving.find('!') - 1);
	for ( vec_str_iter it = this->ban_word.begin(); it != this->ban_word.end(); it++ ) {
		string tmp = *it;
		if ( content.find(*it) != string::npos) {
			string msg_to_send = "KICK #";
			msg_to_send += token + " ";
			msg_to_send += user + endmsg;
			cout << "sending " << msg_to_send << endl;
			ssize_t ret_val = send_msg(msg_to_send);
			if (ret_val == -1) {
				throw runtime_error(string("send: ") + strerror(errno));
			}
		}
	}
	return (1);
}

void	parse_the_answer(string &ans)
{
	unsigned long tok = ans.find("\"content\":\"", 0);
	if (tok != string::npos)
	{
		ans = ans.substr(tok + ::strlen("\"content\":\""));
		unsigned long tok2 = ans.rfind("\"},\"finish_reason\":\"");
		if (tok2 != string::npos)
		{
			ans = ans.substr(0, tok2);
			unsigned long tok3 = ans.find("\\n", 0);
			while (tok3 != string::npos)
			{
				ans.replace(tok3, 2, " ");
				tok3 = ans.find("\\n", tok3);
			}
		}
		else
		{
			ans = "Sorry, error in feedback :(";
		}
	}
}

void splitString(const string &inputString, vector<string> &chunks, size_t const &n)
{
	istringstream       ss(inputString);
	string              word;
	string              chunk;

	while (ss >> word)
	{
		if (chunk.size() + word.size() + 1 > n)
		{
			chunks.push_back(chunk);
			chunk.clear();
		}
		if (!chunk.empty())
			chunk += " ";
		chunk += word;
	}
	if (!chunk.empty())
		chunks.push_back(chunk);
}

void Banbot::check_all_chan() {
	while (server_up) {
		string msg;
		//receive data in a non-blocking way
		clear_resize(msg);
		ssize_t ret_val = recv_msg_nonblock(msg);
		if (ret_val == -1) {
			//check if fail is due to blocking operation
			if (errno == EAGAIN)
				continue;
			throw runtime_error(string("recv: ") + strerror(errno));
		}
		if (ret_val == 0)
			continue;
		msg.resize(ret_val);
		unsigned long pos;
		while ((pos = msg.find(endmsg)) != string::npos) {
			string buff = msg.substr(0, pos);
			if (search_word(buff)) {}
			else {
                // Send buff to chatgpt
				string answer = chatgpt(buff);
                cout << CYAN << answer << RESET << endl;
				parse_the_answer(answer);
				if (answer.length())
				{
					string saving = msg;
					size_t pos2 = msg.find(':');
					pos2 += 1;
					msg.erase(0, pos2);
					string user = saving.substr(saving.find(':') + 1, saving.find('!') - 1);
                    vector<string> chunk;
                    if (answer.size() > CHUNK_SIZE)
                        splitString(answer, chunk, CHUNK_SIZE);
                    else
                        chunk.push_back(answer);
                    for (size_t i = 0; i < chunk.size(); ++i)
                    {
                        ssize_t k = send_msg(composeAnswer(chunk[i], user));
                        if (k == -1)
                            throw runtime_error(string("send gpt answer: ") + strerror(errno));
                        sleep(1);
                    }
				}
			}
			msg.erase(0, pos + 1);
		}
	}
}

void Banbot::routine() {
    cout << GREEN << "starting the bot routine" << RESET << endl;
	signal(SIGINT, handler_sigint);
	initial_chan_join();
    string msg;
    while(server_up) {
		check_all_chan();
	}
	msg = "QUIT :STOPING THE SERVER\r\n";
	send_msg(msg);
	clear_resize(msg);
	recv_msg(msg);
}