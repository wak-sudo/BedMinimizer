#pragma once

#include "Variable.h"
#include "BitVector.h"

namespace MakeVar
{
    std::string mkStr(const std::string &s, long i);
    std::string mkStr(const std::string &s, long i, long j);
    std::string mkStr(const std::string &s, long i, long j, long k);

    // ==================== Variables ====================

    // stage 1:
    Variable origin(long);
    Variable final(long);
    Variable trans(long, long, long);
    Variable reachable(long, long, long);
    Variable wordReachable(long, long);
    Variable canReachFinal(long);
    Variable active(long);
    Variable edge(long, long);

    // stage 2:
    Variable belongs(long, long);
    Variable dagE(long, long);
    Variable sameComponent(long, long);
    Variable compNotEmpty(long);

    // stage 3:
    Variable prodRF(long, long);
    Variable cover(long, long);
    Variable subLangF(long, long);

    // stage 4:
    Variable continuationId_prefix(long, long, long);

    // stage 5:
    Variable starDagR(long, long, long);
    Variable closureE(long, long);

    // stage 6:
    Variable prodRS(long, long);
    Variable subLangS(long, long);
    Variable CoveringPath_prefix(long, long);

    // ==================== BV::BitVector ====================

    // stage 4:
    Logic::BitVector continuationId(long, long, long, long);

    // stage 6:
    Logic::BitVector CoveringPath(long, long, long);
}