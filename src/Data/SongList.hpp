#pragma once

#include <ghc/filesystem.hpp>
#include <iterator>
#include <list>
#include <map>
#include <optional>
#include <string>
#include <vector>

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
        Song() = default;
        explicit Song(fs::path song_folder);
        fs::path folder;
        std::string title;
        std::string artist;
        // Path to the album cover
        std::optional<fs::path> cover;
        // Path the the audio file
        std::optional<fs::path> audio;
        // Mapping from chart difficulty (BSC, ADV, EXT ...) to the numeric level,
        std::map<std::string, unsigned int, cmp_dif_name> chart_levels;

        std::optional<fs::path> full_cover_path() const;

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
    const std::vector<fs::path> getMemoFiles(fs::path song_folder);
    const std::vector<fs::path> getMemonFiles(fs::path song_folder);
}
