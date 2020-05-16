test_files = []

imgui_demo = executable(
    'imgui_demo.out',
    [
        'imgui-sfml-demo.cpp',
        '../include/imgui/imgui.cpp',
        '../include/imgui/imgui_demo.cpp',
        '../include/imgui/imgui_draw.cpp',
        '../include/imgui/imgui_widgets.cpp',
        '../include/imgui-sfml/imgui-SFML.cpp'
    ],
    dependencies : dependencies,
    include_directories: inc
)

test('Able to build imgui demo', imgui_demo)

foreach test_file : test_files
    test_executable = executable(
        test_file+'.out',
        test_file,
        include_directories : inc
    )
    test(test_file+' test', test_executable)
endforeach