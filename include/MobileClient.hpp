#ifndef MOBILE_CLIENT_HPP
#define MOBILE_CLIENT_HPP

#include <NetConfAgent.hpp>

namespace mobileclient {

class MobileClient {
public:
    void setName(const std::string& name);

    bool registerClient(const std::string& number);

    void call(const std::string& incomingNumber);

    void answer();

    void reject();

    void callEnd();

    void handleModuleChange() const;

    void handleOperData(std::string& xpath, std::string& operValue) const;

    bool getCallInitializer() const;

    std::string getXpathState() const;

    std::string getModuleName() const;

    std::string getNumber() const;

    void stopClient();

private:
    bool _callInitializer = false;
    std::string _name;
    std::string _number;
    std::string _xpathState;
    std::string _xpathIncomingNumber;
    std::string _xpathUserName;
    const std::string _moduleName = "mobile-network";
    std::unique_ptr<netconfag::NetConfAgent> _agent;
};

}; // namespace mobileclient ends

#endif // MOBILE_CLIENT_HPP
