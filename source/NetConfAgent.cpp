#include <NetConfAgent.hpp>

#define MAX_LEN 100

volatile int exit_application = 0;

static void
sigint_handler(int signum)
{
    exit_application = 1;
}

namespace netconfag {

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

bool NetConfAgent::closeSys() {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    try {
        _s_sess->session_stop();
        return true;
    } catch (const std::exception& e) {
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
            // while (auto change = session->get_change_next(it))
            auto change = session->get_change_next(it);
                std::cout << change->new_val()->to_string();
            return SR_ERR_OK;
        };

        sub->module_change_subscribe(s_module_name.c_str(), subscribe);
        
        /* loop until ctrl-c is pressed / SIGINT is received 
        */
        signal(SIGINT, sigint_handler);
        while (!exit_application) {

        }
        return true;

    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        return false;
    }
}

bool NetConfAgent::registerOperData(const std::string& module_name, const std::string& xpath, const std::string& oper_value) 
{
    try {
        auto subscribe = std::make_shared<sysrepo::Subscribe>(_s_sess);
        std::string sub_path = xpath;
        sub_path.erase(sub_path.rfind("/"));
        std::cout << sub_path << std::endl;
        
        auto cb = [xpath, oper_value] (sysrepo::S_Session session, const char *module_name, const char *path, const char *request_xpath,
            uint32_t request_id, libyang::S_Data_Node &parent) {
            libyang::S_Context ctx = session->get_context();
            libyang::S_Module mod = ctx->get_module(module_name);

            parent->new_path(ctx, xpath.c_str(), oper_value.c_str(), LYD_ANYDATA_CONSTSTRING, 0);
            std::cout << __PRETTY_FUNCTION__ << std::endl;

            return SR_ERR_OK;
        };

        subscribe->oper_get_items_subscribe(module_name.c_str(), cb, sub_path.c_str());
        std::cout << __PRETTY_FUNCTION__ << std::endl;

        /* loop until ctrl-c is pressed / SIGINT is received 
        */
        signal(SIGINT, sigint_handler);
        while (!exit_application) {

        }
        return true;
    } catch (const std::exception& e) {
        std::cout << e.what() << " WHATAFUCK!!!!" << std::endl;        
        return false;
    }
}

bool NetConfAgent::subscribeForRpc(const std::string& s_xpath, const size_t& amount, const std::map<std::string, std::string>& leaf_name_value) 
{
    std::cout << std::endl << __PRETTY_FUNCTION__ << std::endl;
    try {
        auto subscribe = std::make_shared<sysrepo::Subscribe>(_s_sess);
        auto cbVals = [s_xpath, amount, leaf_name_value](sysrepo::S_Session session, const char* op_path, const sysrepo::S_Vals input,\
            sr_event_t event, uint32_t request_id, sysrepo::S_Vals_Holder output) {
            std::cout << "\n ========== RPC CALLED ==========\n" << std::endl;

            auto out_vals = output->allocate(amount);

            for(size_t n = 0; n < input->val_cnt(); ++n) {
                auto value = input->val(n);
                std::cout << value->xpath();
                std::cout << " = " << value->data()->get_string() << std::endl;    
            }

            /* 
            the structure of the code below matches to the yang model MOBILENETWORK.
            */
            std::map<std::string,std::string>::const_iterator it = leaf_name_value.begin();
            std::pair<std::string, std::string> name_value = *it;
            std::string name = s_xpath + "/" + name_value.first;
            std::cout << name << std::endl;
            out_vals->val(0)->set(name.c_str(), name_value.second.c_str(), SR_STRING_T);

            ++it;
            name_value = *it;
            name = s_xpath + "/" + name_value.first;
            std::cout << name << std::endl;
            out_vals->val(1)->set(name.c_str(), name_value.second.c_str(), SR_ENUM_T);

            std::cout << "\n ========== PRINT RETURN VALUE ==========\n" << std::endl;
            auto value = out_vals->val(0);
            std::cout << value->xpath();
            std::cout << " = " << value->data()->get_string() << std::endl;
            value = out_vals->val(1);
            std::cout << value->xpath();
            std::cout << " = " << value->data()->get_enum() << std::endl;
            
            return SR_ERR_OK;
        };

        std::cout << "\n ========== SUBSCRIBE TO RPC CALL ==========\n" << std::endl;
        subscribe->rpc_subscribe(s_xpath.c_str(), cbVals, 1);

        auto in_vals = std::make_shared<sysrepo::Vals>(1);

        in_vals->val(0)->set("/MOBILENETWORK:something/what_does_this_do", "AND", SR_ENUM_T);       

        std::cout << "\n ========== START RPC CALL FROM THE METHOD. SHOULD BE CHANGED ==========\n" << std::endl;
        auto out_vals = _s_sess->rpc_send("/MOBILENETWORK:something", in_vals);

        return true;
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        return false;
    }
}

bool NetConfAgent::notifySysrepo(const std::string& module_name) 
{
    try {
        
        
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
