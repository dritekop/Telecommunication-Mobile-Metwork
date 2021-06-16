#include <NetConfAgent.hpp>
#include <MobileClient.hpp>

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

bool NetConfAgent::fetchData(std::map<std::string, std::string>& sXpathAndValue) 
{
    try {
        for (auto item : sXpathAndValue) {
            item.second = _s_sess->get_item(item.first.c_str())->val_to_string();
            sXpathAndValue[item.first] = item.second;
        }
        return true;
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        return false;
    }
}

bool NetConfAgent::subscribeForModelChanges(const mobileclient::MobileClient& refUser)
{
    try {
        auto subscribe = [&refUser] (sysrepo::S_Session session, const char* module_name, const char* xpath,\
            sr_event_t event, uint32_t request_id) {
            if (SR_EV_CHANGE == event) {
                if (refUser.getCallInitializer()) {
                    std::cout << "Waiting for answer...\n";
                    return SR_ERR_OK;
                }
                else {
                    std::cout << "Incoming call. Answer or reject?\n";
                    return SR_ERR_OK;
                }
            }
            return SR_ERR_OK;
        };
        
        _s_sub->module_change_subscribe(refUser.getModuleName().c_str(), subscribe, refUser.getXpathState().c_str());

        return true;
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        return false;
    }
}

bool NetConfAgent::registerOperData(const mobileclient::MobileClient& refUser) 
{
    try {
        auto cb = [&refUser] (sysrepo::S_Session session, const char *module_name, const char *path, const char *request_xpath,
            uint32_t request_id, libyang::S_Data_Node &parent) {
            std::string xpath;
            std::string operValue;
            refUser.handleOperData(xpath, operValue);

            libyang::S_Context ctx = session->get_context();
            libyang::S_Module mod = ctx->get_module(module_name);

            parent->new_path(ctx, xpath.c_str(), operValue.c_str(), LYD_ANYDATA_CONSTSTRING, 0);

            return SR_ERR_OK;
        };

        std::string subPath = refUser.getXpathState();
        subPath.erase(subPath.rfind("/"));
        _s_sub->oper_get_items_subscribe(refUser.getModuleName().c_str(), cb, subPath.c_str());

        return true;
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;        
        return false;
    }
}

bool NetConfAgent::subscribeForRpc(const std::string& sXpath, const size_t& amount, const std::map<std::string, std::string>& leafNameValue) 
{
    try {
        auto cbVals = [sXpath, amount, leafNameValue](sysrepo::S_Session session, const char* op_path, const sysrepo::S_Vals input,\
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
            std::map<std::string,std::string>::const_iterator it = leafNameValue.begin();
            std::pair<std::string, std::string> nameValue = *it;
            std::string name = sXpath + "/" + nameValue.first;
            out_vals->val(0)->set(name.c_str(), nameValue.second.c_str(), SR_STRING_T);

            ++it;
            nameValue = *it;
            name = sXpath + "/" + nameValue.first;
            out_vals->val(1)->set(name.c_str(), nameValue.second.c_str(), SR_ENUM_T);

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
        _s_sub->rpc_subscribe(sXpath.c_str(), cbVals, 1);

        return true;
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        return false;
    }
}

bool NetConfAgent::notifySysrepo(const std::string& sXpath, const std::map<std::string, std::string>& sLeafValue) 
{
    try {
        auto in_vals = std::make_shared<sysrepo::Vals>(sLeafValue.size());

        size_t i = 0;
        for (auto item : sLeafValue) {
            std::string setXpath = sXpath + "/" + item.first;
            in_vals->val(i)->set(setXpath.c_str(), item.second.c_str(), SR_STRING_T);
            ++i;
        }

        _s_sess->event_notif_send(sXpath.c_str(), in_vals);

        return true;
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        return false;
    }
}

bool NetConfAgent::changeData(const std::string& sXpath, const std::string& value) 
{
    try {
        _s_sess->set_item_str(sXpath.c_str(), value.c_str());
        _s_sess->apply_changes();

        return true;
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl; 
        return false;
    }
}

}; // namespace netconfag ends
