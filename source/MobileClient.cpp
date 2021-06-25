#include <MobileClient.hpp>

namespace {

const std::string moduleName = "mobile-network";

std::string constructXpath(const std::string& number, const std::string& leaf) {
    std::string xpath = "/mobile-network:core/subscribers[number='" + number + "']/" + leaf;
    return xpath;
}

};

namespace mobileclient {
    MobileClient::MobileClient() :
        _agent(std::make_unique<netconfag::NetConfAgent>()),
        _callInitializer(false) 
    {}

    void MobileClient::setName(const std::string& name) 
    {
        _name = name;
    }

    bool MobileClient::registerClient(const std::string& number) 
    {
        if (!_number.empty())
            return false;
        else
            _number = number;

        _agent->initSysrepo();

        _xpathState = constructXpath(_number, "state");

        _xpathIncomingNumber = constructXpath(_number, "incomingNumber");

        _xpathUserName = constructXpath(_number, "userName");

        std::string state;
        std::map<std::string, std::string> testNumber = {
            {_xpathState, state}
        };
        _agent->fetchData(testNumber);
        if (!testNumber[_xpathState].empty()) 
        {
            std::cout << "Number is registered by other user.\n";
            _number.erase();
            _xpathState.erase();
            _xpathIncomingNumber.erase();
            _xpathUserName.erase();
            return false;
        }

        std::string value = "idle";
        
        _agent->registerOperData(*this, moduleName);
        _agent->changeData(_xpathState, value);
        _agent->subscribeForModelChanges(*this, moduleName);

        return true;
    }

    bool MobileClient::unregisterClient() 
    {
        std::string state;
        std::map<std::string, std::string> testNumber = {
            {_xpathState, state}
        };
        _agent->fetchData(testNumber);
        if (testNumber[_xpathState] != "idle") 
        {
            std::cout << "Can't unregister. Try later, subscriber is " << testNumber[_xpathState] << " now.\n";
            return false;
        }

        std::string deletePath = "/mobile-network:core/subscribers[number='" + _number + "']";
        _agent->changeData(deletePath);
        _agent->closeSys();
        _number.erase();
        _xpathState.erase();
        _xpathIncomingNumber.erase();
        _xpathUserName.erase();
        return true;
    }

    void MobileClient::call(const std::string& incomingNumber) 
    {
        if (incomingNumber == _number) 
        {
            std::cout << "Forbidden action!\n";
            return;
        }

        std::string guestXpathState = constructXpath(incomingNumber, "state");
        
        std::string hostValueState;
        std::string guestValueState;
        std::map<std::string, std::string> mapXpathValue = { 
            {_xpathState, hostValueState},
            {guestXpathState, guestValueState},
        };
        _agent->fetchData(mapXpathValue);

        if (mapXpathValue[_xpathState] != "idle") 
        {
            std::cout << "Forbidden action!\n";
            return;
        }

        if (mapXpathValue[guestXpathState] != "idle") 
        {
            std::cout << "Subscriber can't answer. Try to call later.\n";
            return;
        }

        std::string guestXpathIncomingNumber = constructXpath(incomingNumber, "incomingNumber");

        _callInitializer = true;
        _agent->changeData(guestXpathIncomingNumber, _number);
        _agent->changeData(_xpathIncomingNumber, incomingNumber);
        _agent->changeData(guestXpathState, "active");
        _agent->changeData(_xpathState, "active");
    }

    void MobileClient::answer() 
    {
        std::string state;
        std::string incomingNumber;
        std::map<std::string, std::string> mapXpathValue = { 
            {_xpathState, state},
            {_xpathIncomingNumber, incomingNumber}
        };
        _agent->fetchData(mapXpathValue);

        state = mapXpathValue[_xpathState];
        incomingNumber = mapXpathValue[_xpathIncomingNumber];

        if (_callInitializer || state != "active") 
        {
            std::cout << "Forbidden action!\n";
            return;
        }

        std::string guestXpathState = constructXpath(incomingNumber, "state");
        _agent->changeData(guestXpathState, "busy");
        _agent->changeData(_xpathState, "busy");
    }

    void MobileClient::reject() 
    {
        std::string state;
        std::string incomingNumber;
        std::map<std::string, std::string> mapXpathValue = { 
            {_xpathState, state},
            {_xpathIncomingNumber, incomingNumber}
        };
        _agent->fetchData(mapXpathValue);

        state = mapXpathValue[_xpathState];
        incomingNumber = mapXpathValue[_xpathIncomingNumber];

        if (state != "active") 
        {
            std::cout << "Forbidden action!\n";
            return;
        }

        std::string guestXpathState = constructXpath(incomingNumber, "state");
        std::string guestXpathIncomingNumber = constructXpath(incomingNumber, "incomingNumber");
        _agent->changeData(guestXpathState, "idle");
        _agent->changeData(_xpathState, "idle");
        _agent->changeData(guestXpathIncomingNumber);
        _agent->changeData(_xpathIncomingNumber);
    }

    void MobileClient::callEnd() 
    {
        std::string state;
        std::string incomingNumber;
        std::map<std::string, std::string> mapXpathValue = { 
            {_xpathState, state},
            {_xpathIncomingNumber, incomingNumber}
        };
        _agent->fetchData(mapXpathValue);

        state = mapXpathValue[_xpathState];
        incomingNumber = mapXpathValue[_xpathIncomingNumber];

        if (state != "busy") 
        {
            std::cout << "Forbidden action!\n";
            return;
        }

        std::string guestXpathState = constructXpath(incomingNumber, "state");
        std::string guestXpathIncomingNumber = constructXpath(incomingNumber, "incomingNumber");
        _agent->changeData(guestXpathState, "idle");
        _agent->changeData(_xpathState, "idle");
        _agent->changeData(guestXpathIncomingNumber);
        _agent->changeData(_xpathIncomingNumber);
    }

    void MobileClient::handleModuleChange(const std::string& change) 
    {
        if (_callInitializer && change == "active") 
        {
            std::cout << "Waiting for answer...\n";
        }
        else if (!_callInitializer && change == "active") 
        {
            std::cout << "Incoming call. answer or reject?\n";
        } 
        else if (change == "busy") 
        {
            std::cout << "Talking...\n";
        } 
        else if (change == "idle")
        {
            _callInitializer = false;
            std::cout << "Call ended\n";
        } 
        else if (change == "deleted")
        {
            std::cout << "User is unregistered\n";
        } 
    }

    void MobileClient::handleOperData(std::string& xpath, std::string& operValue) 
    {
        operValue = _name;
        xpath = _xpathUserName;
    }

    std::string MobileClient::getXpathState() const 
    {
        return _xpathState;
    }

    void MobileClient::stopClient() 
    {
        _agent->closeSys();
    }

}; // namespace mobileclient ends
