#pragma once

#include <memory>
#include <mutex>
#include <optional>

#include <ghc/filesystem.hpp>
#include <SFML/Audio.hpp>

#include "../../Toolkit/AffineTransform.hpp"

namespace fs = ghc::filesystem;

// In this file define stuff to try to handle creating sf::Music objects
// and starting their playback asynchronously while trying to keep some thread-safety
namespace MusicSelect {
    struct MusicLoop {
        MusicLoop(fs::path music_path, std::optional<sf::Music::TimeSpan> t_loop);
        std::unique_ptr<sf::Music> music;
        sf::Music::TimeSpan loop;
        Toolkit::AffineTransform<float> fade_out = {0.f, 1.f, 0.f, 1.f}; // placeholder value
    };
    class MusicPreview {
    public:
        MusicPreview() = default;
        void play(std::optional<fs::path> music_path, std::optional<sf::Music::TimeSpan> loop);
        void update();
    private:
        void play_async(std::optional<fs::path> music_path, std::optional<sf::Music::TimeSpan> loop);
        std::optional<MusicLoop> m_music_loop;
        std::mutex m_music_loop_mutex;
    };
}
