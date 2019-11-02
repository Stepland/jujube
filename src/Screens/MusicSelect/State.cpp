#include "State.hpp"

#include <algorithm>
#include <map>

MusicSelect::State::State(const Data::SongList& song_list) {
    ribbon = MusicSelect::Ribbon::title_sort(song_list);
}
