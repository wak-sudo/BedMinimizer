#include <iostream>
#include <string>

#include "Serializer.h"
#include "Utility.h"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <dfa_filepath>\n";
        return 1;
    }

    const std::string dfaFilePath = argv[1];

    const std::string dfaRes = "minimized.json";

    const auto dfa = Serializer::deserializeDfaFromFile(dfaFilePath);

    const auto mini = Utility::minimizeDFA(dfa);
    
    Serializer::saveToFile(dfaRes, mini);

    return 0;
}