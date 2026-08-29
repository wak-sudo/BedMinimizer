#include "Verify.h"
#include "Checker.h"
#include "Utility.h"

#include <stdexcept>

using namespace std;

std::optional<std::string> Verify::checkCertificate(const Certificate &cert)
{
    try
    {
        Checker checker(cert);
        checker.check();
        return nullopt;
    }
    catch(const exception& e)
    {
        return optional(e.what());
    }
}