#pragma once

#include "CnfAst.h"
#include "LogicAst.h"

class Tseytin
{
    static long varTseytinCounter;
    static Cnf::Literal NewTseytinVar();
    static Cnf::Literal TransformAux(std::shared_ptr<LogicAst::Node>, Cnf::Formula &);
public:
    static const std::string HELPER_VAR_PREFIX;
    static Cnf::Formula Transform(std::shared_ptr<LogicAst::Node> node);
    static long GetCounter();
};