#ifndef NET_CONF_AGENT_MOCK_HPP
#define NET_CONF_AGENT_MOCK_HPP

#include <gmock/gmock.h>

#include <NetConfAgent.hpp>

class NetConfAgentMock : public netconfag::NetConfAgent {
public:
    MOCK_METHOD(bool, initSysrepo, (), (override));
    MOCK_METHOD(bool, closeSys, (), (override));
    MOCK_METHOD(bool, fetchData, ((std::map<std::string, std::string>&  sXpathAndValue)), (override));
    MOCK_METHOD(bool, subscribeForModelChanges, (mobileclient::MobileClient& refUser, const std::string& moduleName), (override));
    MOCK_METHOD(bool, registerOperData, (mobileclient::MobileClient& refUser, const std::string& moduleName), (override));
    MOCK_METHOD(bool, changeData, (const std::string& sXpath, const std::string& value), (override));
};

#endif // NET_CONF_AGENT_MOCK_HPP
