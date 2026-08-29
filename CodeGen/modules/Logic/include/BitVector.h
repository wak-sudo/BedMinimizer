#pragma once

#include <vector>
#include <memory>

#include "LogicAst.h"
#include "CnfAst.h"

#include "Variable.h"

namespace Logic
{
    class BitVector
    {
        std::vector<Variable> bits_; // bits[0] is the LSB, bits[n-1] is the MSB
    public:
        BitVector(const std::string& prefix, long num_bits);
        const Variable &bit(unsigned long idx) const;
        size_t size() const;
    };

    Cnf::Clause encode_neq_const(const BitVector &X, unsigned long C);
    LogicAst::NodePtr encode_lt_const(const BitVector &X, unsigned long C);
}
