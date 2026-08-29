#include <iostream>
#include <string>

#include "BedMinimizer.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <certificate_filepath>\n";
        return 1;
    }

    const std::string certFilePath = argv[1];

    const auto res = BedMin::verify(certFilePath);

    if (res.has_value())
        std::cout << res.value() << std::endl;
    else
        std::cout << "Valid" << std::endl;

    return 0;
}