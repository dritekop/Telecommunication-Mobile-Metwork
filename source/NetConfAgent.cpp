#include <NetConfAgent.hpp>

#define MAX_LEN 100

volatile int exit_application = 0;

static void
sigint_handler(int signum)
{
    exit_application = 1;
}

namespace netconfag {

namespace markers {
    int8_t START = 0;
    int8_t SECOND_WORD = 1;
    int8_t ONE_FOR_SPACE = 1;
    uint8_t ONE_ARG = 1;
    uint8_t TWO_ARGS = 2;
    int8_t REGISTER = 0;
    int8_t CALL = 1;
    int8_t ANSWER = 2;
    int8_t REJECT = 3;
    int8_t CALLEND = 4;
};

void NetConfAgent::initSysrepo() 
{
    _s_conn = std::make_shared<sysrepo::Connection>();
    _s_sess = std::make_shared<sysrepo::Session>(_s_conn);
}

std::string NetConfAgent::fetchData(const std::string& s_xpath) 
{
    return _s_sess->get_item(s_xpath.c_str())->to_string();
}

void NetConfAgent::subscribeForModelChanges(const std::string& s_module_name) 
{
    auto sub = std::make_shared<sysrepo::Subscribe>(_s_sess);

    auto subscribe = [] (sysrepo::S_Session session, const char* module_name, const char* xpath,\
        sr_event_t event, uint32_t request_id) {
        //some work is done here
        char change_path[MAX_LEN];
        snprintf(change_path, MAX_LEN, "/%s:*//.", module_name);
        auto it = session->get_changes_iter(change_path);
        while (auto change = session->get_change_next(it))
            std::cout << change->new_val()->to_string();
        return SR_ERR_OK;
    };

    sub->module_change_subscribe(s_module_name.c_str(), subscribe);
    
    /* loop until ctrl-c is pressed / SIGINT is received */
    signal(SIGINT, sigint_handler);
    while (!exit_application) {
        
    }
}

// void NetConfAgent::registerOperData() 
// {}

void NetConfAgent::subscribeForRpc(const std::string& s_xpath) 
{
    auto sub = std::make_shared<sysrepo::Subscribe>(_s_sess);
    std::cout << __PRETTY_FUNCTION__ << std::endl;

    auto subscribe = [] (sysrepo::S_Session session, const char *op_path, const sysrepo::S_Vals input, \
        sr_event_t event, uint32_t request_id, sysrepo::S_Vals_Holder output) {
        //some work is done here    
        return SR_ERR_OK;
    };

    sub->rpc_subscribe(s_xpath.c_str(), subscribe);
}

void NetConfAgent::notifySysrepo(const std::string& module_name) 
{
    auto subscribe = std::make_shared<sysrepo::Subscribe>(_s_sess);
    
    auto cbVals = [] (sysrepo::S_Session session, const sr_ev_notif_type_t notif_type, const char *path,
        const sysrepo::S_Vals vals, time_t timestamp) {
        //some work is done here
    };

    subscribe->event_notif_subscribe(module_name.c_str(), cbVals);
}

void NetConfAgent::changeData(const std::string& s_xpath, const std::string& value) 
{
    _s_sess->set_item_str(s_xpath.c_str(), value.c_str());
    _s_sess->apply_changes();
}

}; // namespace netconfag ends
