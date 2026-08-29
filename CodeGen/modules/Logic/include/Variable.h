#pragma once

#include <string>
#include <memory>

#include "LogicAst.h"
#include "CnfAst.h"

class Variable
{
    std::string name_;

public:
    Variable(std::string name) : name_(std::move(name)) {}

    operator std::string() const
    {
        return name_;
    }

    operator LogicAst::NodePtr() const
    {
        return LogicAst::VAR(name_);
    }

    operator Cnf::Literal() const
    {
        return Cnf::Literal(name_);
    }
};
