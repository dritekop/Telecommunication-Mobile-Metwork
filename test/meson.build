gtest_dep = dependency('gtest', main : true, required : false)

e = executable('testprog', 
    sources : 'test.cpp', 
    dependencies : gtest_dep
)

test('test', e)
