#pragma once

#include <string>
#include <optional>

namespace BedMin
{
    void generateCoding(
        const std::string &dfaFilePath,
        unsigned long targetSize,
        const std::string &codingDestFilePath,
        const std::string &instanceDestFilePath);

    void createCertificate(
        const std::string &instFilePath, const std::string &satResFilePath, const std::string &destCertFilePath, const std::string &destDfaFilePath);

    std::optional<std::string> verify(const std::string &certFilePath);

}
