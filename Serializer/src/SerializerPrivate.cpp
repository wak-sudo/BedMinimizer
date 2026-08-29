#include "SerializerPrivate.h"

#include <fstream>

using namespace std;

nlohmann::json SerializerPrivate::readFile(const string &filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        throw runtime_error("Cannot open a file: " + filePath);
    }

    nlohmann::json j;
    file >> j;

    file.close();

    return j;
}