#include "MusicPreview.hpp"

#include <thread>
#include <iostream>

namespace MusicSelect {

    void MusicPreview::play_async(std::optional<fs::path> music_path, std::optional<sf::Music::TimeSpan> loop) {
        m_music_loop.emplace();
        if (not music_path.has_value()) {
            return;
        }
        if (not m_music_loop->music.openFromFile(music_path->string())) {
            std::cerr << "Could not load " << music_path->string() << std::endl;
            return;
        }
        if (not loop.has_value()) {
            if (m_music_loop->music.getDuration() < sf::seconds(30)) {
                loop.emplace(sf::seconds(0.f), m_music_loop->music.getDuration());
            } else {
                loop.emplace(sf::seconds(15.f), sf::seconds(10.f));
            }
        }
        m_music_loop->music.setLoopPoints(*loop);
        m_music_loop->loop = m_music_loop->music.getLoopPoints();
        m_music_loop->music.setLoop(true);
        m_music_loop->music.setPlayingOffset(loop->offset);
        auto end = m_music_loop->loop.offset + m_music_loop->loop.length;
        m_music_loop->fade_out = Toolkit::AffineTransform<float>{
            (end - sf::seconds(2)).asSeconds(), end.asSeconds(),
            100.f, 0.f
        };
        m_music_loop->music.play();
    }

    void MusicPreview::play(std::optional<fs::path> music_path, std::optional<sf::Music::TimeSpan> loop) {
        std::thread(&MusicPreview::play_async, this, music_path, loop).detach();
    }

    void MusicPreview::update() {
        if (m_music_loop) {
            m_music_loop->music.setVolume(
                m_music_loop->fade_out.clampedTransform(
                    m_music_loop->music.getPlayingOffset().asSeconds()
                )
            );
        }
    }
}
