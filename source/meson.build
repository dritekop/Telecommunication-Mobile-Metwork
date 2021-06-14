inc = include_directories('../include')

net_conf_agent_sl = static_library(
    'agent',
    sources :  './NetConfAgent.cpp',
    include_directories : inc
)

net_conf_ag_dep = declare_dependency(
    link_with : net_conf_agent_sl,
    include_directories : inc,
)
