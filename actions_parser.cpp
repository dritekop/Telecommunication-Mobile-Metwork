#include <iostream>
#include <string>
#include <cstdlib>
#include <regex>

#define BEGIN 0
#define WHITESPACE_MOVE 1

void action_detect(std::string&);

int main(int argc, char** argv)
{
    std::string action_name;
    while(true) {
        std::getline(std::cin, action_name);
        action_detect(action_name);
    }
}

void f_register(std::string& number) {
    std::regex num_regex("[+][3][8][0]([0-9]{9})");
    if (std::regex_match(number,num_regex)) {
        std::cout << number << std::endl;
        std::cout << "Here I should initialize the subscriber instance in the userlist.\n";
    } else {
        std::cout << "Number should have format +380XXXXXXXXX\n";
    }
}

void f_unregister() {
    std::cout << "Here I should delete the subscriber instance from the userlist.\n";
}

void f_call(std::string& number) {
    std::regex num_regex("[+][3][8][0]([0-9]{9})");
    if (std::regex_match(number, num_regex)) {
        std::cout << number << std::endl;
        std::cout << "Here I should initialize the changes of other subcriber.\n";
    } else {
        std::cout << "Number should have format +380XXXXXXXXX\n";
    }
}

void f_name(std::string& name) {
    std::regex name_regex("[a-zA-Z]([ a-zA-Z0-9]+)");
    if (std::regex_match(name,name_regex)) {
        std::cout << name << std::endl;
        std::cout << "Here I should change leaf name.\n";
    } else {
        std::cout << "Error. Change the name.\n";
    }
}

void f_answer() {
    std::cout << "changing the state from active to busy\n";
}

void f_reject() {
    std::cout << "caller and this number are no more active\n";
}

void f_call_end() {
    std::cout << "caller and this number are no more busy\n";
}

void action_detect(std::string& taken_line) {
    std::string command = taken_line;
    std::string arg;

    std::size_t found = command.find(" ");

    if (found != std::string::npos) {
        found += WHITESPACE_MOVE;
        arg = command.substr(found);
        found -= WHITESPACE_MOVE;
        command = command.substr(BEGIN, found);
    }

    if (!command.compare("register")) {
        f_register(arg);
    }
    else if (!command.compare("unregister")) {
        f_unregister();
    }
    else if (!command.compare("call")) {
        f_call(arg);
    }
    else if (!command.compare("name")) {
        f_name(arg);
    }
    else if (!command.compare("answer")) {
        f_answer();
    }
    else if (!command.compare("reject")) {
        f_reject();
    }
    else if (!command.compare("callEnd")) {
        f_call_end();
    }
    else if (!command.compare("exit")) {
        std::exit(EXIT_SUCCESS);
    }
    else {
        std::cout << "Invalid operation\n";
    }
}

