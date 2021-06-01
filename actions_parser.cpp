#include <iostream>
#include <string>
#include <cstdlib>
#include <regex>

void action_detect(std::string&);
void registration();
void calling();

int main(int argc, char** argv)
{
    std::string action_name;
    while(true) {
        std::cin >> action_name;
        action_detect(action_name);
    }
}

void registration() {
    std::string user_name;
    std::string number;

    std::cout << "Type username: ";
    std::cin >> user_name;
    std::cout << "Type number: ";
    std::cin >> number;
    std::regex num_regex("[+][3][8][0]([0-9]{9})");
    while (!std::regex_match(number,num_regex)) {
        std::cout << "Type standart number format +380XXXXXXXXX\n";
        std::cin >> number;
    }
}


void calling() {
    std::string incoming_number;
    std::cout << "Type number to call: ";
    std::cin >> incoming_number;
}

void action_detect(std::string& act) {
    if (!act.compare("register"))
        registration();
    else if (!act.compare("unregister"))
        std::cout << "Thank you for using our product.\n";
    else if (!act.compare("call"))
        calling();
    else if (!act.compare("answer"))
        std::cout << "HALLO!\n";
    else if (!act.compare("reject"))
        std::cout << "no\n";
    else if (!act.compare("callEnd"))
        std::cout << "Bye-bye\n";
    else if (!act.compare("exit"))
        std::exit(EXIT_SUCCESS);
    else
        std::cout << "Invalid operation\n";
}

