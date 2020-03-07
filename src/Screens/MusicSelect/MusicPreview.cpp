#include "MusicPreview.hpp"

#include <thread>
#include <iostream>

namespace MusicSelect {

    std::mutex openFromFile_mutex;

    MusicLoop::MusicLoop(fs::path music_path, std::optional<sf::Music::TimeSpan> t_loop) {
        music = std::make_unique<sf::Music>();
        {
            std::lock_guard<std::mutex> lock{openFromFile_mutex};
            if (not music->openFromFile(music_path.string())) {
                throw std::runtime_error("Could not load " + music_path.string());
            }
        }
        if (not t_loop.has_value()) {
            if (music->getDuration() < sf::seconds(30)) {
                loop = sf::Music::TimeSpan(sf::seconds(0.f), music->getDuration());
            } else {
                loop = sf::Music::TimeSpan(sf::seconds(15.f), sf::seconds(10.f));
            }
        }
        music->setLoopPoints(loop);
        loop = music->getLoopPoints();
        music->setLoop(true);
        music->setPlayingOffset(loop.offset);
        auto end = loop.offset + loop.length;
        fade_out = Toolkit::AffineTransform<float>{
            (end - sf::seconds(2)).asSeconds(), end.asSeconds(),
            100.f, 0.f
        };
    }

    void MusicPreview::play_async(std::optional<fs::path> music_path, std::optional<sf::Music::TimeSpan> loop) {
        if (not music_path.has_value()) {
            return;
        }
        std::optional<MusicLoop> new_loop;
        try {
            new_loop.emplace(*music_path, loop);
        } catch (const std::exception& e) {
            std::cerr << e.what() << '\n';
        }
        {
            std::lock_guard<std::mutex> lock{m_music_loop_mutex};
            if (m_music_loop) {
                m_music_loop->music->stop();
            }
            m_music_loop = std::move(new_loop);
            m_music_loop->music->play();
        }
    }

    void MusicPreview::play(std::optional<fs::path> music_path, std::optional<sf::Music::TimeSpan> loop) {
        std::thread(&MusicPreview::play_async, this, music_path, loop).detach();
    }

    void MusicPreview::update() {
        std::lock_guard<std::mutex> lock{m_music_loop_mutex};
        if (m_music_loop) {
            m_music_loop->music->setVolume(
                m_music_loop->fade_out.clampedTransform(
                    m_music_loop->music->getPlayingOffset().asSeconds()
                )
            );
        }
    }
}
