#include "SongList.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>

#include <memon.hpp>

namespace fs = ghc::filesystem;

namespace Data {

    std::optional<fs::path> Song::full_cover_path() const {
        if (cover) {
            return folder/cover.value();
        } else {
            return {};
        } 
    }

    SongList::SongList::SongList() {

        // Loading all song metadata
        for (const auto& folder : getSongFolders()) {
            try {
                songs.emplace_back(folder);
            } catch (const std::exception& e) {
                std::cerr << "Exception while parsing song folder "
                << folder.string() << " : " << e.what() << '\n';
                continue;
            }
        }
    }


    const std::vector<fs::path> getSongFolders() {

        std::vector<fs::path> potential_song_folders;
        fs::path song_folder = "songs/";

        if (fs::exists(song_folder) and fs::is_directory(song_folder)) {
            for (const auto& dir_item : fs::directory_iterator("songs/")) {
                if (dir_item.is_directory()) {
                    for (auto& path : recursiveSongSearch(dir_item.path())) {
                        potential_song_folders.push_back(path);
                    }
                }
            }
        }

        return potential_song_folders;
    }

    std::list<fs::path> recursiveSongSearch(fs::path song_or_pack) {
        std::list<fs::path> res;
        fs::directory_iterator folder{song_or_pack};
        if (
            std::any_of(
                fs::begin(folder),
                fs::end(folder),
                [](const fs::directory_entry& de) {
                    return de.path().extension() == ".memo" or
                    de.path().extension() == ".memon";
                }
            )
        ) {
            res.push_back(song_or_pack);
            return res;
        } else {
            for (auto& p : fs::directory_iterator(song_or_pack)) {
                if (p.is_directory()) {
                    res.splice(res.end(), recursiveSongSearch(p));
                }
            }
            return res;
        }
    }

    Song::Song(fs::path song_folder) :
        folder(song_folder)
    {
        
        // .memon ?
        auto memon_files = getMemonFiles(song_folder);
        if (not memon_files.empty()) {
            if (memon_files.size() > 1) {
                throw std::invalid_argument("Multiple .memon files");
            } else {
                stepland::memon m;
                std::ifstream file(memon_files.back());
                file >> m;
                this->title = m.song_title;
                this->artist = m.artist;
                if (not m.album_cover_path.empty()) {
                    this->cover.emplace(m.album_cover_path);
                }
                if (not m.music_path.empty()) {
                    this->audio.emplace(m.music_path);
                }
                for (const auto& [difficulty, chart] : m.charts) {
                    this->chart_levels[difficulty] = chart.level;
                }
            }
        } else {
            // .memo ?
            auto memo_files = getMemoFiles(song_folder);
            if (not memo_files.empty()) {
                throw std::invalid_argument("jujube does not support .memo files for now ...");
            } else {
                throw std::invalid_argument("No valid file found in song folder");
            }
        }
    }

    const std::vector<fs::path> getMemoFiles(fs::path song_folder) {
        std::vector<fs::path> res;
        for (const auto& p : fs::directory_iterator(song_folder)) {
            if (p.path().extension() == ".memo") {
                res.push_back(p.path());
            }
        }
        return res;
    }

    const std::vector<fs::path> getMemonFiles(fs::path song_folder) {
        std::vector<fs::path> res;
        for (const auto& p : fs::directory_iterator(song_folder)) {
            if (p.path().extension() == ".memon") {
                res.push_back(p.path());
            }
        }
        return res;
    }

}
