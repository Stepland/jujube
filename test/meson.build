test_files = []

foreach test_file : test_files
    test_executable = executable(
        test_file+'.out',
        test_file,
        include_directories : inc
    )
    test(test_file+' test', test_executable)
endforeach