#ifndef MOBILE_CLIENT_HPP
#define MOBILE_CLIENT_HPP

#include <NetConfAgent.hpp>

namespace mobileclient {

class MobileClient {
public:
    MobileClient();

    MobileClient(const netconfag::NetConfAgent& agent);

    /*    
    *@breaf sets name to be changed in the noconfig leaf of this istance
    *
    *@param name The string with a name to be set to the subscriber
    *
    */
    void setName(const std::string& name);

    /*    
    *@breaf creates an instance for the YANG-model in the NETCONF session 
    *
    *@param number The string defining the user's unique identifier in the database
    *
    *@return true if ok, otherwise false 
    */
    bool registerClient(const std::string& number);

    /*    
    *@breaf deletes the instance created in the registerClient method 
    *
    *@return true if ok, otherwise flase
    */
    bool unregisterClient();

    /*    
    *@breaf makes respective changes in the instances of the model, that are involved in this action
    *
    *@param incomingNumber The const string reference defining the user to be called to
    *
    */
    void call(const std::string& incomingNumber);

    /*    
    *@breaf makes respective changes in the instances of the model, that take part in this call
    *
    */
    void answer();

    /*    
    *@breaf makes respective changes in the instances of the model, that take part in the call action
    *
    */
    void reject();

    /*    
    *@breaf makes respective changes in the instances of the model, that are involved in the call action
    *
    */
    void callEnd();

    /*    
    *@breaf here is concentrated a logic how program reacts on the module's changes
    *
    *@param change The string identifying what a change had happened in the module
    *
    */
    void handleModuleChange(const std::string& change);

    /*    
    *@breaf this method helps to send respective data to the netconfagent to set the noconfig leaf's value
    *
    *@param xpath The string reference with xpath to the noconfig leaf, 
            operValue The string reference with the value to be assigned to the leaf
    *
    */
    void handleOperData(std::string& xpath, std::string& operValue);

    /*    
    *@breaf getter to the private field
    *
    *@return string The value of the field xpathState
    */
    std::string getXpathState() const;

    /*    
    *@breaf closes sysrepo session
    *
    */
    bool stopClient();

private:
    std::unique_ptr<netconfag::NetConfAgent> _agent;
    bool _callInitializer;
    std::string _name;
    std::string _number;
    std::string _xpathState;
    std::string _xpathIncomingNumber;
    std::string _xpathUserName;
    enum {
        idle,
        active,
        busy
    } _state;
};

}; // namespace mobileclient ends

#endif // MOBILE_CLIENT_HPP
