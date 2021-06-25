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

    /*    
    *@breaf closes sysrepo session
    *
    *@param no
    *
    *@return true if ok, otherwise false
    */
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
    *@breaf calls parameter's method that handles module's changes
    *
    *@param refUser The user-defined MobileClient reference
    *
    *@returns true if ok, otherwise false
    */
    bool subscribeForModelChanges(mobileclient::MobileClient& refUser, const std::string& moduleName);

    /*    
    *@breaf register operational data and sets value to the noconfig leaf, 
    * using the parameter's method
    *
    *@param refUser The user-defined MobileClient reference
    *
    *@return true if ok, otherwise false
    */
    bool registerOperData(mobileclient::MobileClient& refUser, const std::string& moduleName);

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
    *@breaf passed module will send notifications if it is needed
    *
    *@param s_xpath The string where are stored names of the module and of the notif-node that should send notifications
    * s_leaf_value The map where the key is the string marking the leaf's name and 
    *              the value is the string marking the value of the notification's leaf
    *
    *@return true if ok, otherwise false
    */
    bool notifySysrepo(const std::string& sXpath, const std::map<std::string, std::string>& sLeafValue);

    /*    
    *@breaf sets data for leaf of the model instance or delete if the second parameter isn't provided
    *
    *@param xpath The string, value The string with the default value ""
    *
    *@return true if ok, otherwise false
    */
    bool changeData(const std::string& sXpath, const std::string& value = std::string());

private:
    sysrepo::S_Connection _s_conn;
    sysrepo::S_Session _s_sess;
    sysrepo::S_Subscribe _s_sub;
};

}; // namespace netconfag ends

#endif // NET_CONF_AGENT_HPP
