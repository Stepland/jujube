#pragma once

#include <cstddef>

#include <ghc/filesystem.hpp>
#include <nlohmann/json.hpp>
#include <SFML/System.hpp>

#include "../Input/KeyMapping.hpp"

namespace Data {
    // By convention all axis-independant lengths are expressed as a ratio of the screen WIDTH
    // see panel_size and panel_spacing for example
    struct Screen {
        std::size_t width = 768;
        std::size_t height = 1360;
        bool fullscreen = false;
    };

    void to_json(nlohmann::json& j, const Screen& s);
    void from_json(const nlohmann::json& j, Screen& s);

    struct Layout {
        float panel_size = 160.f / 768.f;
        float panel_spacing = (112.f / 3.f) / 768.f;
        float panel_step() const {return panel_size+panel_spacing;};
        float ribbon_x = 8.f / 768.f;
        float ribbon_y = 602.f / 768.f;
        float big_cover_size = 300.f / 768.f;
        float big_cover_x = 0.5f;
        float big_cover_y = 0.017f;
        float big_level_x = 656.f / 768.f;
        float big_level_y = 30.f / 768.f;
        float upper_part_height = 464.f / 768.f;
    };

    void to_json(nlohmann::json& j, const Layout& l);
    void from_json(const nlohmann::json& j, Layout& l);

    struct Options {
        std::string marker;
        std::string ln_marker;
        sf::Time audio_offset;
    };

    void to_json(nlohmann::json& j, const Options& o);
    void from_json(const nlohmann::json& j, Options& o);

    // RAII style class which loads preferences from the dedicated file when constructed and saves them when destructed
    struct Preferences {
        Screen screen;
        Layout layout;
        Options options;
        Input::KeyMapping key_mapping;
        ghc::filesystem::path jujube_path;

        Preferences(const ghc::filesystem::path& t_jujube_path);
        ~Preferences();
    };

    void to_json(nlohmann::json& j, const Preferences& p);
    void from_json(const nlohmann::json& j, Preferences& p);

    struct HoldsPreferences {
        HoldsPreferences(Preferences& t_preferences) : preferences(t_preferences) {};
        float get_screen_width() const {return static_cast<float>(preferences.screen.width);};
        float get_screen_height() const {return static_cast<float>(preferences.screen.height);};
        float get_panel_size() const {return preferences.layout.panel_size*get_screen_width();};
        float get_panel_spacing() const {return preferences.layout.panel_spacing*get_screen_width();};
        float get_panel_step() const {return preferences.layout.panel_step()*get_screen_width();};
        float get_ribbon_x() const {return preferences.layout.ribbon_x*get_screen_width();};
        float get_ribbon_y() const {return preferences.layout.ribbon_y*get_screen_width();};
        float get_ribbon_size() const {return 3*get_panel_spacing()+4*get_panel_size();};
        float get_big_cover_x() const {return preferences.layout.big_cover_x*get_screen_width();};
        float get_big_cover_y() const {return preferences.layout.big_cover_y*get_screen_width();};
        float get_big_level_x() const {return preferences.layout.big_level_x*get_screen_width();};
        float get_big_level_y() const {return preferences.layout.big_level_y*get_screen_width();};
        float get_upper_part_height() const {return preferences.layout.upper_part_height*get_screen_width();};
        // Scales a length in pixels from the original jubeat resolution of 768x1360 to the current screen resolution
        float scale(float length) const {return (length*get_screen_width())/768.f;};
        Preferences& preferences;
    };
}
