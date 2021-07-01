inc = include_directories('../include')

gtest_dep = dependency('gtest', main : true)
gmock_dep = dependency('gmock')
sysrepo_dep = dependency('sysrepo-cpp')
libyang_dep = dependency('libyang-cpp')

e = executable('testprog', 
    sources : 'test.cpp', 
    dependencies : [gtest_dep, gmock_dep, sysrepo_dep, libyang_dep],
    link_with : mobile_client_sl,
    include_directories : inc
)

test('test', e)
