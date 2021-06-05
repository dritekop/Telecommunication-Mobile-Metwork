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
    std::string s_xpath = "/mobile-network:core/subscribers[number='" + number + "']/state"; 
    const char* xpath = &s_xpath[START];
    const char* state = "idle";
    auto value = std::make_shared<sysrepo::Val>((char*)state, SR_IDENTITYREF_T);

    _s_sess->set_item(xpath, value);
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    _s_sess->apply_changes();
}

//void NetConfAgent::subscribeforModelChanges(std::string& number) {}

// void NetConfAgent::registerOperData() {}

// void NetConfAgent::subscribeForRpc() {}

// void NetConfAgent::notifySysrepo() {}

void NetConfAgent::changeData(std::string& xpath, std::string incomingNumber, uint8_t action) {

}
