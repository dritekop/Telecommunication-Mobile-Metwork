#ifndef MOBILE_CLIENT_HPP
#define MOBILE_CLIENT_HPP

#include <NetConfAgent.hpp>

namespace mobileclient {

class MobileClient {
public:
    MobileClient();

    /*    
    *@breaf sets name to be changed in the noconfig leaf of this istance
    *
    *@param name The string
    *
    *@return void
    */
    void setName(const std::string& name);

    /*    
    *@breaf creates an instance for the YANG-model in the NETCONF session 
    *
    *@param number The string
    *
    *@return true if ok, otherwise false 
    */
    bool registerClient(const std::string& number);

    /*    
    *@breaf deletes the instance created in the registerClient method 
    *
    *@param no
    *
    *@return true if ok, otherwise flase
    */
    bool unregisterClient();

    /*    
    *@breaf makes respective changes in the instances
    *
    *@param incomingNumber The const string reference
    *
    *@return void
    */
    void call(const std::string& incomingNumber);

    /*    
    *@breaf makes respective changes in the instances
    *
    *@param no
    *
    *@return void
    */
    void answer();

    /*    
    *@breaf makes respective changes in the instances
    *
    *@param no
    *
    *@return void
    */
    void reject();

    /*    
    *@breaf makes respective changes in the instances
    *
    *@param no
    *
    *@return void
    */
    void callEnd();

    /*    
    *@breaf here is concentrated a logic how program reacts on the module's changes
    *
    *@param change The string
    *
    *@return void
    */
    void handleModuleChange(const std::string& change);

    /*    
    *@breaf this method helps to send respective data to the netconfagent to set the noconfig leaf's value
    *
    *@param xpath The string reference, operValue The string reference
    *
    *@return void
    */
    void handleOperData(std::string& xpath, std::string& operValue);

    /*    
    *@breaf getter
    *
    *@param xpath no
    *
    *@return string The value of the field xpathState
    */
    std::string getXpathState() const;

    /*    
    *@breaf reset NetConfAgent pointer and erases all fields
    *
    *@param xpath no
    *
    *@return void
    */
    void stopClient();

private:
    std::unique_ptr<netconfag::NetConfAgent> _agent;
    bool _callInitializer;
    std::string _name;
    std::string _number;
    std::string _xpathState;
    std::string _xpathIncomingNumber;
    std::string _xpathUserName;
};

}; // namespace mobileclient ends

#endif // MOBILE_CLIENT_HPP
