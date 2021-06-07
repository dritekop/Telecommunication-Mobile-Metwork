#include <NetConfAgent.hpp>
#include <csignal>
#include <cstdlib>

#define MAX_LEN 100

volatile int exit_application = 0;

static void
sigint_handler(int signum)
{
    exit_application = 1;
}

/* Helper function for printing changes given operation, old and new value. */
static void
print_change(sysrepo::S_Change change) 
{
    if(change->oper() == SR_OP_MODIFIED) {
        if (nullptr != change->old_val() && nullptr != change->new_val()) {
           std::cout << "Change xpath: ";
           std::cout << change->new_val()->to_string();
        }
    }
}

/* Helper function for subscription */
int func_for_sub(sysrepo::S_Session session, const char* module_name, const char* xpath,\
     sr_event_t event, uint32_t request_id) 
{
    std::cout << "Waiting...\n";
    
    char change_path[MAX_LEN];
    snprintf(change_path, MAX_LEN, "/%s:*//.", module_name);
    
    auto it = session->get_changes_iter(change_path);
    
    while (auto change = session->get_change_next(it)) 
    {
        print_change(change);
    }

    return SR_ERR_OK;
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
    sub->module_change_subscribe(s_module_name.c_str(), func_for_sub);
    
    /* loop until ctrl-c is pressed / SIGINT is received */
    signal(SIGINT, sigint_handler);
    while (!exit_application) {
        
    }
}

// void NetConfAgent::registerOperData() 
// {}

// void NetConfAgent::subscribeForRpc() 
// {}

// void NetConfAgent::notifySysrepo() 
// {}

void NetConfAgent::changeData(const std::string& s_xpath, const std::string& value) 
{
    _s_sess->set_item_str(s_xpath.c_str(), value.c_str());
    _s_sess->apply_changes();
}

}; // namespace netconfag ends
