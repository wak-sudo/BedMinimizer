#include <iostream>
#include <string>

#include "BedMinimizer.h"

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <dfa_filepath> <target_size>\n";
        return 1;
    }

    const std::string dfaFilePath = argv[1];
    const long targetSize = stoi(std::string(argv[2]));

    const std::string codingRes = "problem.sat";
    const std::string instanceInfo = "instanceInfo.json";

    BedMin::generateCoding(dfaFilePath, targetSize, codingRes, instanceInfo);
    return 0;
}