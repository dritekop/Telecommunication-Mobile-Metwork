project('mobnet', 'cpp',
    default_options : ['cpp_std=c++17', '-libagent'])

subdir('source')

sysrepo_dep = dependency('sysrepo-cpp')
libyang_dep = dependency('libyang-cpp')

executable('user_node',
    sources : 'main.cpp',
    dependencies : [net_conf_ag_dep, sysrepo_dep, libyang_dep]
)

