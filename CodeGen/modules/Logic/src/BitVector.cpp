#include "BitVector.h"

#include <cmath>
#include <cassert>

using namespace Logic;
using namespace std;

BitVector::BitVector(const std::string& prefix, long num_bits)
{
    if (num_bits < 1)
        throw invalid_argument("Number of bits cannot be lesser than 1");
    bits_.reserve(num_bits);
    for (long i = 0; i < num_bits; ++i)
    {
        const string name = prefix + "_" + std::to_string(i);
        bits_.push_back(name);
    }
}

const Variable &BitVector::bit(unsigned long idx) const
{
    return bits_[idx];
}

size_t BitVector::size() const { return bits_.size(); }

Cnf::Clause Logic::encode_neq_const(const BitVector &X, unsigned long C)
{
    const long xSize = X.size();
    if (C != 0)
    {
        const long cLen = floor(log2(C)) + 1;
        if (cLen > xSize)
            throw invalid_argument("C bit size cannot be bigger than the bitvector size.");
    }

    Cnf::Clause c;

    for (long i = 0; i < xSize; ++i)
    {
        bool bit_c = (C >> i) & 1;
        const auto l = Cnf::Literal(X.bit(i));
        if (bit_c)
        {
            c |= Cnf::NOT(l);
        }
        else
        {
            c |= l;
        }
    }
    return c;
}

LogicAst::NodePtr Logic::encode_lt_const(const BitVector &X, unsigned long C)
{
    if (C == 0)
        throw invalid_argument("For the lt function, C cannot be equal 0.");

    const long cLen = floor(log2(C)) + 1;
    const long xSize = X.size();
    if (cLen > xSize)
        throw invalid_argument("C bit size cannot be bigger than the bitvector size.");

    // we encode this problem in a linear number of clauses
    // by introducing a linear number of auxilary variables.
    // We will use a dynamic approach.
    // Let's assume that c_i and x_i and prev_i-1 are known.
    // prev_i-1 iff X_0:i-1 < C_0:i-1
    // then prev_i iff (c_i or not x_i) and (c_i or prev_i-1) and (not x_i or prev_i-1)

    LogicAst::NodePtr res = nullptr;
    auto helper = LogicAst::GenFreshVar();
    if (C & 1)
        res = IFF(helper, LogicAst::NOT(X.bit(0)));
    else
        res = LogicAst::NOT(helper);

    for (long i = 1; i < xSize; i++)
    {
        const bool bit_c = (C >> i) & 1;

        auto newHelper = LogicAst::GenFreshVar();
        if (!bit_c)
        {
            const auto formula = AND(LogicAst::NOT(X.bit(i)), helper);
            res &= IFF(newHelper, formula);
        }
        else
        {
            const auto formula = OR(LogicAst::NOT(X.bit(i)), helper);
            res &= IFF(newHelper, formula);
        }
        helper = newHelper;
    }

    res &= helper;

    return res;
}