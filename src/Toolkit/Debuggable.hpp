#pragma once

namespace Toolkit {
    // Classes that can show a debug interface toggled by the boolean
    class Debuggable {
    public:
        virtual ~Debuggable() = default;
        virtual void draw_debug() = 0;
        bool debug = false;
    };
}