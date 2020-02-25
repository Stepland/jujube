#pragma once

#include <iterator>
#include <list>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <variant>
#include <unordered_map>

#include <ghc/filesystem.hpp>
#include <SFML/Audio.hpp>

#include "Chart.hpp"

namespace fs = ghc::filesystem;

namespace Data {

    /*
    * Difficulty name ordering : BSC > ADV > EXT > anything else in lexicographical order
    */
    struct cmp_dif_name {
        std::map<std::string,int> dif_names = {{"BSC",1},{"ADV",2},{"EXT",3}};
        bool operator()(const std::string& a, const std::string& b) const;
    };

    // Basic metadata about a song
    struct Song {
        fs::path folder;
        std::string title;
        std::string artist;
        // Path to the album cover
        std::optional<fs::path> cover;
        // Path the the audio file
        std::optional<fs::path> audio;
        std::optional<sf::Music::TimeSpan> preview;
        // Mapping from chart difficulty (BSC, ADV, EXT ...) to the numeric level,
        std::map<std::string, unsigned int, cmp_dif_name> chart_levels;

        std::optional<fs::path> full_cover_path() const;
        std::optional<fs::path> full_audio_path() const;

        virtual std::optional<Chart> get_chart(const std::string& difficulty) const = 0;

        static bool sort_by_title(const Data::Song& a, const Data::Song& b) {
            return a.title < b.title;
        }
        virtual ~Song() = default;
    };

    struct MemonSong : public Song {
        explicit MemonSong(const fs::path& memon_path);
        std::optional<Chart> get_chart(const std::string& difficulty) const;
    private:
        fs::path memon_path;
    };

    /*
    struct MemoSong : public Song {
        explicit MemoSong(const std::unordered_map<std::string, fs::path>& memo_paths);
    private:
        std::unordered_map<std::string, fs::path> memo_paths;
    };
    */

    // Class holding all the necessary song data to run the Music Select screen
    class SongList {
    public:
        SongList();
        std::list<std::shared_ptr<Song>> songs;
    };

    // Returns the folders conscidered to contain a valid song
    // classic memo files should have the .memo extension
    std::list<std::shared_ptr<Song>> recursiveSongSearch(fs::path song_or_pack);
}
