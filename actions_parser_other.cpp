#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <map>
#include <functional>
#include <stdexcept>
#include <regex>
#include <cstdlib>

#define START 0
#define SECOND_WORD 1
#define ONE_FOR_SPACE 1
#define ONE_ARG 1
#define TWO_ARGS 2

void action_detect(std::string&);
void f_register(const std::vector<std::string>&);
void f_unregister(const std::vector<std::string>&);
void f_call(const std::vector<std::string>&);
void f_name(std::vector<std::string>&);
void f_answer(const std::vector<std::string>&);
void f_call_end(const std::vector<std::string>&);
void f_reject(const std::vector<std::string>&);

std::string s_register = "register";
std::string s_unregister = "unregister";
std::string s_call = "call";
std::string s_name = "name";
std::string s_answer = "answer";
std::string s_reject = "reject";
std::string s_call_end = "callEnd";

std::map<const std::string, std::function<void(std::vector<std::string>&)>> command_list {
    {s_register, f_register},
    {s_unregister, f_unregister},
    {s_call, f_call},
    {s_name, f_name},
    {s_answer, f_answer},
    {s_reject, f_reject},
    {s_call_end, f_call_end},
};

int main() {
    std::string input_line;

    while(true) {
        std::getline(std::cin, input_line);
        action_detect(input_line);
    }
}

void f_register(const std::vector<std::string>& line_tokens) {
    if (line_tokens.size() != TWO_ARGS) {
        std::cout << "Wrong number of the arguments\n";
        return;
    }

    std::string number = line_tokens.at(SECOND_WORD);
    std::regex num_regex("\\+380([0-9]{9})");

    if (std::regex_match(number,num_regex)) {
        std::cout << number << std::endl;
        std::cout << "Here I should initialize the subscriber instance in the userlist.\n";
    } else {
        std::cout << "Number should have format +380XXXXXXXXX\n";
    }
}

void f_unregister(const std::vector<std::string>& line_tokens) {
    if (line_tokens.size() != ONE_ARG) {
        std::cout << "This command doesn't need any argument\n";
        return;
    }

    std::cout << "Here I should delete the subscriber instance from the userlist.\n";
}

void f_call(const std::vector<std::string>& line_tokens) {
    if (line_tokens.size() != TWO_ARGS) {
        std::cout << "Wrong number of the arguments\n";
        return;
    }

    std::string number = line_tokens.at(SECOND_WORD);
    std::regex num_regex("\\+380([0-9]{9})");

    if (std::regex_match(number, num_regex)) {
        std::cout << number << std::endl;
        std::cout << "Here I should initialize the changes of other subcriber.\n"; 
    } else {
        std::cout << "Number should have format +380XXXXXXXXX\n";
    }
}

void f_name(std::vector<std::string>& line_tokens) {
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
        std::cout << name << std::endl;
        std::cout << "Here I should change leaf name.\n";
    } else {
        std::cout << "Error. Change the name.\n";
    }
}

void f_answer(const std::vector<std::string>& line_tokens) {
    if (line_tokens.size() != ONE_ARG) {
        std::cout << "This command doesn't need any argument\n";
        return;
    }

    std::cout << "changing the state from active to busy\n";
}

void f_reject(const std::vector<std::string>& line_tokens) {
    if (line_tokens.size() != ONE_ARG) {
        std::cout << "This command doesn't need any argument\n";
        return;
    }

    std::cout << "caller and this number are no more active\n";
}

void f_call_end(const std::vector<std::string>& line_tokens) {
    if (line_tokens.size() != ONE_ARG) {
        std::cout << "This command doesn't need any argument\n";
        return;
    }

    std::cout << "caller and this number are no more busy\n";
}



void action_detect(std::string& input_line) {
    std::vector<std::string> words = {};
    std::string space_delimiter = " ";
    std::string command;

    size_t pos = START;
    while ((pos = input_line.find(space_delimiter)) != std::string::npos) {
        words.push_back(input_line.substr(START, pos));
        input_line.erase(START, pos + space_delimiter.length());
    }
    words.push_back(input_line);

    try {
        command = words.at(START);

        if (!command.compare("exit")) {
            std::cout << "Thank you for using our product\n";
            std::exit(EXIT_SUCCESS);
        }

    } catch (std::out_of_range& e) {
        std::cout << "Sorry, error in my mind\n";
    }

    try {
        command_list.at(command)(words);
    } catch (std::out_of_range& e) {
        std::cout << "Invalid operation\n";
    }
}

