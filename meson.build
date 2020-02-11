project(
    'jujube',
    'cpp',
    default_options : ['cpp_std=c++17'],
    version : '0.1.0-alpha',
)

cc = meson.get_compiler('cpp')

sfml = []
foreach module : ['system', 'window', 'graphics', 'audio']
    sfml += [dependency('sfml-'+module, version : '>=2.5.1')]
endforeach

thread_dep = dependency('threads')
m_dep = cc.find_library('m', required : false)
gl_dep = dependency('gl')


add_project_link_arguments(['-lstdc++', '-lstdc++fs'], language : 'cpp')

sources = [
    'include/imgui/imgui.cpp',
    'include/imgui/imgui_draw.cpp',
    'include/imgui/imgui_widgets.cpp',
    'include/imgui-sfml/imgui-SFML.cpp',
    'src/Main.cpp',
    'src/Data/Chart.hpp',
    'src/Data/KeyMapping.hpp',
    'src/Data/KeyMapping.cpp',
    'src/Data/Note.hpp',
    'src/Data/Note.cpp',
    'src/Data/Preferences.hpp',
    'src/Data/Score.hpp',
    'src/Data/SongList.hpp',
    'src/Data/SongList.cpp',
    # 'src/Screens/Gameplay.hpp',
    'src/Screens/MusicSelect/ButtonHighlight.hpp',
    'src/Screens/MusicSelect/ButtonHighlight.cpp',
    'src/Screens/MusicSelect/MusicSelect.hpp',
    'src/Screens/MusicSelect/MusicSelect.cpp',
    'src/Screens/MusicSelect/Panel.hpp',
    'src/Screens/MusicSelect/Panel.cpp',
    'src/Screens/MusicSelect/Resources.hpp',
    'src/Screens/MusicSelect/Resources.cpp',
    'src/Screens/MusicSelect/Ribbon.hpp',
    'src/Screens/MusicSelect/Ribbon.cpp',
    # 'src/Screens/Result.hpp',
    'src/Resources/Autoloader.hpp',
    'src/Resources/Autoloader.cpp',
    # 'src/Resources/CoverAtlas.hpp',
    # 'src/Resources/CoverAtlas.cpp',
    'src/Toolkit/AffineTransform.hpp',
    'src/Toolkit/Debuggable.hpp',
    'src/Toolkit/EasingFunctions.hpp',
    'src/Toolkit/EasingFunctions.cpp',
    'src/Toolkit/QuickRNG.hpp',
    'src/Toolkit/QuickRNG.cpp',
]

executable(
    'jujube',
    sources,
    dependencies: [sfml, thread_dep, m_dep, gl_dep],
    include_directories : include_directories('include', 'include/imgui', 'include/imgui-sfml'), 
    cpp_args : [
        '-Wall',
        '-Wextra',
        '-Wshadow',
        '-Wnon-virtual-dtor',
        '-pedantic'
    ]
)

install_subdir('assets', install_dir : 'assets', strip_directory : true)
install_subdir('songs', install_dir: 'songs', strip_directory : true)