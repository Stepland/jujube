#pragma once

#include <cstddef>

#include <SFML/System.hpp>

namespace Data {

    struct Screen {
        std::size_t width;
        std::size_t height;
        bool fullscreen;

        template<class Archive>
        void serialize(Archive & archive) {
            archive(
                CEREAL_NVP(width),
                CEREAL_NVP(height),
                CEREAL_NVP(fullscreen) 
            ); 
        }
    };

    struct Layout {
        float panel_position_x;
        float panel_position_y;
        float panel_size;
        float panel_spacing;

        template<class Archive>
        void serialize(Archive & archive) {
            archive(
                CEREAL_NVP(panel_position_x),
                CEREAL_NVP(panel_position_y),
                CEREAL_NVP(panel_size),
                CEREAL_NVP(panel_spacing) 
            ); 
        }
    };

    // RAII style class which saves preferences when destructed
    struct Preferences {
        Screen screen;
        Layout layout;

        template<class Archive>
        void serialize(Archive & archive) {
            archive(
                CEREAL_NVP(version),
                CEREAL_NVP(screen),
                CEREAL_NVP(layout) 
            ); 
        }
    };
}
