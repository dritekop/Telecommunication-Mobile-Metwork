#include <MobileClient.hpp>

namespace mobileclient {
    bool MobileClient::registerClient(const std::string& number) {
        if (!_number.empty())
            return false;
        else
            _number = number;

        try {
            std::string xpath = "/mobile-network:core/subscribers[number='" + _number + "']/state";
            std::string value = "idle";
            _agent = std::make_unique<netconfag::NetConfAgent>();
            _agent->initSysrepo();
            _agent->changeData(xpath, value);

            return true;
        } catch (const std::exception& e) {
            std::cout << e.what() << std::endl;
            return false;
        }
    }

    void MobileClient::stopClient() {
        _agent->closeSys();
    }
}; // namespace mobileclient ends
