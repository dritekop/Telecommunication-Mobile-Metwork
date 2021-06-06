#ifndef NET_CONF_AGENT_HPP
#define NET_CONF_AGENT_HPP

#include <memory>

#include <sysrepo-cpp/Session.hpp>
#include <sysrepo-cpp/Connection.hpp>

class NetConfAgent {
public:
    void initSysrepo();

    void closeSysrepo();

    void fetchData(std::string, int8_t);

    //void subscribeforModelChanges(std::string& number);

    // void registerOperData();

    // void subscribeForRpc();

    // void notifySysrepo();

    void changeData(const std::string&, const std::string&, uint8_t);

private:
    sysrepo::S_Connection _s_conn;
    sysrepo::S_Session _s_sess;
    sysrepo::S_Subscribe _s_sub;
};

#endif // NET_CONF_AGENT_HPP

