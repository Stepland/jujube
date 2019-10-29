#include "SongList.hpp"

#include <algorithm>
#include <iostream>
#include <list>

namespace fs = std::filesystem;

Data::SongList::SongList::SongList() {

    // Loading all song metadata
    for (const auto& folder : getSongFolders()) {
        try {
            songs.emplace_back(folder);
        } catch (const std::invalid_argument& e) {
            std::cerr << "Exception while parsing song folder "
            << folder.string() << " : " << e.what() << '\n';
            continue;
        }
    }

    // Loading all cover previews
    for (const auto& song : songs) {
        if (song.cover) {
            try {
                cover_previews.emplace_back(*song.cover);
            } catch (const std::invalid_argument& e) {
                std::cerr << "Exception while loading song cover "
                << song.cover->string() << " : " << e.what() << '\n';
            }
        }
    }
    
}


const std::vector<fs::path> Data::getSongFolders() {

    std::vector<fs::path> song_folders;

    for (const auto& dir_item : fs::directory_iterator("songs/")) {
        if (dir_item.is_directory()) {
            for (auto& path : recursiveSongSearch(dir_item.path())) {
                song_folders.push_back(path);
            }
        }
    }

    return song_folders;
}

std::list<fs::path> Data::recursiveSongSearch(fs::path song_or_pack) {
    std::list<fs::path> res;
    fs::directory_iterator folder{song_or_pack};
    if (
        std::any_of(
            fs::begin(folder),
            fs::end(folder),
            [](const fs::directory_entry& de) {
                de.path().extension() == ".memo" or
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

Data::Song::Song(fs::path song_folder) {
    
    // .memon ?
    auto memon_files = getMemonFiles(song_folder);
    if (not memon_files.empty()) {
        if (memon_files.size() > 1) {
            throw std::invalid_argument("Multiple .memon files");
        } else {
            
        }
    } else {
        // .memo ?
        auto memo_files = getMemoFiles(song_folder);
        if (not memo_files.empty()) {

        } else {
            throw std::invalid_argument("No valid file found in song folder");
        }
    }
}

const std::vector<fs::path>& Data::getMemoFiles(fs::path song_folder) {
    std::vector<fs::path> res;
    for (const auto& p : fs::directory_iterator(song_folder)) {
        if (p.path().extension() == ".memo") {
            res.push_back(p.path());
        }
    }
    return res;
}

const std::vector<fs::path>& Data::getMemonFiles(fs::path song_folder) {
    std::vector<fs::path> res;
    for (const auto& p : fs::directory_iterator(song_folder)) {
        if (p.path().extension() == ".memon") {
            res.push_back(p.path());
        }
    }
    return res;
}
