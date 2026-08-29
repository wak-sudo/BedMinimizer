#pragma once

#include <string>

/// @brief For the external use to recognize variables by the reader.
struct CodingStringTable
{
    static constexpr std::string trans = "trans";
    static constexpr std::string origin = "origin";
    static constexpr std::string final = "final";
    static constexpr std::string belongs = "belongs";
    static constexpr std::string cover = "cover";
    static constexpr std::string coveringPath = "coveringPath";
};