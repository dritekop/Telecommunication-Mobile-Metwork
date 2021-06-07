project('mobnet', 'cpp',
    default_options : ['cpp_std=c++17'])

subdir('source')

sysrepo_dep = dependency('sysrepo-cpp')

executable('user_node',
    sources : 'main.cpp',
    dependencies : [net_conf_ag_dep, sysrepo_dep]
)

