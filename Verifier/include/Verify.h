#pragma once

#include "Certificate.h"

#include <memory>
#include <optional>

namespace Verify
{
    std::optional<std::string> checkCertificate(const Certificate &cert);
}