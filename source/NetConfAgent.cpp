#include <NetConfAgent.hpp>

#define START 0
#define BUSY 0
#define ACTIVE 1
#define IDLE 2

void NetConfAgent::initSysrepo() {
    _s_conn = std::make_shared<sysrepo::Connection>();
    _s_sess = std::make_shared<sysrepo::Session>(_s_conn);
    _s_sub = std::make_shared<sysrepo::Subscribe>(_s_sess);
}

void NetConfAgent::closeSysrepo() {
    _s_sess->session_stop();
}

void NetConfAgent::fetchData(const std::string& number) {
    const char* xpath = "/mobile-network:core/subscribers/number";
    const char* const_number = &number[START];
    auto value = std::make_shared<sysrepo::Val>((char *)const_number, SR_IDENTITYREF_T);
    _s_sess->set_item(xpath, value);
}

//void NetConfAgent::subscribeforModelChanges(std::string& number) {}

// void NetConfAgent::registerOperData() {}

// void NetConfAgent::subscribeForRpc() {}

// void NetConfAgent::notifySysrepo() {}

void NetConfAgent::changeData(std::string& xpath, std::string incomingNumber, uint8_t action) {

}
