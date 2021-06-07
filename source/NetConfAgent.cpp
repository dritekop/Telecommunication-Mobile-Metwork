#include <NetConfAgent.hpp>

#define START 0
#define BUSY 0
#define ACTIVE 1
#define IDLE 2
#define REGISTER 0
#define CALL 1
#define ANSWER 2
#define REJECT 3
#define CALLEND 4

void NetConfAgent::initSysrepo() {
    _s_conn = std::make_shared<sysrepo::Connection>();
    _s_sess = std::make_shared<sysrepo::Session>(_s_conn);
    _s_sub = std::make_shared<sysrepo::Subscribe>(_s_sess);
}

void NetConfAgent::closeSysrepo() {
    _s_sess->session_stop();
}

void NetConfAgent::fetchData(const std::string& s_xpath) {
    std::cout << "THINK MAN!\n";
}

//void NetConfAgent::subscribeforModelChanges(std::string& number) {}

// void NetConfAgent::registerOperData() {}

// void NetConfAgent::subscribeForRpc() {}

// void NetConfAgent::notifySysrepo() {}

void NetConfAgent::changeData(const std::string& host_number, const std::string& guest_number,\
                    std::string host_xpath, std::string guest_xpath, uint8_t action) {
    if (action == REGISTER) {
        host_xpath += "/state"; 
        const char* xpath = &host_xpath[START];
        const char* init_state = "idle";
        sysrepo::S_Val value = std::make_shared<sysrepo::Val>((char*)init_state, SR_IDENTITYREF_T);
        _s_sess->set_item(xpath, value);
        _s_sess->apply_changes();

    } else if (action == CALL) {
        //to control if number exists
        const char* control = guest_xpath.c_str();
        _s_sess->get_item(control);

        std::string host_state_xpath = host_xpath + "/state";
        std::string guest_state_xpath = guest_xpath + "/state";
        const char* c_host_state_xpath = host_state_xpath.c_str();
        const char* c_guest_state_xpath = guest_state_xpath.c_str();
        const char* state = "active";
        _s_sess->set_item_str(c_host_state_xpath, state);      
        _s_sess->set_item_str(c_guest_state_xpath, state);

        std::string host_incoming_number_xpath = host_xpath + "/incomingNumber";
        std::string guest_incoming_number_xpath = guest_xpath + "/incomingNumber";
        const char* c_host_incoming_number_xpath = host_incoming_number_xpath.c_str();
        const char* c_guest_incoming_number_xpath = guest_incoming_number_xpath.c_str();
        const char* c_host_incoming_number = guest_number.c_str();
        const char* c_guest_incoming_number = host_number.c_str();
        _s_sess->set_item_str(c_host_incoming_number_xpath, c_host_incoming_number);      
        _s_sess->set_item_str(c_guest_incoming_number_xpath, c_guest_incoming_number);
        _s_sess->apply_changes();

    } else if (action == ANSWER) {
        host_xpath += "/state";

    }
}

