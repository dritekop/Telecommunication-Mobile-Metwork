#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <stdexcept>
#include <regex>
#include <thread>

#include <NetConfAgent.hpp>

namespace {
    const int8_t START = 0;
    const int8_t SECOND_WORD = 1;
    const int8_t ONE_FOR_SPACE = 1;
    const uint8_t ONE_ARG = 1;
    const uint8_t TWO_ARGS = 2;
};

void action_detect(std::string&, std::unique_ptr<netconfag::NetConfAgent>&);
void f_register(const std::vector<std::string>&, std::unique_ptr<netconfag::NetConfAgent>&);
void f_unregister(const std::vector<std::string>&, std::unique_ptr<netconfag::NetConfAgent>&);
void f_call(const std::vector<std::string>&, std::unique_ptr<netconfag::NetConfAgent>&);
void f_name(const std::vector<std::string>&, std::unique_ptr<netconfag::NetConfAgent>&);
void f_answer(const std::vector<std::string>&, std::unique_ptr<netconfag::NetConfAgent>&);
void f_call_end(const std::vector<std::string>&, std::unique_ptr<netconfag::NetConfAgent>&);
void f_reject(const std::vector<std::string>&, std::unique_ptr<netconfag::NetConfAgent>&);
void f_exit(const std::vector<std::string>&, std::unique_ptr<netconfag::NetConfAgent>&);

std::string s_register = "register";
std::string s_unregister = "unregister";
std::string s_call = "call";
std::string s_name = "name";
std::string s_answer = "answer";
std::string s_reject = "reject";
std::string s_call_end = "callEnd";
std::string s_exit = "exit";

std::map<const std::string, std::function<void(std::vector<std::string>&, std::unique_ptr<netconfag::NetConfAgent>&)>> command_list 
{
    {s_register, f_register},
    {s_unregister, f_unregister},
    {s_call, f_call},
    {s_name, f_name},
    {s_answer, f_answer},
    {s_reject, f_reject},
    {s_call_end, f_call_end},
    {s_exit, f_exit}
};

int main() 
{
    std::string input_line;
    auto user = std::make_unique<netconfag::NetConfAgent>();

    if (!user->initSysrepo())
        std::exit(EXIT_FAILURE);

    std::string module = "/MOBILENETWORK:something";
    size_t amount = 2;
    std::map<std::string, std::string> names = {
        {"incomingNumber", "001"},
        {"state", "active"},
    };

    std::thread sub_for_rpc(&netconfag::NetConfAgent::subscribeForRpc, user.get(), module, amount, names);
    
    std::thread reg_op_data(&netconfag::NetConfAgent::registerOperData, user.get(),
        "mobile-network", "/mobile-network:core/subscribers[number='+380977777777']/userName", "Bob");

    std::thread sub_model_changes(&netconfag::NetConfAgent::subscribeForModelChanges, user.get(),
        "mobile-network");

    std::string x = "/mobile-network:core/subscribers[number='+380966666666']/state";
    std::string y;
    std::map<std::string, std::string> map_to_fetch = {
        {x , y},
    };
    user->fetchData(map_to_fetch);
    std::cout << map_to_fetch[x] << std::endl;

    
    std::string x1 = "val1";
    std::string x2 = "val2";
    std::string y1 = "VAL1";
    std::string y2 = "VAL2";
    std::map<std::string, std::string> map_to_notify = {
        {x1 , y1},
        {x2, y2}
    };
    if (user->notifySysrepo("/MOBILENETWORK:test-notif", map_to_notify))
        std::cout << "Notification is sent\n";

    while(true) 
    {
        std::getline(std::cin, input_line);
        action_detect(input_line, user);
    }
    
    sub_for_rpc.join();
    reg_op_data.join();
    sub_model_changes.join();

    user->closeSys();
}

void f_register(const std::vector<std::string>& line_tokens, std::unique_ptr<netconfag::NetConfAgent>& user) 
{
    if (line_tokens.size() != ::TWO_ARGS) {
        std::cout << "Wrong number of the arguments\n";
        return;
    }

    std::string number = line_tokens.at(::SECOND_WORD);
    std::regex num_regex("\\+380([0-9]{9})");

    if (std::regex_match(number,num_regex)) {
        std::cout << number << std::endl;
    } 
    else {
        std::cout << "Number should have format +380XXXXXXXXX\n";
    }
}

void f_unregister(const std::vector<std::string>& line_tokens, std::unique_ptr<netconfag::NetConfAgent>& user) 
{
    if (line_tokens.size() != ::ONE_ARG) {
        std::cout << "This command doesn't need any argument\n";
        return;
    }

    std::cout << __PRETTY_FUNCTION__ << std::endl;
}

void f_call(const std::vector<std::string>& line_tokens, std::unique_ptr<netconfag::NetConfAgent>& user) 
{
    if (line_tokens.size() != ::TWO_ARGS) {
        std::cout << "Wrong number of the arguments\n";
        return;
    }

    std::string number = line_tokens.at(::SECOND_WORD);
    std::regex num_regex("\\+380([0-9]{9})");

    if (std::regex_match(number, num_regex)) {
        try {
            std::cout << number << std::endl;
        } catch (const std::exception& e) {
            std::cout << "Invalid number.\n";
        } 
    } 
    else {
        std::cout << "Number should have format +380XXXXXXXXX\n";
    }
}

void f_name(const std::vector<std::string>& line_tokens, std::unique_ptr<netconfag::NetConfAgent>& user) 
{
    if (line_tokens.size() < ::TWO_ARGS) {
        std::cout << "Wrong number of the arguments\n";
        return;
    }

    std::string name = "";

    for (std::string i : line_tokens)
        name += i + " ";

    name.erase(::START, s_name.length() + ::ONE_FOR_SPACE);

    std::regex name_regex("[a-zA-Z]([ a-zA-Z0-9]+)");

    if (std::regex_match(name,name_regex)) {
        std::cout << name << std::endl;
    } 
    else {
        std::cout << "Error. Change the name.\n";
    }
}

void f_answer(const std::vector<std::string>& line_tokens, std::unique_ptr<netconfag::NetConfAgent>& user) 
{
    if (line_tokens.size() != ::ONE_ARG) {
        std::cout << "This command doesn't need any argument\n";
        return;
    }

    std::cout << __PRETTY_FUNCTION__ << std::endl;    
}

void f_reject(const std::vector<std::string>& line_tokens, std::unique_ptr<netconfag::NetConfAgent>& user) 
{
    if (line_tokens.size() != ::ONE_ARG) {
        std::cout << "This command doesn't need any argument\n";
        return;
    }

    std::cout << __PRETTY_FUNCTION__ << std::endl;
}

void f_call_end(const std::vector<std::string>& line_tokens, std::unique_ptr<netconfag::NetConfAgent>& user) 
{
    if (line_tokens.size() != ::ONE_ARG) {
        std::cout << "This command doesn't need any argument\n";
        return;
    }

    std::cout << __PRETTY_FUNCTION__ << std::endl;
}

void f_exit(const std::vector<std::string>& line_tokens, std::unique_ptr<netconfag::NetConfAgent>& user) 
{
    if (line_tokens.size() != ::ONE_ARG) {
        std::cout << "This command doesn't need any argument\n";
        return;
    }

    std::cout << "Thank you for using our product\n";

    std::exit(EXIT_SUCCESS);
}

void action_detect(std::string& input_line, std::unique_ptr<netconfag::NetConfAgent>& user) 
{
    std::vector<std::string> words = {};
    std::string space_delimiter = " ";
    std::string command;

    size_t pos = ::START;
    while ((pos = input_line.find(space_delimiter)) != std::string::npos) {
        words.push_back(input_line.substr(::START, pos));
        input_line.erase(::START, pos + space_delimiter.length());
    }
    words.push_back(input_line);

    command = words.at(::START);

    try {
        command_list.at(command)(words, user);
    } catch (std::out_of_range& e) {
        std::cout << "Invalid operation\n";
    }
}

