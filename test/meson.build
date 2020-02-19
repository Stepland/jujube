test_files = [
    'cereal_specialize_enums.cpp',
    'cereal_unordered_map.cpp'
]

foreach test_file : test_files
    test_executable = executable(
        test_file+'.out',
        test_file,
        include_directories : inc
    )
    test(test_file+' test', test_executable)
endforeach