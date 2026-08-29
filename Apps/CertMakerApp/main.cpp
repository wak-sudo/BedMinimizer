#include <iostream>

#include "BedMinimizer.h"

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <path_to_instance> <path_to_sat_res>\n";
        return 1;
    }

    const std::string satResName = argv[2];
    const std::string instanceName = argv[1];

    const std::string dfaDotNameRes = "output_dfa.dot";
    const std::string dfaJsonNameRes = "output_dfa.json";
    const std::string certName = "certificate.json";

    BedMin::createCertificate(instanceName, satResName, certName, dfaJsonNameRes);

    return 0;
}