#include <MobileClient.hpp>

namespace mobileclient {
    void MobileClient::setName(const std::string& name) {
        _name = name;
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

        std::string value = "idle";
        _agent = std::make_unique<netconfag::NetConfAgent>();
        _agent->initSysrepo();
        _agent->registerOperData(*this);
        _agent->changeData(_xpathState, value);
        _agent->subscribeForModelChanges(*this);

        return true;
    }

    void MobileClient::call(const std::string& incomingNumber) {
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
        std::map<std::string, std::string> mapXpathValue = { 
            {_xpathState, state},
            {_xpathIncomingNumber, incomingNumber}
        };
        _agent->fetchData(mapXpathValue);

        if (_callInitializer || mapXpathValue[_xpathState] != "active") {
            std::cout << "Forbidden action!\n";
            return;
        }

        std::string guestXpathState = "/mobile-network:core/subscribers[number='" + mapXpathValue[_xpathIncomingNumber] + "']/state";
        _agent->changeData(guestXpathState, "busy");
        _agent->changeData(_xpathState, "busy");
    }

    void MobileClient::reject() {
        std::string state;
        std::string incomingNumber;
        std::map<std::string, std::string> mapXpathValue = { 
            {_xpathState, state},
            {_xpathIncomingNumber, incomingNumber}
        };
        _agent->fetchData(mapXpathValue);

        if (mapXpathValue[_xpathState] != "active") {
            std::cout << "Forbidden action!\n";
            return;
        }

        std::string guestXpathState = "/mobile-network:core/subscribers[number='" + mapXpathValue[_xpathIncomingNumber] + "']/state";
        std::string guestXpathIncomingNumber = "/mobile-network:core/subscribers[number='" + mapXpathValue[_xpathIncomingNumber] + "']/incomingNumber";
        _agent->changeData(guestXpathState, "idle");
        _agent->changeData(_xpathState, "idle");
        _agent->changeData(guestXpathIncomingNumber, "");
        _agent->changeData(_xpathIncomingNumber, "");
    }

    void MobileClient::callEnd() {
        std::string state;
        std::string incomingNumber;
        std::map<std::string, std::string> mapXpathValue = { 
            {_xpathState, state},
            {_xpathIncomingNumber, incomingNumber}
        };
        _agent->fetchData(mapXpathValue);

        if (mapXpathValue[_xpathState] != "busy") {
            std::cout << "Forbidden action!\n";
            return;
        }

        std::string guestXpathState = "/mobile-network:core/subscribers[number='" + mapXpathValue[_xpathIncomingNumber] + "']/state";
        std::string guestXpathIncomingNumber = "/mobile-network:core/subscribers[number='" + incomingNumber + "']/incomingNumber";
        _agent->changeData(guestXpathState, "idle");
        _agent->changeData(_xpathState, "idle");
        _agent->changeData(guestXpathIncomingNumber, "");
        _agent->changeData(_xpathIncomingNumber, "");
    }

    void MobileClient::handleModuleChange() const {
        std::string valueState;
        std::map<std::string, std::string> mapXpathValue = {
            {_xpathState, valueState}
        };
        _agent->fetchData(mapXpathValue);

        // std::cout << std::boolalpha << _callInitializer << " call initializer, state - " << mapXpathValue[_xpathState] << std::endl;

        if (_callInitializer && mapXpathValue[_xpathState] == "active") {
            std::cout << "Waiting for answer...\n";
        }
        else if (!_callInitializer && mapXpathValue[_xpathState] == "active") {
            std::cout << "Incoming call. Answer or reject?\n";
        } 
        else if (mapXpathValue[_xpathState] == "busy") {
            std::cout << "Talking...\n";
        } else {
            std::cout << "Smth else\n";
        }
    }

    void MobileClient::handleOperData(std::string& xpath, std::string& operValue) const {
        operValue = _name;
        xpath = "/mobile-network:core/subscribers[number='" + _number + "']/userName";
    }

    bool MobileClient::getCallInitializer() const {
        return _callInitializer;
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
