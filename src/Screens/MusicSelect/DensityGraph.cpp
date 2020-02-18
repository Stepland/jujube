#include "DensityGraph.hpp"

#include <algorithm>
#include <functional>

#include <SFML/Audio.hpp>

namespace Data {

    DensityGraph compute_density_graph_1(const SongDifficulty& sd) {
        return compute_density_graph_2(sd.song, sd.difficulty);
    }

    DensityGraph compute_density_graph_2(const Song& song, const std::string& difficulty) {
        auto c = song.get_chart(difficulty);
        if (not c.has_value()) {
            throw std::invalid_argument("Song "+song.title+" has no "+difficulty+" chart");
        }
        if (not song.audio.has_value()) {
            return compute_density_graph_3(*c, c->notes.begin()->timing, c->notes.rbegin()->timing);
        }
        sf::Music m;
        if (not m.openFromFile(*song.full_audio_path())) {
            return compute_density_graph_3(*c, c->notes.begin()->timing, c->notes.rbegin()->timing);
        }
        return compute_density_graph_3(
            *c,
            std::min(0L, c->notes.begin()->timing),
            std::max(c->notes.rbegin()->timing, static_cast<long>(m.getDuration().asMilliseconds()*3/10))
        );
    }

    DensityGraph compute_density_graph_3(const Chart& chart, long start, long end) {
        DensityGraph d{};
        if (start != end) {
            for (auto &&note : chart.notes) {
                auto index = (note.timing-start)*115/(end-start);
                d.at(index) += 1;
            }
            std::replace_if(
                d.begin(),
                d.end(),
                std::bind(std::greater<unsigned int>(), std::placeholders::_1, 8),
                8
            );
        }
        return d;
    }
}
