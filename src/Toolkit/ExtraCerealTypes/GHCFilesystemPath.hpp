#pragma once

#include <string>

#include <cereal/cereal.hpp>
#include <ghc/filesystem.hpp>

namespace ghc {
    namespace filesystem {
        template <class Archive>
        std::string save_minimal(const Archive &, const path& p) {
            return p.string();
        }

        template <class Archive>
        void load_minimal(const Archive &, path& p, const std::string& value) {
            p = path{value};
        }
    }
}