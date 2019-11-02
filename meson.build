project(
    'jujube',
    'cpp',
    default_options : ['cpp_std=c++17'],
    version : '0.1.0-alpha',
)

sfml = []
foreach module : ['system', 'window', 'graphics', 'audio'] 
    sfml += [dependency('sfml-'+module, version : '>=2.5.1')]
endforeach

cpp = meson.get_compiler('cpp')
filesystem = cpp.find_library('stdc++fs')

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
]

executable(
    'jujube',
    sources,
    dependencies: [sfml, filesystem],
    include_directories : include_directories('include')
)