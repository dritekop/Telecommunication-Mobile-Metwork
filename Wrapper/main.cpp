#include "./headers/Wrapper.hpp"
#include <iostream>

#define TEN_SECONDS 10000

int main() {
    wrapper::Wrapper test;
    std::time_t t = std::time(nullptr);
    std::cout
    << std::asctime(std::localtime(&t));
    test.sleep(TEN_SECONDS);
    t = std::time(nullptr);
    std::cout
    << std::asctime(std::localtime(&t)); 
}
