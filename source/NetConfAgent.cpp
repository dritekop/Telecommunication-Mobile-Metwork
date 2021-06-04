#include <NetConfAgent.hpp>

void NetConfAgent::initSysrepo() {
    _s_conn = std::make_shared<sysrepo::Connection>();
    std::cout << _s_conn.get() << "  ";
    _s_sess = std::make_shared<sysrepo::Session>(_s_conn);
    std::cout << _s_sess.get() << "  ";
    _s_sub = std::make_shared<sysrepo::Subscribe>(_s_sess);
    std::cout << _s_sub.get() << std::endl;
}

void NetConfAgent::closeSysrepo() {
    _s_sess->session_stop();
}

//void NetConfAgent::subscribeforModelChanges(std::string& number) {
    //std::cout << "Perhaps, today will be done\n";
//}

// void NetConfAgent::registerOperData() {}

// void NetConfAgent::subscribeForRpc() {}

// void NetConfAgent::notifySysrepo() {}

// void NetConfAgent::changeData() {}

