#include <iostream>
#include <string>
#include <cstdlib>
#include <regex>

#define WITHOUT_REGISTER_WORD_LENGTH 9
#define WITHOUT_CALL_WORD_LENGTH 5
#define WITHOUT_NAME_WORD_LENGTH 5

void action_detect(std::string&);
void registration(std::string&);
void calling(std::string&);
void change_name(std::string&);

int main(int argc, char** argv)
{
    std::string action_name;
    while(true) {
        std::getline(std::cin, action_name);
        action_detect(action_name);
    }
}

void registration(std::string& act) {
    std::string number = act.substr(WITHOUT_REGISTER_WORD_LENGTH);
    std::cout << number << std::endl;
    std::cout << "Here I should initialize the subscriber instance in the userlist.\n";
}

void calling(std::string& act) {
    std::string incoming_number = act.substr(WITHOUT_CALL_WORD_LENGTH);
    std::cout << incoming_number << std::endl;
    std::cout << "Here I should initialize the changes of other subcriber.\n";
}

void change_name(std::string& act) {
    std::string name = act.substr(WITHOUT_NAME_WORD_LENGTH);
    std::cout << name << std::endl;
    std::cout << "Here I should set the name of the user.\n";
}

void action_detect(std::string& act) {
    std::regex register_regex("[r][e][g][i][s][t][e][r][ ][+][3][8][0]([0-9]{9})",\
        std::regex::icase);
    std::regex call_regex("[c][a][l][l][ ][+][3][8][0]([0-9]{9})",\
        std::regex::icase);
    std::regex name_regex("[n][a][m][e][ ][a-zA-Z]([ a-zA-Z0-9]+)",\
        std::regex::icase);
    std::regex unregister_regex("[u][n][r][e][g][i][s][t][e][r]",\
        std::regex::icase);
    std::regex answer_regex("[a][n][s][w][e][r]",\
        std::regex::icase);
    std::regex reject_regex("[r][e][j][e][c][t]",\
        std::regex::icase);
    std::regex call_end_regex("[c][a][l][l][e][n][d]",\
        std::regex::icase);
    std::regex exit_regex("[e][x][i][t]",\
        std::regex::icase);

    if (std::regex_match(act, register_regex))
        registration(act);
    else if (std::regex_match(act, unregister_regex))
        std::cout << "Here I should delete the subscriber instance from the userlist.\n";
    else if (std::regex_match(act, call_regex))
        calling(act);
    else if (std::regex_match(act, name_regex))
        change_name(act);
    else if (std::regex_match(act, answer_regex))
        std::cout << "changes state to busy\n";
    else if (std::regex_match(act, reject_regex))
        std::cout << "caller is no more busy\n";
    else if (std::regex_match(act, call_end_regex))
        std::cout << "caller and this number are no more busy\n";
    else if (std::regex_match(act, exit_regex))
        std::exit(EXIT_SUCCESS);
    else
        std::cout << "Invalid operation\n";
}

