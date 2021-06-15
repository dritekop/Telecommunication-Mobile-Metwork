#ifndef MOBILE_CLIENT_HPP
#define MOBILE_CLIENT_HPP

#include <NetConfAgent.hpp>

namespace mobileclient {

class MobileClient {
public:
    bool registerClient(const std::string& number);

    void call(const std::string& incoming_number);

    void stopClient();

private:
    std::string _name;
    std::string _number;
    std::string _xpath_state;
    std::unique_ptr<netconfag::NetConfAgent> _agent;
};

}; // namespace mobileclient ends

#endif // MOBILE_CLIENT_HPP
