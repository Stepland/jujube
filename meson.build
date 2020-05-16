project(
    'jujube',
    ['cpp', 'c'],
    default_options : ['cpp_std=c++17'],
    version : '0.1.0-alpha'
)

sources = [
    'include/imgui/imgui.cpp',
    'include/imgui/imgui_draw.cpp',
    'include/imgui/imgui_widgets.cpp',
    'include/imgui-sfml/imgui-SFML.cpp',
    'include/whereami/whereami.c',
    'include/whereami/whereami++.cpp',
    'src/Data/Chart.cpp',
    'src/Data/Chart.hpp',
    'src/Data/GradedNote.cpp',
    'src/Data/GradedNote.hpp',
    'src/Data/Note.hpp',
    'src/Data/Preferences.hpp',
    'src/Data/Preferences.cpp',
    'src/Data/Score.hpp',
    'src/Data/Score.cpp',
    'src/Data/Song.hpp',
    'src/Data/Song.cpp',
    'src/Data/TimeBounds.hpp',
    'src/Drawables/BlackFrame.hpp',
    'src/Drawables/BlackFrame.cpp',
    'src/Drawables/ButtonHighlight.hpp',
    'src/Drawables/ButtonHighlight.cpp',
    'src/Drawables/DensityGraph.hpp',
    'src/Drawables/DensityGraph.cpp',
    'src/Drawables/GradedDensityGraph.hpp',
    'src/Drawables/GradedDensityGraph.cpp',
    'src/Input/Buttons.hpp',
    'src/Input/Buttons.cpp',
    'src/Input/KeyMapping.hpp',
    'src/Input/KeyMapping.cpp',
    'src/Input/Events.hpp',
    'src/Input/Events.cpp',
    'src/Resources/TextureCache.cpp',
    'src/Resources/TextureCache.hpp',
    'src/Resources/LNMarker.cpp',
    'src/Resources/LNMarker.hpp',
    'src/Resources/Marker.cpp',
    'src/Resources/Marker.hpp',
    'src/Resources/SharedResources.hpp',
    'src/Resources/SharedResources.cpp',
    'src/Resources/SpriteSheet.cpp',
    'src/Resources/SpriteSheet.hpp',
    'src/Resources/SplitSpriteSheet.cpp',
    'src/Resources/SplitSpriteSheet.hpp',
    # 'src/Resources/CoverAtlas.hpp',
    # 'src/Resources/CoverAtlas.cpp',
    'src/Screens/MusicSelect/Drawables/ControlPanels.hpp',
    'src/Screens/MusicSelect/Drawables/ControlPanels.cpp', 
    'src/Screens/MusicSelect/Options/OptionPage.hpp',
    'src/Screens/MusicSelect/Options/OptionPage.cpp',
    'src/Screens/MusicSelect/Options/InputRemap.hpp',
    'src/Screens/MusicSelect/Options/InputRemap.cpp',   
    'src/Screens/MusicSelect/Panels/MarkerPanel.hpp',
    'src/Screens/MusicSelect/Panels/MarkerPanel.cpp',
    'src/Screens/MusicSelect/Panels/Panel.hpp',
    'src/Screens/MusicSelect/Panels/Panel.cpp',     
    'src/Screens/MusicSelect/Panels/SubpagePanel.hpp',
    'src/Screens/MusicSelect/Panels/SubpagePanel.cpp',
    'src/Screens/MusicSelect/MusicPreview.hpp',
    'src/Screens/MusicSelect/MusicPreview.cpp',
    'src/Screens/MusicSelect/MusicSelect.hpp',
    'src/Screens/MusicSelect/MusicSelect.cpp',  
    'src/Screens/MusicSelect/PanelLayout.hpp',
    'src/Screens/MusicSelect/PanelLayout.cpp',
    'src/Screens/MusicSelect/Ribbon.hpp',
    'src/Screens/MusicSelect/Ribbon.cpp',
    'src/Screens/MusicSelect/Resources.hpp',
    'src/Screens/MusicSelect/Resources.cpp',
    'src/Screens/MusicSelect/SongInfo.hpp',
    'src/Screens/MusicSelect/SongInfo.cpp',
    'src/Screens/Gameplay/Drawables/Cursor.hpp',
    'src/Screens/Gameplay/Drawables/Cursor.cpp',
    'src/Screens/Gameplay/Gameplay.hpp',
    'src/Screens/Gameplay/Gameplay.cpp',
    'src/Screens/Gameplay/PreciseMusic.hpp',
    'src/Screens/Gameplay/PreciseMusic.cpp',
    'src/Screens/Gameplay/Silence.hpp',
    'src/Screens/Gameplay/Silence.cpp',
    'src/Screens/Gameplay/TimedEventsQueue.hpp',
    'src/Screens/Gameplay/TimedEventsQueue.cpp',
    # 'src/Screens/Result.hpp',
    'src/Toolkit/AffineTransform.hpp',
    'src/Toolkit/Cache.hpp',
    'src/Toolkit/Debuggable.hpp',
    'src/Toolkit/DurationInFrames.hpp',
    'src/Toolkit/EasingFunctions.hpp',
    'src/Toolkit/EasingFunctions.cpp',
    'src/Toolkit/GHCFilesystemPathHash.hpp',
    'src/Toolkit/HSL.hpp',
    'src/Toolkit/HSL.cpp',
    'src/Toolkit/SFMLHelpers.hpp',
    'src/Toolkit/SFMLHelpers.cpp',
    'src/Toolkit/QuickRNG.hpp',
    'src/Toolkit/QuickRNG.cpp',
    'src/Main.cpp',
]

cc = meson.get_compiler('cpp')

dependencies = []

foreach module : ['system', 'window', 'graphics', 'audio']
    dependencies += [dependency('sfml-'+module, version : '>=2.5.1')]
endforeach

dependencies += dependency('threads')
dependencies += cc.find_library('m', required : false)
dependencies += dependency('gl')
dependencies += cc.find_library('atomic')
dependencies += cc.find_library('X11')

add_project_link_arguments(['-lstdc++'], language : 'cpp')

inc = include_directories('include', 'include/imgui', 'include/imgui-sfml')

subdir('test')

executable(
    'jujube',
    sources,
    dependencies: dependencies,
    include_directories : inc, 
    cpp_args : [
        '-Wall',
        '-Wextra',
        '-Wshadow',
        '-Wnon-virtual-dtor',
        '-pedantic'
    ]
)