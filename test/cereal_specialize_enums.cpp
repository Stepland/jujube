#include <cassert>
#include <cstdio>
#include <fstream>
#include <string>
#include <unordered_map>

#include <cereal/archives/json.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/unordered_map.hpp>
#include <magic_enum.hpp>

enum class Keys {
    blue,
    green,
    yellow,
    orange,
    red,
};

enum Values {
    YES,
    NO,
    MAYBE
};

namespace cereal {
    template <
        class Archive,
        traits::EnableIf<traits::is_text_archive<Archive>::value> = traits::sfinae
    > inline
    void save(Archive& ar, const std::unordered_map<Keys, Values>& map ) {
        for(auto&& i : magic_enum::enum_entries<Keys>()) {
            auto it = map.find(i.first);
            if (it != map.end()) {
                ar(
                    cereal::make_nvp(
                        std::string{i.second},
                        std::string{magic_enum::enum_name(it->second)}
                    )
                );
            }
        }
    }

    template <
        class Archive,
        traits::EnableIf<traits::is_text_archive<Archive>::value> = traits::sfinae
    > inline
    void load(Archive& ar, std::unordered_map<Keys, Values>& map ) {
        map.clear();
        while(true) {
            const auto namePtr = ar.getNodeName();
            if (not namePtr) {
                break;
            }
            std::string key_str = namePtr;
            std::string value_str;
            ar(value_str);

            auto key = magic_enum::enum_cast<Keys>(key_str);
            if (not key.has_value()) {
                throw std::runtime_error("Invalid Key : "+key_str);
            }

            auto value = magic_enum::enum_cast<Values>(value_str);
            if (not value.has_value()) {
                throw std::runtime_error("Invalid Value : "+value_str);
            }

            map.emplace(*key, *value);
        }
    }
} // namespace cereal

struct MyThingWithAnUnorderedMap {
    std::unordered_map<Keys, Values> m_map;
};

struct Indirect {
    MyThingWithAnUnorderedMap m;

    template<class Archive>
    void serialize(Archive & archive) {
        archive(
            cereal::make_nvp("map", m.m_map)
        ); 
    }
};

int main(int argc, char const *argv[]) {
    Indirect ref;
    ref.m.m_map = {{Keys::blue, YES}, {Keys::green, MAYBE}};
    {
        std::ofstream file;
        file.open("cereal_test_specialize_enums.json", std::ofstream::trunc | std::ofstream::out);
        cereal::JSONOutputArchive archive{file};
        archive(ref);
    }

    Indirect test;
    {
        std::ifstream file;
        file.open("cereal_test_specialize_enums.json", std::ifstream::in);
        cereal::JSONInputArchive archive{file};
        archive(test);
    }
    assert((ref.m.m_map == test.m.m_map));
    std::remove("cereal_test_specialize_enums.json");
    return 0;
}