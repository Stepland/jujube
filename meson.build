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
    'src/Model/Chart.hpp',
    'src/Model/MusicList.hpp',
    'src/Model/MusicList.cpp',
    'src/Model/Score.hpp',
    'src/Screens/Gameplay.hpp',
    'src/Screens/MusicSelect.hpp',
    'src/Screens/Result.hpp',
    'src/Textures/TexturePack.hpp',
    'src/Textures/TexturePack.cpp',
]

executable(
    'jujube',
    sources,
    dependencies: [sfml, filesystem]
)