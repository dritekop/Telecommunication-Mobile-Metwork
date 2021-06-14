inc = include_directories('../include')

net_conf_agent_sl = static_library(
    'agent',
    sources :  './NetConfAgent.cpp',
    include_directories : inc
)

mobile_client_sl = static_library(
    'mobclient',
    sources : './MobileClient.cpp',
    include_directories : inc,
    link_with : net_conf_agent_sl
)

mob_client_dep = declare_dependency(
    link_with : mobile_client_sl,
    include_directories : inc,
)
