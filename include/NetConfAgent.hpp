#ifndef NET_CONF_AGENT_HPP
#define NET_CONF_AGENT_HPP

#include <memory>
#include <csignal>
#include <cstdlib>
#include <unistd.h>

#include <sysrepo-cpp/Session.hpp>
#include <sysrepo-cpp/Connection.hpp>

namespace mobileclient {
    class MobileClient;
};

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
    bool fetchData(std::map<std::string, std::string>& sXpathAndValue);

    /*    
    *@breaf print changes in the module
    *
    *@param s_xpath The string shows module where changes should be monitored
    * number The string to complete s_xpath correctly
    *
    *@returns true if ok, otherwise false
    */
    bool subscribeForModelChanges(const mobileclient::MobileClient& refUser);

    /*    
    *@breaf register operational data and sets value to the noconfig leaf
    *
    *@param module_name The string names module where is located noconfig leaf, 
    * ptrUser The pointer to the user-defined class in order to call its method  
    *
    *@return true if ok, otherwise false
    */
    bool registerOperData(const mobileclient::MobileClient& refUser);

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
    bool subscribeForRpc(const std::string& sXpath, const size_t& amount, const std::map<std::string, std::string>& leafNameValue);

    /*
    *@breaf passed module will send nitofications if it is needed
    *
    *@param s_xpath The string where are stored names of the module and of the notif-node that should send notifications
    * s_leaf_value The map where the key is the string marking the leaf's name and 
    *              the value is the string marking the value of the notification's leaf
    *
    *@return true if ok, otherwise false
    */
    bool notifySysrepo(const std::string& sXpath, const std::map<std::string, std::string>& sLeafValue);

    /*    
    *@breaf sets data for leaf of the model instance
    *
    *@param xpath The string, value The string
    *
    *@return true if ok, otherwise false
    */
    bool changeData(const std::string& sXpath, const std::string& value);

private:
    sysrepo::S_Connection _s_conn;
    sysrepo::S_Session _s_sess;
    sysrepo::S_Subscribe _s_sub;
};

}; // namespace netconfag ends

#endif // NET_CONF_AGENT_HPP
