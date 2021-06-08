#ifndef NET_CONF_AGENT_HPP
#define NET_CONF_AGENT_HPP

#include <memory>
#include <csignal>
#include <cstdlib>

#include <sysrepo-cpp/Session.hpp>
#include <sysrepo-cpp/Connection.hpp>

namespace netconfag {

namespace markers {
    extern int8_t START;
    extern int8_t SECOND_WORD;
    extern int8_t ONE_FOR_SPACE;
    extern uint8_t ONE_ARG;
    extern uint8_t TWO_ARGS;
    extern int8_t REGISTER;
    extern int8_t CALL;
    extern int8_t ANSWER;
    extern int8_t REJECT;
    extern int8_t CALLEND;
};

class NetConfAgent {
public:
    /* Initialization of the class' fields */
    void initSysrepo();

    /* ? gets the string - xpath with the leaf's value */
    std::string fetchData(const std::string&);

    /* ? prints the string xpath with the changed leaf's value */
    void subscribeForModelChanges(const std::string&);

    // void registerOperData();

    void subscribeForRpc(const std::string&);

    void notifySysrepo(const std::string&);

    /*Wrapper to use cpp-binding functions to change the leaf in the model*/
    void changeData(const std::string&, const std::string&);

private:
    sysrepo::S_Connection _s_conn;
    sysrepo::S_Session _s_sess;
};

}; // namespace netconfag ends

#endif // NET_CONF_AGENT_HPP
