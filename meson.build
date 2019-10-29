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
    'src/Data/Note.hpp',
    'src/Data/Note.cpp',
    'srx/Data/Score.hpp',
    'src/Data/SongList.hpp',
    'src/Data/SongList.cpp',
    'src/Screens/Gameplay.hpp',
    'src/Screens/MusicSelect.hpp',
    'src/Screens/Result.hpp',
]

executable(
    'jujube',
    'src/Test.cpp',
    'src/Textures/CoverPack.hpp',
    'src/Textures/CoverPack.cpp',
    dependencies: [sfml, filesystem]
)