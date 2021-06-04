project('mobnet', 'cpp')

subdir('source')

sysrepo_dep = dependency('sysrepo-cpp')

executable('user_node',
    sources : 'main.cpp',
    dependencies : [net_conf_ag_dep, sysrepo_dep]
)

