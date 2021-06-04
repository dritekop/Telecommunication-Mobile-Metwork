#include <NetConfAgent.hpp>

void NetConfAgent::initSysrepo() {
    s_conn = std::make_shared<sysrepo::Connection>();
    std::cout << s_conn.get() << "  ";
    s_sess = std::make_shared<sysrepo::Session>(s_conn);
    std::cout << s_sess.get() << "  ";
    s_sub = std::make_shared<sysrepo::Subscribe>(s_sess);
    std::cout << s_sub.get() << std::endl;
}

void NetConfAgent::closeSysrepo() {
    s_sess->session_stop();
}

//void NetConfAgent::subscribeforModelChanges(std::string& number) {
    //std::cout << "Perhaps, today will be done\n";
//}

// void NetConfAgent::registerOperData() {}

// void NetConfAgent::subscribeForRpc() {}

// void NetConfAgent::notifySysrepo() {}

// void NetConfAgent::changeData() {}

