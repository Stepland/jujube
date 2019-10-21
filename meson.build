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

executable(
    'jujube',
    'src/Main.cpp',
    dependencies: sfml
)