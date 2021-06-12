#include <NetConfAgent.hpp>

#define MAX_LEN 100

namespace netconfag {

bool NetConfAgent::initSysrepo() 
{
    try {
        _s_conn = std::make_shared<sysrepo::Connection>();
        _s_sess = std::make_shared<sysrepo::Session>(_s_conn);
        _s_sub = std::make_shared<sysrepo::Subscribe>(_s_sess);
        return true;
    } catch (const std::exception& e) {
        std::cout << "Error. Try to start server.\n";
        std::cout << e.what() << std::endl;
        return false;
    }
}

bool NetConfAgent::closeSys() {
    try {
        _s_sess->session_stop();
        return true;
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        return false;
    }
}

bool NetConfAgent::fetchData(std::map<std::string, std::string>& s_xpath_and_value) 
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    try {
        for (auto item : s_xpath_and_value) {
            item.second = _s_sess->get_item(item.first.c_str())->val_to_string();
        }
        return true;
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        return false;
    }
}

bool NetConfAgent::subscribeForModelChanges(const std::string& s_module_name) 
{
    try {
        auto subscribe = [] (sysrepo::S_Session session, const char* module_name, const char* xpath,\
            sr_event_t event, uint32_t request_id) {
            
            char change_path[MAX_LEN];
            snprintf(change_path, MAX_LEN, "/%s:*//.", module_name);
            auto it = session->get_changes_iter(change_path);
            auto change = session->get_change_next(it);
            std::cout << change->new_val()->to_string();
            return SR_ERR_OK;
        };
        
        _s_sub->module_change_subscribe(s_module_name.c_str(), subscribe);

        return true;
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        return false;
    }
}

bool NetConfAgent::registerOperData(const std::string& module_name, const std::string& xpath, const std::string& oper_value) 
{
    try {
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

        _s_sub->oper_get_items_subscribe(module_name.c_str(), cb, sub_path.c_str());

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
        _s_sub->rpc_subscribe(s_xpath.c_str(), cbVals, 1);

        return true;
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        return false;
    }
}

bool NetConfAgent::notifySysrepo(const std::string& module_name) 
{
    try {
        auto cbVals = [] (sysrepo::S_Session session, const sr_ev_notif_type_t notif_type, const char *path,
            const sysrepo::S_Vals vals, time_t timestamp) {
            std::cout << "\n ========== NOTIF RECEIVED ==========\n" << std::endl;

            for(size_t n = 0; n < vals->val_cnt(); ++n) {
                auto value = vals->val(n);
                std::cout << value->xpath();
                std::cout << " = " << value->data()->get_string() << std::endl;
            }
        };

        std::cout << "\n ========== SUBSCRIBE TO NOTIF ==========\n" << std::endl;
        _s_sub->event_notif_subscribe(module_name.c_str(), cbVals);

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
