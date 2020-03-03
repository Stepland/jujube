#include <cassert>
#include <fstream>
#include <string>

#include <cereal/archives/json.hpp>
#include <cereal/types/string.hpp>
#include <ghc/filesystem.hpp>

#include "../src/Toolkit/ExtraCerealTypes/GHCFilesystemPath.hpp"

int main(int argc, char const *argv[]) {
    ghc::filesystem::path ref{"hihi/haha/hoho.jpg"};
    {
        std::ofstream file;
        file.open("cereal_test_specialize_path.json", std::ofstream::trunc | std::ofstream::out);
        cereal::JSONOutputArchive archive{file};
        archive(ref);
    }

    ghc::filesystem::path test;
    {
        std::ifstream file;
        file.open("cereal_test_specialize_path.json", std::ifstream::in);
        cereal::JSONInputArchive archive{file};
        archive(test);
    }
    assert((ref == test));
    //std::remove("cereal_test_specialize_path.json");
    return 0;
}