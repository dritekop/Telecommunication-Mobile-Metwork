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
        std::string value = "idle";
        _agent = std::make_unique<netconfag::NetConfAgent>();
        _agent->initSysrepo();
        _agent->registerOperData(_moduleName, this);
        _agent->changeData(_xpathState, value);
        // _agent->subscribeForModelChanges("mobile-network", _number);

        return true;
    }

    void MobileClient::call(const std::string& incomingNumber) {
        std::string guestXpathState = "/mobile-network:core/subscribers[number='" + incomingNumber + "']/state";
        
        std::string guestValueState;
        std::string value;
        std::map<std::string, std::string> mapXpathValue = { 
            {guestXpathState, guestValueState},
            {_xpathState, value}
        };
        _agent->fetchData(mapXpathValue);

        if (mapXpathValue[guestXpathState] != "idle") {
            std::cout << "Subscriber can't answer. Try to call later.\n";
            return;
        }

        std::string guestXpathIncomingNumber = guestXpathState;
        guestXpathIncomingNumber.erase(guestXpathIncomingNumber.rfind("/"));
        guestXpathIncomingNumber += "/incomingNumber";

        std::string hostXpathIncomingNumber = _xpathState;
        hostXpathIncomingNumber.erase(hostXpathIncomingNumber.rfind("/"));
        hostXpathIncomingNumber += "/incomingNumber";

        _agent->changeData(hostXpathIncomingNumber, incomingNumber);
        _agent->changeData(guestXpathIncomingNumber, _number);
        _agent->changeData(_xpathState, "active");
        _agent->changeData(guestXpathState, "active");
    }

    void MobileClient::handleOperData(std::string& xpath, std::string& oper_value) {
        oper_value = _name;
        xpath = "/mobile-network:core/subscribers[number='" + _number + "']/userName";
    }

    void MobileClient::stopClient() {
        _agent->closeSys();
    }
}; // namespace mobileclient ends
