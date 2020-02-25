project(
    'jujube',
    'cpp',
    default_options : ['cpp_std=c++17'],
    version : '0.1.0-alpha'
)

cc = meson.get_compiler('cpp')

sfml = []
foreach module : ['system', 'window', 'graphics', 'audio']
    sfml += [dependency('sfml-'+module, version : '>=2.5.1')]
endforeach

thread_dep = dependency('threads')
m_dep = cc.find_library('m', required : false)
gl_dep = dependency('gl')

add_project_link_arguments(['-lstdc++'], language : 'cpp')

sources = [
    'include/imgui/imgui.cpp',
    'include/imgui/imgui_draw.cpp',
    'include/imgui/imgui_widgets.cpp',
    'include/imgui-sfml/imgui-SFML.cpp',
    'src/Data/Buttons.hpp',
    'src/Data/Buttons.cpp',
    'src/Data/Chart.cpp',
    'src/Data/Chart.hpp',
    'src/Data/KeyMapping.hpp',
    'src/Data/KeyMapping.cpp',
    'src/Data/Note.hpp',
    'src/Data/Preferences.hpp',
    'src/Data/Score.hpp',
    'src/Data/Song.hpp',
    'src/Data/Song.cpp',
    'src/Drawables/BlackFrame.hpp',
    'src/Drawables/BlackFrame.cpp',
    'src/Resources/TextureCache.cpp',
    'src/Resources/TextureCache.hpp',
    # 'src/Resources/CoverAtlas.hpp',
    # 'src/Resources/CoverAtlas.cpp',
    # 'src/Screens/Gameplay.hpp',
    'src/Screens/MusicSelect/ButtonHighlight.hpp',
    'src/Screens/MusicSelect/ButtonHighlight.cpp',
    'src/Screens/MusicSelect/DensityGraph.hpp',
    'src/Screens/MusicSelect/DensityGraph.cpp',
    'src/Screens/MusicSelect/MusicPreview.hpp',
    'src/Screens/MusicSelect/MusicPreview.cpp',
    'src/Screens/MusicSelect/MusicSelect.hpp',
    'src/Screens/MusicSelect/MusicSelect.cpp',
    'src/Screens/MusicSelect/Panel.hpp',
    'src/Screens/MusicSelect/Panel.cpp',
    'src/Screens/MusicSelect/Ribbon.hpp',
    'src/Screens/MusicSelect/Ribbon.cpp',
    'src/Screens/MusicSelect/SharedResources.hpp',
    'src/Screens/MusicSelect/SharedResources.cpp',
    'src/Screens/MusicSelect/SongInfo.hpp',
    'src/Screens/MusicSelect/SongInfo.cpp',
    # 'src/Screens/Result.hpp',
    'src/Toolkit/AffineTransform.hpp',
    'src/Toolkit/Cache.hpp',
    'src/Toolkit/Debuggable.hpp',
    'src/Toolkit/EasingFunctions.hpp',
    'src/Toolkit/EasingFunctions.cpp',
    'src/Toolkit/HSL.hpp',
    'src/Toolkit/HSL.cpp',
    'src/Toolkit/NormalizedOrigin.hpp',
    'src/Toolkit/QuickRNG.hpp',
    'src/Toolkit/QuickRNG.cpp',
    'src/Main.cpp',
]

inc = include_directories('include', 'include/imgui', 'include/imgui-sfml')

subdir('test')

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