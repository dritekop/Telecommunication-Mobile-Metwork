project('mobnet', 'cpp',
    default_options : ['cpp_std=c++17'])

subdir('source')
subdir('test')

sysrepo_dep = dependency('sysrepo-cpp')
libyang_dep = dependency('libyang-cpp')
thread = dependency('threads')

executable('user_node',
    sources : 'main.cpp',
    dependencies : [mob_client_dep, sysrepo_dep, libyang_dep, thread],
)
