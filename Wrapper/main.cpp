#include "./headers/Wrapper.hpp"
#include <iostream>

int main() {
    wrapper::Wrapper test;
    std::time_t t = std::time(nullptr);
    std::cout
    << std::asctime(std::localtime(&t));
    test.sleep(10000);
    t = std::time(nullptr);
    std::cout
    << std::asctime(std::localtime(&t)); 
}
