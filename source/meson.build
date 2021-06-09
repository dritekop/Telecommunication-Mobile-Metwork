inc = include_directories('../include')

src = [
    'NetConfAgent.cpp'
]

src_with_path = []

foreach i : src
    src_with_path += './' + i
endforeach

net_conf_agent_sl = static_library(
    'agent',
    sources : [ src_with_path ],
    include_directories : [ inc ]
)

net_conf_ag_dep = declare_dependency(
    link_with : net_conf_agent_sl,
    include_directories : inc,
)
