#include <iostream>
#include <string>

#include "Serializer.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <dfa_filepath>\n";
        return 1;
    }

    const std::string dfaFilePath = argv[1];

    const std::string dfaRes = "resDfaDot.dot";

    const auto dfa = Serializer::deserializeDfaFromFile(dfaFilePath);
    Serializer::saveDfaAsDot(dfaRes, dfa);

    return 0;
}