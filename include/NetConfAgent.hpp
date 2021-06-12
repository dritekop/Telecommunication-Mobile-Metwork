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
    *@param s_xpath and value The map where key is a string xpath and 
    *mapped type is a string where will be stored value of the element on this xpath
    *
    *@returns true if ok, otherwise false
    */
    bool fetchData(std::map<std::string, std::string>& s_xpath_and_value);

    /*    
    *@breaf print changes in the module
    *
    *@param module_name The string names the module to which be subscribed
    *
    *@returns true if ok, otherwise false
    */
    bool subscribeForModelChanges(const std::string& s_module_name);

    /*    
    *@breaf register operational data and sets value to the noconfig leaf
    *
    *@param module_name The string names module where is located noconfig leaf, 
    * xpath The string where is stored xpath to this leaf, 
    * oper_value The string where is stored data to be assigned to the operational data 
    *
    *@return true if ok, otherwise false
    */
    bool registerOperData(const std::string& module_name, const std::string& xpath, const std::string& oper_value);

    /*    
    *@breaf indicates sent rpc
    *
    *@param s_xpath The string where is stored xpath to the rpc block in the yang-model
    * amount The size of output leaves in this rpc block
    * leaf_name_value The map where the key is the leaf name in the output block inside the  rpc block
    * and mapped type is the string where is stored value to be set to that leaf  
    *
    *@returns true if ok, otherwise false
    */
    bool subscribeForRpc(const std::string& s_xpath, const size_t& amount, const std::map<std::string, std::string>& leaf_name_value);

    /*
    *@breaf passed module will send nitofications if it is needed
    *
    *@param module_name The string where is stored name of the module that should send notifications
    *
    *@return true if ok, otherwise false
    */
    bool notifySysrepo(const std::string& module_name);

    /*    
    *@breaf sets data for leaf of the model instance
    *
    *@param xpath The string, value The string
    *
    *@return true if ok, otherwise false
    */
    bool changeData(const std::string& s_xpath, const std::string& value);

private:
    sysrepo::S_Connection _s_conn;
    sysrepo::S_Session _s_sess;
    sysrepo::S_Subscribe _s_sub;
};

}; // namespace netconfag ends

#endif // NET_CONF_AGENT_HPP
