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

void NetConfAgent::fetchData(std::string s_xpath, int8_t opt) {
    s_xpath += "/state"; 
    const char* xpath = &s_xpath[START];
    const char* state = "idle";
    auto value = std::make_shared<sysrepo::Val>((char*)state, SR_IDENTITYREF_T);
    _s_sess->set_item(xpath, value);
    _s_sess->apply_changes();
}

//void NetConfAgent::subscribeforModelChanges(std::string& number) {}

// void NetConfAgent::registerOperData() {}

// void NetConfAgent::subscribeForRpc() {}

// void NetConfAgent::notifySysrepo() {}

void NetConfAgent::changeData(const std::string& host_xpath, const std::string& guest_xpath, uint8_t action) {
    switch (action) {
        case 1:
            const char* c_host_xpath = host_xpath.c_str();
            const char* c_guest_xpath = guest_xpath.c_str();
            const char* state = "active";
            _s_sess->set_item_str(c_guest_xpath, state);      
            _s_sess->set_item_str(c_host_xpath, state);
            _s_sess->apply_changes();
            //TO DO!
    }
}
