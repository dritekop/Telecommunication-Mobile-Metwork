#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <stdexcept>
#include <regex>
#include <cstdlib>
#include <NetConfAgent.hpp>

#define START 0
#define SECOND_WORD 1
#define ONE_FOR_SPACE 1
#define ONE_ARG 1
#define TWO_ARGS 2
#define REGISTER 0
#define CALL 1
#define ANSWER 2
#define REJECT 3
#define CALLEND 4

void action_detect(std::string&, std::unique_ptr<NetConfAgent>&);
void f_register(const std::vector<std::string>&, std::unique_ptr<NetConfAgent>&);
void f_unregister(const std::vector<std::string>&, std::unique_ptr<NetConfAgent>&);
void f_call(const std::vector<std::string>&, std::unique_ptr<NetConfAgent>&);
void f_name(const std::vector<std::string>&, std::unique_ptr<NetConfAgent>&);
void f_answer(const std::vector<std::string>&, std::unique_ptr<NetConfAgent>&);
void f_call_end(const std::vector<std::string>&, std::unique_ptr<NetConfAgent>&);
void f_reject(const std::vector<std::string>&, std::unique_ptr<NetConfAgent>&);
void f_exit(const std::vector<std::string>&, std::unique_ptr<NetConfAgent>&);

std::string _number = "";
std::string _xpath = "";
std::string _incoming_number = "";

std::string s_register = "register";
std::string s_unregister = "unregister";
std::string s_call = "call";
std::string s_name = "name";
std::string s_answer = "answer";
std::string s_reject = "reject";
std::string s_call_end = "callEnd";
std::string s_exit = "exit";

std::map<const std::string, std::function<void(std::vector<std::string>&, std::unique_ptr<NetConfAgent>&)>> command_list {
    {s_register, f_register},
    {s_unregister, f_unregister},
    {s_call, f_call},
    {s_name, f_name},
    {s_answer, f_answer},
    {s_reject, f_reject},
    {s_call_end, f_call_end},
    {s_exit, f_exit}
};

int main() {
    std::string input_line;
    auto user = std::make_unique<NetConfAgent>();

    try {
        user->initSysrepo();
    } catch ( const std::exception& e ) {
        std::cout << "Error. Try to start server.\n";
        std::exit(EXIT_FAILURE);
    }

    while(true) {
        std::getline(std::cin, input_line);
        action_detect(input_line, user);
    }
}

void f_register(const std::vector<std::string>& line_tokens, std::unique_ptr<NetConfAgent>& user) {
    if (line_tokens.size() != TWO_ARGS) {
        std::cout << "Wrong number of the arguments\n";
        return;
    }

    std::string number = line_tokens.at(SECOND_WORD);
    std::regex num_regex("\\+380([0-9]{9})");

    if (std::regex_match(number,num_regex)) {
        _xpath = "/mobile-network:core/subscribers[number='" + number + "']";
        user->changeData("", "", _xpath, "", REGISTER);
        _number = number;
    } else {
        std::cout << "Number should have format +380XXXXXXXXX\n";
    }
}

void f_unregister(const std::vector<std::string>& line_tokens, std::unique_ptr<NetConfAgent>& user) {
    if (!_number.length()) {
        std::cout << "No registered number\n";
        return;
    }

    if (line_tokens.size() != ONE_ARG) {
        std::cout << "This command doesn't need any argument\n";
        return;
    }

    std::cout << "Here I should delete the subscriber instance from the userlist.\n";
}

void f_call(const std::vector<std::string>& line_tokens, std::unique_ptr<NetConfAgent>& user) {
    if (!_number.length()) {
        std::cout << "No registered number\n";
        return;
    }

    if (_incoming_number.length()) {
        std::cout << "Sibscriber is busy\n";
        return;
    }

    if (line_tokens.size() != TWO_ARGS) {
        std::cout << "Wrong number of the arguments\n";
        return;
    }

    std::string number = line_tokens.at(SECOND_WORD);
    std::regex num_regex("\\+380([0-9]{9})");

    if (std::regex_match(number, num_regex)) {
        try {
            std::string host_xpath = _xpath;
            std::string guest_xpath = "";
            guest_xpath += "/mobile-network:core/subscribers[number='" + number + "']";
            _incoming_number = number;
            user->changeData(_number, _incoming_number, host_xpath, guest_xpath, CALL);
        } catch (const std::exception& e) {
            std::cout << "Invalid number.\n";
        } 
    } else {
        std::cout << "Number should have format +380XXXXXXXXX\n";
    }
}

void f_name(const std::vector<std::string>& line_tokens, std::unique_ptr<NetConfAgent>& user) {
    if (!_number.length()) {
        std::cout << "No registered number\n";
        return;
    }

    if (line_tokens.size() < TWO_ARGS) {
        std::cout << "Wrong number of the arguments\n";
        return;
    }

    std::string name = "";

    for (std::string i : line_tokens)
        name += i + " ";

    name.erase(START, s_name.length() + ONE_FOR_SPACE);

    std::regex name_regex("[a-zA-Z]([ a-zA-Z0-9]+)");

    if (std::regex_match(name,name_regex)) {
        // user->registerOperData();
    } else {
        std::cout << "Error. Change the name.\n";
    }
}

void f_answer(const std::vector<std::string>& line_tokens, std::unique_ptr<NetConfAgent>& user) {
    if (!_number.length()) {
        std::cout << "No registered number\n";
        return;
    }

    if (line_tokens.size() != ONE_ARG) {
        std::cout << "This command doesn't need any argument\n";
        return;
    }

    // user->changeData();    
}

void f_reject(const std::vector<std::string>& line_tokens, std::unique_ptr<NetConfAgent>& user) {
    if (!_number.length()) {
        std::cout << "No registered number\n";
        return;
    }

    if (line_tokens.size() != ONE_ARG) {
        std::cout << "This command doesn't need any argument\n";
        return;
    }

    // user->changeData();
}

void f_call_end(const std::vector<std::string>& line_tokens, std::unique_ptr<NetConfAgent>& user) {
    if (!_number.length()) {
        std::cout << "No registered number\n";
        return;
    }

    if (line_tokens.size() != ONE_ARG) {
        std::cout << "This command doesn't need any argument\n";
        return;
    }

    // user->changeData();
}

void f_exit(const std::vector<std::string>& line_tokens, std::unique_ptr<NetConfAgent>& user) {
    if (line_tokens.size() != ONE_ARG) {
        std::cout << "This command doesn't need any argument\n";
        return;
    }

    if (_number.length())
        std::cout << "Thank you for using our product\n";
    
    user->closeSysrepo();
    std::exit(EXIT_SUCCESS);
}

void action_detect(std::string& input_line, std::unique_ptr<NetConfAgent>& user) {
    std::vector<std::string> words = {};
    std::string space_delimiter = " ";
    std::string command;

    size_t pos = START;
    while ((pos = input_line.find(space_delimiter)) != std::string::npos) {
        words.push_back(input_line.substr(START, pos));
        input_line.erase(START, pos + space_delimiter.length());
    }
    words.push_back(input_line);

    command = words.at(START);

    try {
        command_list.at(command)(words, user);
    } catch (std::out_of_range& e) {
        std::cout << "Invalid operation\n";
    }
}

