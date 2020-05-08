#include "LNMarker.hpp"

#include <fstream>
#include <iostream>

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

    LNMarkers::LNMarkers(const fs::path& jujube_path) {
        load_from_folder(jujube_path/"markers"/"long");
        load_from_folder(jujube_path/"assets"/"markers"/"long");
        if (empty()) {
            throw std::runtime_error("No long note markers found, jujube needs at least one to operate");
        }
    }
    void LNMarkers::load_from_folder(const fs::path& lnmarkers_folder) {
        if (fs::exists(lnmarkers_folder)) {
            for (auto& p : fs::directory_iterator(lnmarkers_folder)) {
                if (p.is_directory()) {
                    try {
                        LNMarker m{p.path()};
                        emplace(m.name, m);
                    } catch (const std::exception& e) {
                        std::cerr << "Unable to load marker folder "
                        << p.path().filename().string() << " : "
                        << e.what() << '\n';
                    }
                }
            }
        }
    }
}
