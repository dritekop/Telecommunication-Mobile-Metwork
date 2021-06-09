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

bool NetConfAgent::initSysrepo() 
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    try {
        _s_conn = std::make_shared<sysrepo::Connection>();
        _s_sess = std::make_shared<sysrepo::Session>(_s_conn);
        return true;
    } catch (const std::exception& e) {
        std::cout << "Error. Try to start server.\n";
        std::cout << e.what() << std::endl;
        return false;
    }
}

bool NetConfAgent::fetchData(const std::string& s_xpath, std::string& value) 
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    try {
        value = _s_sess->get_item(s_xpath.c_str())->val_to_string();
        return true;
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        return false;
    }
}

bool NetConfAgent::subscribeForModelChanges(const std::string& s_module_name) 
{
    try {
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
        return true;

    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        return false;
    }
}

bool NetConfAgent::registerOperData(const std::string& module_name, const std::string& xpath) 
{
    try {
        auto subscribe = std::make_shared<sysrepo::Subscribe>(_s_sess);
        
        libyang::S_Context ctx = _s_sess->get_context();
        libyang::S_Module mod = ctx->get_module(xpath.c_str());
        
        auto cb = [module_name, mod] 
            (sysrepo::S_Session session, const char* module_name, 
                const char* path, const char* request_xpath,
                uint32_t request_id, libyang::S_Data_Node &parent) {
            auto subscribers = std::make_shared<libyang::Data_Node>(parent, mod, "subscribers[number='+380977777777']");
            auto name = std::make_shared<libyang::Data_Node>(subscribers, mod, "userName", "Boom");;
            
            return SR_ERR_OK; 
        };

        subscribe->oper_get_items_subscribe(module_name.c_str(), cb, xpath.c_str());

        return true;
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;        
        return false;
    }
}

bool NetConfAgent::subscribeForRpc(const std::string& s_xpath) 
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    try {
        auto sub = std::make_shared<sysrepo::Subscribe>(_s_sess);

        auto subscribe = [] (sysrepo::S_Session session, const char *op_path, const sysrepo::S_Vals input, \
            sr_event_t event, uint32_t request_id, sysrepo::S_Vals_Holder output) {
            //some work is done here    
            return SR_ERR_OK;
        };

        sub->rpc_subscribe(s_xpath.c_str(), subscribe);

        return true;
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        return false;
    }
}

bool NetConfAgent::notifySysrepo(const std::string& module_name) 
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    try {
        auto subscribe = std::make_shared<sysrepo::Subscribe>(_s_sess);
        
        auto cbVals = [] (sysrepo::S_Session session, const sr_ev_notif_type_t notif_type, const char *path,
            const sysrepo::S_Vals vals, time_t timestamp) {
            //some work is done here
        };

        subscribe->event_notif_subscribe(module_name.c_str(), cbVals);
        
        return true;
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        return false;
    }
}

bool NetConfAgent::changeData(const std::string& s_xpath, const std::string& value) 
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    try {
        _s_sess->set_item_str(s_xpath.c_str(), value.c_str());
        _s_sess->apply_changes();
        
        return true;
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl; 
        return false;
    }
}

}; // namespace netconfag ends
