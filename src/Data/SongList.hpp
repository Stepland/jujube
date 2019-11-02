#pragma once

#include <filesystem>
#include <iterator>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include "../Resources/CoverAtlas.hpp"

namespace fs = std::filesystem;

namespace Data {

    // Basic metadata about a song
    struct Song {
        Song() = default;
        explicit Song(fs::path song_folder);
        std::string title;
        std::string artist;
        // Path to the album cover
        std::optional<fs::path> cover;
        // Path the the audio file
        std::optional<fs::path> audio;
        // Mapping from chart difficulty (BSC, ADV, EXT ...) to the numeric level,
        // the level is stored multiplied by 10 and displayed divided by 10
        // to allow for decimal levels (introduced in jubeat ... festo ?)
        std::unordered_map<std::string, unsigned int> chart_levels;

        static bool sort_by_title(const Data::Song& a, const Data::Song& b) {
            return a.title < b.title;
        }
    };

    // Class holding all the necessary song data to run the Music Select screen
    class SongList {
    public:
        SongList();
        std::vector<Song> songs;
    };

    // Returns the folders conscidered to contain a valid song
    // classic memo files should have the .memo extension
    // .memon files also accepted
    const std::vector<fs::path> getSongFolders();
    std::list<fs::path> recursiveSongSearch(fs::path song_or_pack);
    const std::vector<fs::path>& getMemoFiles(fs::path song_folder);
    const std::vector<fs::path>& getMemonFiles(fs::path song_folder);
}
