#include "LNMarker.hpp"

#include <fstream>

namespace Resources {
    LNMarker::LNMarker(const fs::path& t_folder) :
        folder(t_folder)
    {
        if (not fs::is_directory(folder)) {
            throw std::invalid_argument(folder.string()+" is not a folder");
        }
        if (not fs::exists(folder/"marker.json")) {
            throw std::invalid_argument("LNMarker folder ( "+folder.string()+" ) has no long.json file");
        }
        std::ifstream marker_json{folder/"long.json"};
        nlohmann::json j;
        marker_json >> j;
        j.get_to(*this);
        background.load_and_check(folder, size, fps, {16, 30});
        outline.load_and_check(folder, size, fps, {16, 30});
        highlight.load_and_check(folder, size, fps, {16, 30});
        tail.load_and_check(folder, size, fps, {16, 30});
        tip_appearance.load_and_check(folder, size, fps, {16, 30});
        tip_begin_cycle.load_and_check(folder, size, fps, {8, 30});
        tip_cycle.load_and_check(folder, size, fps, {16, 30});
    }

    void from_json(const nlohmann::json& j, LNMarker& m) {
        j.at("name").get_to(m.name);
        j.at("fps").get_to(m.fps);
        j.at("size").get_to(m.size);
        j.at("note").at("background").get_to(m.background);
        j.at("note").at("outline").get_to(m.outline);
        j.at("note").at("highlight").get_to(m.highlight);
        j.at("tail").get_to(m.tail);
        j.at("tip").at("appearance").get_to(m.tip_appearance);
        j.at("tip").at("begin cycle").get_to(m.tip_begin_cycle);
        j.at("tip").at("cycle").get_to(m.tip_cycle);
    }
}
