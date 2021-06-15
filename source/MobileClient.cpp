#include <MobileClient.hpp>

namespace mobileclient {
    bool MobileClient::registerClient(const std::string& number) {
        if (!_number.empty())
            return false;
        else
            _number = number;

        _xpath_state = "/mobile-network:core/subscribers[number='" + _number + "']/state";
        std::string value = "idle";
        _agent = std::make_unique<netconfag::NetConfAgent>();
        _agent->initSysrepo();
        _agent->changeData(_xpath_state, value);
        // _agent->subscribeForModelChanges("mobile-network", _number);

        return true;
    }

    void MobileClient::call(const std::string& incoming_number) {
        std::string guest_xpath_state = "/mobile-network:core/subscribers[number='" + incoming_number + "']/state";
        
        std::string guest_value_state;
        std::string value;
        std::map<std::string, std::string> map_xpath_value = { 
            {guest_xpath_state, guest_value_state},
            {_xpath_state, value}
        };
        _agent->fetchData(map_xpath_value);

        if (map_xpath_value[guest_xpath_state] != "idle") {
            std::cout << "Subscriber can't answer. Try to call later.\n";
            return;
        }

        std::string guest_xpath_incoming_number = guest_xpath_state;
        guest_xpath_incoming_number.erase(guest_xpath_incoming_number.rfind("/"));
        guest_xpath_incoming_number += "/incomingNumber";

        std::string host_xpath_incoming_number = _xpath_state;
        host_xpath_incoming_number.erase(host_xpath_incoming_number.rfind("/"));
        host_xpath_incoming_number += "/incomingNumber";

        _agent->changeData(host_xpath_incoming_number, incoming_number);
        _agent->changeData(guest_xpath_incoming_number, _number);
        _agent->changeData(_xpath_state, "active");
        _agent->changeData(guest_xpath_state, "active");
    }

    void MobileClient::stopClient() {
        _agent->closeSys();
    }
}; // namespace mobileclient ends
