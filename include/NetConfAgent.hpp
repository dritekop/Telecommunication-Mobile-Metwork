#ifndef NET_CONF_AGENT_HPP
#define NET_CONF_AGENT_HPP

#include <memory>

#include <sysrepo-cpp/Session.hpp>
#include <sysrepo-cpp/Connection.hpp>

class NetConfAgent {
public:
    /*Initialization of the class' fields*/
    void initSysrepo();

    /*Takes a value of the leaf with its xpath from the model as a string value*/
    std::string fetchData(const std::string&);

    /**/
    void subscribeForModelChanges(const std::string&);

    // void registerOperData();

    // void subscribeForRpc();

    // void notifySysrepo();

    /*Wrapper to use cpp-binding functions to change the leaf in the model*/
    void changeData(const std::string&, const std::string&);

private:
    sysrepo::S_Connection _s_conn;
    sysrepo::S_Session _s_sess;
};

#endif // NET_CONF_AGENT_HPP
