#ifndef MOBILE_CLIENT_HPP
#define MOBILE_CLIENT_HPP

#include <NetConfAgent.hpp>

namespace mobileclient {

class MobileClient {
public:
    void setName(const std::string& name);

    bool registerClient(const std::string& number);

    void call(const std::string& incomingNumber);

    void handleOperData(std::string& xpath, std::string& operValue);

    void stopClient();

private:
    std::string _name;
    std::string _number;
    std::string _xpathState;
    const std::string _moduleName = "mobile-network";
    std::unique_ptr<netconfag::NetConfAgent> _agent;
};

}; // namespace mobileclient ends

#endif // MOBILE_CLIENT_HPP
