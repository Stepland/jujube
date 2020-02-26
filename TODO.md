# v1.0.0
## Done
### Music Select Screen
- Category Panel click
- Song Panel draw
    - async song cover loading
    - fade in
    - stretch cover to fit
    - draw song title
- Fix debug menu
- Handling Resolution changes
    - Make Panels Drawable and Transformable
    - Make Ribbon Transformable
- Compute panel size from resolution
- Compute FUCKING EVERYTHING from resolution
- Top Screen Part Handling
    - Cover
    - Title
    - Artist
    - Chart
    - Chart List
    - Density graph
        - format-agnostic chart class
- Song Panel click
    - difficulty cycle
- Fix the annoying visual glitch that flashes a song title and dif on the ribbon
- Top Screen Part Handling
    - Density graph
        - format-agnostic chart class
- Black frame
- Sound
    - Music Sample
        - Async playback

### Misc
- Handling Resolution changes
    - reload preferences on change
- Fix jujube crashing on empty layout
- Preference persistency system
- Add KeyMapping to preferences
    - Make KeyMapping look nicer in json form
    - Sort by button
- Handle JCK and special characters
    - Japanese
    - utf8 to sfml conversion
    
## TODO
### Misc
- Make Drawables lazily react to resolution changes
- Handle JCK and special characters
    - Find a font that handles more stuff
        - make a ttc bundle : https://fontforge.org/en-US/

### Music Select Screen
- Sound
    - Sound Effects
- Fullscreen handling
- Song Panel click
    - animation
    - cd
- Chart Panel
- Visible controls
- Sort menu
    - Differenciate Sort / Folder
- Options menu
    - Controller mapping
- Start Button

### Gameplay Screen

### Results Screen

### Windows and macOS builds

### memo Compatibility

### Themeing support
- Python bindings ?

# v1.1.0
## FB9 Support

## Music Select Screen
- bound memory usage of Toolkit::Cache

# v1.2.0

## P4IO Support
### TODO
- Buy a P4IO