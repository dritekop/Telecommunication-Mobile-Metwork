gtest_dep = dependency('gtest', required : false)

e = executable('testprog', 
    sources : 'test.cpp', 
    dependencies : [gtest_dep, mob_client_dep]
)

test('test', e)
