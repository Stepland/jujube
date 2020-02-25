#include "Song.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>

#include <memon/memon.hpp>

namespace fs = ghc::filesystem;

namespace Data {

    bool cmp_dif_name::operator()(const std::string &a, const std::string &b) const {
        if (dif_names.find(a) != dif_names.end()) {
            if (dif_names.find(b) != dif_names.end()) {
                return dif_names.find(a)->second < dif_names.find(b)->second;
            } else {
                return true;
            }
        } else {
            if (dif_names.find(b) != dif_names.end()) {
                return false;
            } else {
                return a < b;
            }
        }
    }

    std::optional<fs::path> Song::full_cover_path() const {
        if (cover) {
            return folder/cover.value();
        } else {
            return {};
        } 
    }

    std::optional<fs::path> Song::full_audio_path() const {
        if (audio) {
            return folder/audio.value();
        } else {
            return {};
        } 
    }

    SongList::SongList() :
        songs()
    {
        fs::path song_folder = "songs/";

        if (fs::exists(song_folder) and fs::is_directory(song_folder)) {
            for (const auto& dir_item : fs::directory_iterator("songs/")) {
                if (dir_item.is_directory()) {
                    songs.splice(songs.end(), recursiveSongSearch(dir_item.path()));
                }
            }
        }
        std::cout << "Loaded Data::SongList, found " << songs.size() << " songs" << std::endl;
    }

    std::list<std::shared_ptr<Song>> recursiveSongSearch(fs::path song_or_pack) {
        std::list<std::shared_ptr<Song>> res;

        // First try : any .memo file in the folder ?
        fs::directory_iterator folder_memo{song_or_pack};
        if (
            std::any_of(
                fs::begin(folder_memo),
                fs::end(folder_memo),
                [](const fs::directory_entry& de) {return de.path().extension() == ".memo";}
            )
        ) {
            throw std::invalid_argument("jujube does not support .memo files for now ...");
        }

        // Second try : any .memon file in the folder ?
        // if yes get the first one
        fs::directory_iterator folder_memon{song_or_pack};
        auto memon_path = std::find_if(
            fs::begin(folder_memon),
            fs::end(folder_memon),
            [](const fs::directory_entry& de) {return de.path().extension() == ".memon";}
        );
        if (memon_path != fs::end(folder_memon)) {
            auto song = std::make_shared<MemonSong>(memon_path->path());
            if (not song->chart_levels.empty()) {
                res.push_back(song);
            }
            return res;
        }
        // Nothing found : recurse in subfolders
        for (auto& p : fs::directory_iterator(song_or_pack)) {
            if (p.is_directory()) {
                res.splice(res.end(), recursiveSongSearch(p));
            }
        }
        return res;
    }

    MemonSong::MemonSong(const fs::path& t_memon_path) :
        memon_path(t_memon_path)
    {
        auto song_folder = t_memon_path.parent_path();
        folder = song_folder;
        stepland::memon m;
        {
            std::ifstream file(t_memon_path);
            file >> m;
        }
        this->title = m.song_title;
        this->artist = m.artist;
        if (not m.album_cover_path.empty()) {
            this->cover.emplace(m.album_cover_path);
        }
        if (not m.music_path.empty()) {
            this->audio.emplace(m.music_path);
        }
        if (m.preview) {
            this->preview.emplace(
                sf::seconds(m.preview->position),
                sf::seconds(m.preview->duration)
            );
        }
        for (const auto& [difficulty, chart] : m.charts) {
            this->chart_levels[difficulty] = chart.level;
        }
    }

    std::optional<Chart> MemonSong::get_chart(const std::string& difficulty) const {
        stepland::memon m;
        {
            std::ifstream file(memon_path);
            file >> m;
        }
        auto chart = m.charts.find(difficulty);
        if (chart == m.charts.end()) {
            return {};
        } else {
            return Chart(m, difficulty);
        }
    }
}
