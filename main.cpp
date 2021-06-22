#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <stdexcept>
#include <regex>
#include <thread>

#include <MobileClient.hpp>

void actionDetect(std::string&, std::unique_ptr<mobileclient::MobileClient>&);
void funRegister(const std::vector<std::string>&, std::unique_ptr<mobileclient::MobileClient>&);
void funUnregister(const std::vector<std::string>&, std::unique_ptr<mobileclient::MobileClient>&);
void funCall(const std::vector<std::string>&, std::unique_ptr<mobileclient::MobileClient>&);
void funName(const std::vector<std::string>&, std::unique_ptr<mobileclient::MobileClient>&);
void funAnswer(const std::vector<std::string>&, std::unique_ptr<mobileclient::MobileClient>&);
void funCallEnd(const std::vector<std::string>&, std::unique_ptr<mobileclient::MobileClient>&);
void funReject(const std::vector<std::string>&, std::unique_ptr<mobileclient::MobileClient>&);
void funExit(const std::vector<std::string>&, std::unique_ptr<mobileclient::MobileClient>&);

namespace {
    const int8_t START = 0;
    const int8_t SECOND_WORD = 1;
    const int8_t ONE_FOR_SPACE = 1;
    const uint8_t ONE_ARG = 1;
    const uint8_t TWO_ARGS = 2;
    bool exitHandler = false;

    const std::string strRegister = "register";
    const std::string strUnregister = "unregister";
    const std::string strCall = "call";
    const std::string strName = "name";
    const std::string strAnswer = "answer";
    const std::string strReject = "reject";
    const std::string strCallEnd = "callEnd";
    const std::string strExit = "exit";

    std::map<const std::string, std::function<void(std::vector<std::string>&, std::unique_ptr<mobileclient::MobileClient>&)>> commandList 
    {
        {strRegister, funRegister},
        {strUnregister, funUnregister},
        {strCall, funCall},
        {strName, funName},
        {strAnswer, funAnswer},
        {strReject, funReject},
        {strCallEnd, funCallEnd},
        {strExit, funExit}
    };
};

int main() 
{
    std::string inputLine;

    auto user = std::make_unique<mobileclient::MobileClient>();

    while(true) 
    {
        std::getline(std::cin, inputLine);
        actionDetect(inputLine, user);
    }
}

void actionControl() 
{
    if (!exitHandler) 
    {
        std::cout << "Sorry...\n";
        std::exit(EXIT_FAILURE);
    }
}

void funRegister(const std::vector<std::string>& lineTokens, std::unique_ptr<mobileclient::MobileClient>& user) 
{
    if (lineTokens.size() != TWO_ARGS) 
    {
        std::cout << "Wrong number of the arguments\n";
        return;
    }

    std::string number = lineTokens.at(SECOND_WORD);
    std::regex numRegex("\\+380([0-9]{9})");

    if (std::regex_match(number,numRegex)) 
    {
        if (user->registerClient(number)) 
        {
            ::exitHandler = true;
            std::cout << "Subscriber was succesfully registered\n";
        } else 
        {
            std::cout << "Number is already registered by other user.\n";
        }
    } 
    else 
    {
        std::cout << "Number should have format +380XXXXXXXXX\n";
    }
}

void funUnregister(const std::vector<std::string>& lineTokens, std::unique_ptr<mobileclient::MobileClient>& user) 
{
    if (lineTokens.size() != ONE_ARG) 
    {
        std::cout << "This command doesn't need any argument\n";
        return;
    }

    if (::exitHandler) 
    {
        if (user->unregisterClient())
            ::exitHandler = false;
    }
    else 
    {
        std::exit(EXIT_FAILURE);
    }
}

void funCall(const std::vector<std::string>& lineTokens, std::unique_ptr<mobileclient::MobileClient>& user) 
{
    if (lineTokens.size() != TWO_ARGS) 
    {
        std::cout << "Wrong number of the arguments\n";
        return;
    }

    actionControl();

    std::string number = lineTokens.at(SECOND_WORD);
    std::regex numRegex("\\+380([0-9]{9})");

    if (std::regex_match(number, numRegex))
    {
        try 
        {
            user->call(number);
        } 
        catch (const std::exception& e) 
        {
            std::cout << "Invalid number.\n";
        } 
    } 
    else 
    {
        std::cout << "Number should have format +380XXXXXXXXX\n";
    }
}

void funName(const std::vector<std::string>& lineTokens, std::unique_ptr<mobileclient::MobileClient>& user) 
{
    if (lineTokens.size() < TWO_ARGS) 
    {
        std::cout << "Wrong number of the arguments\n";
        return;
    }

    std::string name = "";

    for (std::string i : lineTokens)
        name += i + " ";

    name.erase(START, strName.length() + ONE_FOR_SPACE);

    std::regex nameRegex("[a-zA-Z]([ a-zA-Z0-9]+)");

    if (std::regex_match(name,nameRegex)) 
    {
        user->setName(name);
    } 
    else 
    {
        std::cout << "Error. Change the name.\n";
    }
}

void funAnswer(const std::vector<std::string>& lineTokens, std::unique_ptr<mobileclient::MobileClient>& user) 
{
    if (lineTokens.size() != ONE_ARG) 
    {
        std::cout << "This command doesn't need any argument\n";
        return;
    }

    actionControl();

    user->answer();  
}

void funReject(const std::vector<std::string>& lineTokens, std::unique_ptr<mobileclient::MobileClient>& user) 
{
    if (lineTokens.size() != ONE_ARG) 
    {
        std::cout << "This command doesn't need any argument\n";
        return;
    }

    actionControl();

    user->reject();
}

void funCallEnd(const std::vector<std::string>& lineTokens, std::unique_ptr<mobileclient::MobileClient>& user) 
{
    if (lineTokens.size() != ONE_ARG) 
    {
        std::cout << "This command doesn't need any argument\n";
        return;
    }

    actionControl();

    user->callEnd();
}

void funExit(const std::vector<std::string>& lineTokens, std::unique_ptr<mobileclient::MobileClient>& user) 
{
    if (lineTokens.size() != ONE_ARG) 
    {
        std::cout << "This command doesn't need any argument\n";
        return;
    }

    if (::exitHandler) 
    {
        user->stopClient();
        std::cout << "Thank you for using our product\n";
    }

    std::exit(EXIT_SUCCESS);
}

void actionDetect(std::string& inputLine, std::unique_ptr<mobileclient::MobileClient>& user) 
{
    std::vector<std::string> words = {};
    std::string spaceDelimiter = " ";
    std::string command;

    size_t pos = START;
    while ((pos = inputLine.find(spaceDelimiter)) != std::string::npos) 
    {
        words.push_back(inputLine.substr(START, pos));
        inputLine.erase(START, pos + spaceDelimiter.length());
    }
    words.push_back(inputLine);

    command = words.at(START);

    try 
    {
        commandList.at(command)(words, user);
    } 
    catch (std::out_of_range& e) 
    {
        std::cout << "Invalid operation\n";
    }
}

