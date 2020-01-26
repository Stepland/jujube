project(
    'jujube',
    'cpp',
    default_options : ['cpp_std=c++20'],
    version : '0.1.0-alpha',
)

sfml = []
foreach module : ['system', 'window', 'graphics', 'audio'] 
    sfml += [dependency('sfml-'+module, version : '>=2.5.1')]
endforeach

add_project_link_arguments(['-lstdc++', '-lstdc++fs'], language : 'cpp')

sources = [
    'src/Main.cpp',
    'src/Data/Chart.hpp',
    'src/Data/KeyMapping.hpp',
    'src/Data/KeyMapping.cpp',
    'src/Data/Note.hpp',
    'src/Data/Note.cpp',
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
    'src/Resources/CoverAtlas.hpp',
    'src/Resources/CoverAtlas.cpp',
    'src/Toolkit/QuickRNG.hpp',
    'src/Toolkit/QuickRNG.cpp',
    'src/Toolkit/AffineTransform.hpp',
]

executable(
    'jujube',
    sources,
    dependencies: [sfml],
    include_directories : include_directories('include'),
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