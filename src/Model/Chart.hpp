#pragma once

#include <string>

class Chart {
public:
    Chart();
};

class ChartMetadata {
public:
    ChartMetadata(
        std::string title,
        std::string artist
    );
private:
    std::string title;
    std::string artist;

}