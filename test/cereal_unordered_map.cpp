#include <cassert>
#include <cstdio>
#include <fstream>
#include <string>
#include <unordered_map>

#include <cereal/archives/json.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/unordered_map.hpp>

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
        file.open("cereal_test_unordered_map.json", std::ofstream::trunc | std::ofstream::out);
        cereal::JSONOutputArchive archive{file};
        archive(ref);
    }

    Indirect test;
    {
        std::ifstream file;
        file.open("cereal_test_unordered_map.json", std::ifstream::in);
        cereal::JSONInputArchive archive{file};
        archive(test);
    }
    assert((ref.m.m_map == test.m.m_map));
    std::remove("cereal_test_specialize_enums.json");
    return 0;
}
