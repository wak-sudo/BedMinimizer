#include "MakeVar.h"
#include "CodingStringTable.h"

#include "Utility.h"

#include <string>

std::string MakeVar::mkStr(const std::string &s, long i)
{
    return s + "_" + std::to_string(i);
}

std::string MakeVar::mkStr(const std::string &s, long i, long j)
{
    return s + "_" + std::to_string(i) + "_" + std::to_string(j);
}

std::string MakeVar::mkStr(const std::string &s, long i, long j, long k)
{
    return s + "_" + std::to_string(i) + "_" + std::to_string(j) + "_" + std::to_string(k);
}

// stage 1:
Variable MakeVar::origin(long q) { return Variable(mkStr(CodingStringTable::origin, q)); }
Variable MakeVar::final(long q) { return Variable(mkStr(CodingStringTable::final, q)); }
Variable MakeVar::trans(long q, long a, long p) { return Variable(mkStr(CodingStringTable::trans, q, a, p)); }
Variable MakeVar::reachable(long p, long q, long s) { return Variable(mkStr("reachable", p, q, s)); }
Variable MakeVar::wordReachable(long p, long q) { return Variable(mkStr("wordReachable", p, q)); }
Variable MakeVar::canReachFinal(long p) { return Variable(mkStr("canReachFinal", p)); }
Variable MakeVar::active(long q) { return Variable(mkStr("active", q)); }
Variable MakeVar::edge(long p, long q) { return Variable(mkStr("edge", p, q)); }

// stage 2:
Variable MakeVar::belongs(long c, long q) { return Variable(mkStr(CodingStringTable::belongs, c, q)); }
Variable MakeVar::dagE(long c, long d) { return Variable(mkStr("dagE", c, d)); }
Variable MakeVar::sameComponent(long q, long p) { return Variable(mkStr("sameComponent", q, p)); }
Variable MakeVar::compNotEmpty(long p) { return Variable(mkStr("compNotEmpty", p)); }

// stage 3:
Variable MakeVar::prodRF(long p, long q) { return Variable(mkStr("prodRF", p, q)); }
Variable MakeVar::cover(long c, long i) { return Variable(mkStr(CodingStringTable::cover, c, i)); }
Variable MakeVar::subLangF(long b, long a) { return Variable(mkStr("subLangF", b, a)); }

// stage 4:
Variable MakeVar::continuationId_prefix(long a, long b, long p) { return Variable(mkStr("continuationId", a, b, p)); }

// stage 5:
Variable MakeVar::starDagR(long p, long q, long s) { return Variable(mkStr("starDagR", p, q, s)); }
Variable MakeVar::closureE(long p, long q) { return Variable(mkStr("closureE", p, q)); }

// stage 6:
Variable MakeVar::prodRS(long p, long q) { return Variable(mkStr("prodRS", p, q)); }
Variable MakeVar::subLangS(long a, long b) { return Variable(mkStr("subLangS", a, b)); }
Variable MakeVar::CoveringPath_prefix(long i, long j) { return Variable(mkStr(CodingStringTable::coveringPath, i, j)); }

// ==================== BV::BitVector ====================

// stage 4:
Logic::BitVector MakeVar::continuationId(long a, long b, long p, long size)
{
    return Logic::BitVector(continuationId_prefix(a, b, p), size);
}

// stage 6:
Logic::BitVector MakeVar::CoveringPath(long i, long j, long size)
{
    return Logic::BitVector(CoveringPath_prefix(i, j), size);
}