#ifndef NET_CONF_AGENT_HPP
#define NET_CONF_AGENT_HPP

#include <memory>

#include <sysrepo-cpp/Session.hpp>
#include <sysrepo-cpp/Connection.hpp>

class NetConfAgent {
public:
    void initSysrepo();

    void closeSysrepo();

    // void fetchData();

    //void subscribeforModelChanges(std::string& number);

    // void registerOperData();

    // void subscribeForRpc();

    // void notifySysrepo();

    // void changeData();

private:
    // std::unique_ptr<sysrepo::Connection> conn;
    // std::unique_ptr<sysrepo::Session> sess;
    // std::unique_ptr<sysrepo::Subscribe> sub;
    sysrepo::S_Connection s_conn;
    sysrepo::S_Session s_sess;
    sysrepo::S_Subscribe s_sub;
};

#endif // NET_CONF_AGENT_HPP

