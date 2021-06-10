#ifndef NET_CONF_AGENT_HPP
#define NET_CONF_AGENT_HPP

#include <memory>
#include <csignal>
#include <cstdlib>
#include <unistd.h>

#include <sysrepo-cpp/Session.hpp>
#include <sysrepo-cpp/Connection.hpp>
// #include <libyang/Libyang.hpp>

namespace netconfag {

class NetConfAgent {
public:    
    /* Initialization of the class' fields */
    bool initSysrepo();

    /* */
    bool closeSys();

    /* ? gets the string - xpath with the leaf's value */
    bool fetchData(const std::string&, std::string&);

    /* ? prints the string xpath with the changed leaf's value */
    bool subscribeForModelChanges(const std::string&);

    /* */
    bool registerOperData(const std::string&, const std::string&, const std::string&);

    /* */
    bool subscribeForRpc(const std::string&);

    /* */
    bool notifySysrepo(const std::string&);

    /*Wrapper to use cpp-binding functions to change the leaf in the model*/
    bool changeData(const std::string&, const std::string&);

private:
    sysrepo::S_Connection _s_conn;
    sysrepo::S_Session _s_sess;
};

}; // namespace netconfag ends

#endif // NET_CONF_AGENT_HPP
