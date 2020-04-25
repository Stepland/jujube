#include "Preferences.hpp"

#include <iostream>
#include <fstream>

namespace Data {

    void to_json(nlohmann::json& j, const Screen& s) {
        j = nlohmann::json{
            {"width", s.width},
            {"height", s.height},
            {"fullscreen", s.fullscreen}
        };
    }

    void from_json(const nlohmann::json& j, Screen& s) {
        j.at("width").get_to(s.width);
        j.at("height").get_to(s.height);
        j.at("fullscreen").get_to(s.fullscreen);
    }
    
    
    void to_json(nlohmann::json& j, const Layout& l) {
        j = nlohmann::json{
            {"panel_size", l.panel_size},
            {"panel_spacing", l.panel_spacing},
            {"ribbon_x", l.ribbon_x},
            {"ribbon_y", l.ribbon_y},
            {"upper_part_height", l.upper_part_height}
        };
    }

    void from_json(const nlohmann::json& j, Layout& l) {
        j.at("panel_size").get_to(l.panel_size);
        j.at("panel_spacing").get_to(l.panel_spacing);
        j.at("ribbon_x").get_to(l.ribbon_x);
        j.at("ribbon_y").get_to(l.ribbon_y);
        j.at("upper_part_height").get_to(l.upper_part_height);
    }
    
    void to_json(nlohmann::json& j, const Options& o) {
        j = nlohmann::json{{"marker", o.marker}};
    }

    void from_json(const nlohmann::json& j, Options& o) {
        j.at("marker").get_to(o.marker);
    }    
    
    // RAII style class which loads preferences from the dedicated file when constructed and saves them when destructed
    Preferences::Preferences(const ghc::filesystem::path& t_jujube_path) :
        screen(),
        layout(),
        options(),
        key_mapping(),
        jujube_path(t_jujube_path)
    {
        auto path = jujube_path/"data"/"preferences.json";
        if (ghc::filesystem::exists(path)) {
            std::ifstream prefs_file;
            prefs_file.open(path);
            nlohmann::json j;
            try {
                prefs_file >> j;
                j.get_to(*this);
            } catch (const std::exception& e) {
                std::cerr << "Error while loading data/preferences.json : " << e.what() << std::endl;
                std::cerr << "Using fallback preferences instead" << std::endl;
                return;
            }
            key_mapping = KeyMapping{key_mapping.m_button_to_key};
        }
    }
        
    Preferences::~Preferences() {
        auto data_folder = jujube_path/"data";
        if (not ghc::filesystem::exists(data_folder)) {
            ghc::filesystem::create_directory(data_folder);
        }
        if (not ghc::filesystem::is_directory(data_folder)) {
            std::cerr << "Can't create data folder to save preferences, a file named 'data' exists" << std::endl;
        }
        std::ofstream preferences_file;
        preferences_file.open(data_folder/"preferences.json", std::ofstream::trunc | std::ofstream::out);
        nlohmann::json j = *this;
        preferences_file << j.dump(4);
    }

    void to_json(nlohmann::json& j, const Preferences& p) {
        j = nlohmann::json{
            {"screen", p.screen},
            {"layout", p.layout},
            {"options", p.options},
            {"key_mapping", p.key_mapping}
        };
    }

    void from_json(const nlohmann::json& j, Preferences& p) {
        j.at("screen").get_to(p.screen);
        j.at("layout").get_to(p.layout);
        j.at("options").get_to(p.options);
        j.at("key_mapping").get_to(p.key_mapping);
    }
}