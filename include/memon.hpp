#ifndef MEMON_HPP_
#define MEMON_HPP_

#include <cassert>
#include <set>
#include <vector>

#include "json.hpp"

namespace stepland {

    class note {

    public:

        note(int t_position, int t_timing, int t_length = 0, int t_tail_position = 0) {
            if (t_timing<0) {
                throw std::runtime_error(
                    "Tried creating a note with negative timing : "
                    + std::to_string(t_timing)
                );
            }
            if (!(t_position>=0 and t_position<=15)) {
                throw std::runtime_error(
                    "Tried creating a note with invalid position : "
                    + std::to_string(t_position)
                );
            }
            if (t_length<0) {
                throw std::runtime_error(
                    "Tried creating a note with invalid length : "
                    + std::to_string(t_length)
                );
            }
            if (t_length > 0) {
                if (t_tail_position < 0 or t_tail_position > 11 or !tail_pos_correct(t_position, t_tail_position)) {
                    throw std::runtime_error(
                        "Tried creating a long note with invalid tail position : "
                        + std::to_string(t_tail_position)
                    );
                }
            }
            this->timing = t_timing;
            this->pos = t_position;
            this->length = t_length;
            this->tail_pos = t_tail_position;
        };

        static bool tail_pos_correct(int note_position, int tail_position) {
            assert((note_position >= 0 and note_position <= 15));
            assert((tail_position >= 0 and tail_position <= 11));
            int x = note_position%4;
            int y = note_position/4;
            int dx = 0;
            int dy = 0;
            // Vertical
            if (tail_position%2 == 0) {
                // Going up
                if ((tail_position/2)%2 == 0) {
                    dy = -(tail_position/4 + 1);
                // Going down
                } else {
                    dy = tail_position/4 +1;
                }
            // Horizontal
            } else {
                // Going right
                if ((tail_position/2)%2 == 0) {
                    dx = tail_position/4 + 1;
                // Going left
                } else {
                    dx = -(tail_position/4 + 1);
                }
            }
            return ((0 <= x+dx) and (x+dx <= 4)) and ((0 <= y+dy) and (y+dy <= 4));
        };
        bool operator==(const note &rhs) const {
            return timing == rhs.timing && pos == rhs.pos;
        };
        bool operator!=(const note &rhs) const {
            return not(rhs == *this);
        };
        bool operator<(const note &rhs) const {
            if (timing < rhs.timing) {
                return true;
            }
            if (rhs.timing < timing) {
                return false;
            }
            return pos < rhs.pos;
        };
        bool operator>(const note &rhs) const {
            return rhs < *this;
        };
        bool operator<=(const note &rhs) const {
            return !(rhs < *this);
        };
        bool operator>=(const note &rhs) const {
            return !(*this < rhs);
        };
        int get_timing() const {return timing;};
        int get_pos() const {return pos;};
        int get_length() const {return length;};
        int get_tail_pos() const {return tail_pos;};

    private:

        int timing;
        int pos;
        int length;
        int tail_pos;

    };

    struct chart {
        int level;
        std::set<note> notes;
        int resolution;
    };

    struct compare_dif_names {
        const std::map<std::string,int> names = {{"BSC", 0}, {"ADV", 1}, {"EXT", 2}};
        bool operator()(const std::string& a, const std::string& b) const {
            bool a_in_names = names.find(a) != names.end();
            bool b_in_names = names.find(b) != names.end();
            if (a_in_names) {
                if (b_in_names) {
                    return names.at(a) < names.at(b);
                } else {
                    return true;
                }
            } else {
                if (b_in_names) {
                    return false;
                } else {
                    return a < b;
                }
            }
        };
    };
    
    /*
    * Represents a .memon file : several charts and some metadata
    */
    struct memon {

        std::map<std::string,chart,compare_dif_names> charts;
        std::string song_title;
        std::string artist;
        std::string music_path;
        std::string album_cover_path;
        float BPM;
        float offset;

        friend std::istream& operator>>(std::istream& file, memon& m) {
            nlohmann::json j;
            file >> j;
            if (j.find("version") != j.end()) {
                if (j.at("version").is_string()) {
                    auto version = j.at("version").get<std::string>();
                    if (version == "0.1.0") {
                        m.load_from_memon_v0_1_0(j);
                    } else {
                        throw std::invalid_argument("Unsupported .memon version : "+version);
                    }
                } else {
                    throw std::invalid_argument("Unexpected version field : "+j.at("version").dump());
                }
            } else {
                m.load_from_memon_fallback(j);
            }
            return file;
        }

        /*
        * Memon schema v0.1.0 :
        * 	- "data" is now an object mapping a difficulty name to a chart,
        *     this way the difficulty names are guaranteed to be unique
        * 	- "jacket path" is now "album cover path" because engrish much ?
        */
        void load_from_memon_v0_1_0(nlohmann::json memon_json) {
            
            auto metadata = memon_json.at("metadata");
            if (not metadata.is_object()) {
                throw std::invalid_argument("metadata fields is not an object");
            }
            
            this->song_title = metadata.value("song title","");
            this->artist = metadata.value("artist","");
            this->music_path = metadata.value("music path","");
            this->album_cover_path = metadata.value("album cover path","");
            this->BPM = metadata.value("BPM",120.0f);
            this->offset = metadata.value("offset",0.0f);

            if (not memon_json.at("data").is_object()) {
                throw std::invalid_argument("data field is not an object");
            }

            for (auto& [dif_name, chart_json] : memon_json.at("data").items()) {
                
                chart new_chart;
                new_chart.level = chart_json.value("level", 0);
                new_chart.resolution = chart_json.at("resolution").get<int>();

                if (not chart_json.at("notes").is_array()) {
                    throw std::invalid_argument(dif_name+" chart notes field has bad structure");
                }
            
                for (auto& note : chart_json.at("notes")) {
                    if (
                        not (
                            note.is_object()
                            and note.find("n") != note.end()
                            and note.find("t") != note.end()
                            and note.find("l") != note.end()
                            and note.find("p") != note.end()
                        )
                    ) {
                        throw std::invalid_argument(dif_name+" chart has notes with bad structure");
                    }
                    
                    new_chart.notes.emplace(
                        note.at("n").get<int>(),
                        note.at("t").get<int>(),
                        note.at("l").get<int>(),
                        note.at("p").get<int>()
                    );
                }
                this->charts[dif_name] = new_chart;
            }
        }

        /*
        * Fallback memon parser
        * Respects the old schema, with notable quirks :
        *   - "data" is an array of charts, each with a difficulty name
        *   - the album cover path field is named "jacket path"
        */
        void load_from_memon_fallback(nlohmann::json memon_json) {
            
            auto metadata = memon_json.at("metadata");
            if (not metadata.is_object()) {
                throw std::invalid_argument("metadata fields is not an object");
            }

            this->song_title = metadata.value("song title","");
            this->artist = metadata.value("artist","");
            this->music_path = metadata.value("music path","");
            this->album_cover_path = metadata.value("jacket path","");
            this->BPM = metadata.value("BPM",120.f);
            this->offset = metadata.value("offset",0.f);

            if (not memon_json.at("data").is_array()) {
                throw std::invalid_argument("data field is not an array");
            }

            for (auto& chart_json : memon_json.at("data")) {
                chart new_chart;
                new_chart.level = chart_json.value("level", 0);
                new_chart.resolution = chart_json.at("resolution").get<int>();
                std::string dif_name = chart_json.at("dif_name").get<std::string>();

                if (this->charts.find(dif_name) != this->charts.end()) {
                    throw std::invalid_argument("duplicate chart name in memon : "+dif_name);
                }
                if (not chart_json.at("notes").is_array()) {
                    throw std::invalid_argument(dif_name+" chart notes field has bad structure");
                }

                for (auto& note : chart_json.at("notes")) {
                    if (
                        not (
                            note.is_object()
                            and note.find("n") != note.end()
                            and note.find("t") != note.end()
                            and note.find("l") != note.end()
                            and note.find("p") != note.end()
                        )
                    ) {
                        throw std::invalid_argument(dif_name+" chart has notes with bad structure");
                    }
                    
                    new_chart.notes.emplace(
                        note.at("n").get<int>(),
                        note.at("t").get<int>(),
                        note.at("l").get<int>(),
                        note.at("p").get<int>()
                    );
                }
                this->charts[dif_name] = new_chart;
            }
        }
    };
}

#endif /* MEMON_HPP_ */
