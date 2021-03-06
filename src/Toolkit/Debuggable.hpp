#pragma once

namespace Toolkit {
    // Classes that can show a debug interface toggled by the boolean
    class Debuggable {
    public:
        virtual ~Debuggable() = default;
        virtual void draw_debug() {};
        bool debug = false;
    };
}