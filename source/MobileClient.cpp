#include <MobileClient.hpp>

namespace mobileclient {
    void MobileClient::setName(const std::string& name) {
        _name = name;
    }

    void MobileClient::dryMethodOne(std::string& state, std::string& incomingNumber) const {
        std::map<std::string, std::string> mapXpathValue = { 
            {_xpathState, state},
            {_xpathIncomingNumber, incomingNumber}
        };
        _agent->fetchData(mapXpathValue);

        state = mapXpathValue[_xpathState];
        incomingNumber = mapXpathValue[_xpathIncomingNumber];
    }

    bool MobileClient::registerClient(const std::string& number) {
        if (!_number.empty())
            return false;
        else
            _number = number;

        _xpathState = "/mobile-network:core/subscribers[number='" + _number + "']/state";
        
        _xpathIncomingNumber = _xpathState;
        _xpathIncomingNumber.erase(_xpathIncomingNumber.rfind("/"));
        _xpathIncomingNumber += "/incomingNumber";

        _xpathUserName = _xpathState;
        _xpathUserName.erase(_xpathUserName.rfind("/"));
        _xpathUserName += "/userName";

        _agent = std::make_unique<netconfag::NetConfAgent>();
        _agent->initSysrepo();

        std::string state;
        std::map<std::string, std::string> testNumber = {
            {_xpathState, state}
        };
        _agent->fetchData(testNumber);
        if (!testNumber[_xpathState].empty()) {
            _number.erase();
            _xpathState.erase();
            _xpathIncomingNumber.erase();
            _xpathUserName.erase();
            return false;
        }

        std::string value = "idle";
        
        _agent->registerOperData(*this);
        _agent->changeData(_xpathState, value);
        _agent->subscribeForModelChanges(*this);

        return true;
    }

    bool MobileClient::unregisterClient() {
        std::string state;
        std::map<std::string, std::string> testNumber = {
            {_xpathState, state}
        };
        _agent->fetchData(testNumber);
        if (testNumber[_xpathState] != "idle") {
            std::cout << "Can't unregister. Try later, subscriber is " << testNumber[_xpathState] << " now.\n";
            return false;
        }

        std::string deletePath = "/mobile-network:core/subscribers[number='" + _number + "']";
        _agent->changeData(deletePath);
        _agent->closeSys();
        _agent.reset();
        _number.erase();
        _xpathState.erase();
        _xpathIncomingNumber.erase();
        _xpathUserName.erase();
        return true;
    }

    void MobileClient::call(const std::string& incomingNumber) {
        if (incomingNumber == _number) {
            std::cout << "Forbidden action!\n";
            return;
        }
        std::string guestXpathState = "/mobile-network:core/subscribers[number='" + incomingNumber + "']/state";
        
        std::string hostValueState;
        std::string guestValueState;
        std::map<std::string, std::string> mapXpathValue = { 
            {_xpathState, hostValueState},
            {guestXpathState, guestValueState},
        };
        _agent->fetchData(mapXpathValue);

        if (mapXpathValue[_xpathState] != "idle") {
            std::cout << "Forbidden action!\n";
            return;
        }

        if (mapXpathValue[guestXpathState] != "idle") {
            std::cout << "Subscriber can't answer. Try to call later.\n";
            return;
        }

        std::string guestXpathIncomingNumber = "/mobile-network:core/subscribers[number='" + incomingNumber + "']/incomingNumber";

        _callInitializer = true;
        _agent->changeData(guestXpathIncomingNumber, _number);
        _agent->changeData(_xpathIncomingNumber, incomingNumber);
        _agent->changeData(guestXpathState, "active");
        _agent->changeData(_xpathState, "active");
    }

    void MobileClient::answer() {
        std::string state;
        std::string incomingNumber;
        dryMethodOne(state, incomingNumber);

        if (_callInitializer || state != "active") {
            std::cout << "Forbidden action!\n";
            return;
        }

        std::string guestXpathState = "/mobile-network:core/subscribers[number='" + incomingNumber + "']/state";
        _agent->changeData(guestXpathState, "busy");
        _agent->changeData(_xpathState, "busy");
    }

    void MobileClient::reject() {
        std::string state;
        std::string incomingNumber;
        dryMethodOne(state, incomingNumber);

        if (state != "active") {
            std::cout << "Forbidden action!\n";
            return;
        }

        std::string guestXpathState = "/mobile-network:core/subscribers[number='" + incomingNumber + "']/state";
        std::string guestXpathIncomingNumber = "/mobile-network:core/subscribers[number='" + incomingNumber + "']/incomingNumber";
        _agent->changeData(guestXpathState, "idle");
        _agent->changeData(_xpathState, "idle");
        _agent->changeData(guestXpathIncomingNumber, "");
        _agent->changeData(_xpathIncomingNumber, "");
    }

    void MobileClient::callEnd() {
        std::string state;
        std::string incomingNumber;
        dryMethodOne(state, incomingNumber);

        if (state != "busy") {
            std::cout << "Forbidden action!\n";
            return;
        }

        std::string guestXpathState = "/mobile-network:core/subscribers[number='" + incomingNumber + "']/state";
        std::string guestXpathIncomingNumber = "/mobile-network:core/subscribers[number='" + incomingNumber + "']/incomingNumber";
        _agent->changeData(guestXpathState, "idle");
        _agent->changeData(_xpathState, "idle");
        _agent->changeData(guestXpathIncomingNumber, "");
        _agent->changeData(_xpathIncomingNumber, "");
    }

    void MobileClient::handleModuleChange() {
        std::string state;
        std::string incomingNumber;
        dryMethodOne(state, incomingNumber);

        if (_callInitializer && state == "active") {
            std::cout << "Waiting for answer...\n";
        }
        else if (!_callInitializer && state == "active") {
            std::cout << "Incoming call from " << incomingNumber << ". answer or reject?\n";
        } 
        else if (state == "busy") {
            std::cout << "Talking with " << incomingNumber << "...\n";
        } else if (state == "idle"){
            _callInitializer = false;
            std::cout << "Call ended\n";
        } else {
            std::cout << "User is unregistered\n";
        } 
    }

    void MobileClient::handleOperData(std::string& xpath, std::string& operValue) {
        operValue = _name;
        xpath = "/mobile-network:core/subscribers[number='" + _number + "']/userName";
    }

    std::string MobileClient::getXpathState() const {
        return _xpathState;
    }

    std::string MobileClient::getModuleName() const {
        return _moduleName;
    }

    void MobileClient::stopClient() {
        _agent->closeSys();
    }

}; // namespace mobileclient ends
