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
    /*    
    *@breaf establishes sysrepo session
    *
    *@param no
    *
    *@return true if ok, otherwise false
    */
    bool initSysrepo();

    /* */
    bool closeSys();

    /*    
    *@breaf gets data
    *
    *@param string xpath, string where value will be written
    *
    *@return true if ok, otherwise false
    */
    bool fetchData(const std::string&, std::string&);

    /*    
    *@breaf print changes in the module
    *
    *@param module name
    *
    *@return true if ok, otherwise false
    */
    bool subscribeForModelChanges(const std::string&);

    /*    
    *@breaf register operational data and sets value to the noconfig leaf
    *
    *@param string module name, string xpath, string value 
    *
    *@return true if ok, otherwise false
    */
    bool registerOperData(const std::string&, const std::string&, const std::string&);

    /* */
    bool subscribeForRpc(const std::string&, const size_t&, const std::map<std::string, std::string>&);

    /* */
    bool notifySysrepo(const std::string&);

    /*    
    *@breaf sets data for leaf of the model instance
    *
    *@param string xpath, string value
    *
    *@return true if ok, otherwise false
    */
    bool changeData(const std::string&, const std::string&);

private:
    sysrepo::S_Connection _s_conn;
    sysrepo::S_Session _s_sess;
};

}; // namespace netconfag ends

#endif // NET_CONF_AGENT_HPP
