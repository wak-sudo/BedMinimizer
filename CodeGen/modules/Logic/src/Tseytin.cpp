#include "Tseytin.h"

#include <vector>
#include <variant>
#include <stdexcept>

using namespace std;
using namespace Cnf;

long Tseytin::varTseytinCounter = 0;
const std::string Tseytin::HELPER_VAR_PREFIX = "$tseytin_";

Literal Tseytin::NewTseytinVar()
{
    const string s = HELPER_VAR_PREFIX + to_string(varTseytinCounter);
    varTseytinCounter++;
    return Literal(s);
}

long Tseytin::GetCounter()
{
    return varTseytinCounter;
}

Literal Tseytin::TransformAux(shared_ptr<LogicAst::Node> node, Formula &f)
{
    return visit(
        [&](const auto &x) -> Literal
        {
            using T = decay_t<decltype(x)>;

            if constexpr (is_same_v<T, LogicAst::Var>)
            {
                return Literal(x.name());
            }
            else if constexpr (is_same_v<T, LogicAst::Not>)
            {
                const auto subId = TransformAux(x.child(), f);
                const auto newVar = NewTseytinVar();
                f &= AND(OR(NOT(subId), NOT(newVar)), OR(subId, newVar));
                return newVar;
            }
            else if constexpr (is_same_v<T, LogicAst::Binary>)
            {
                const auto leftSubId = TransformAux(x.left(), f);
                const auto rightSubId = TransformAux(x.right(), f);
                const auto newId = NewTseytinVar();
                switch (x.type())
                {
                case LogicAst::Binary::Type::AND:
                    f &= OR(NOT(newId), leftSubId);
                    f &= OR(NOT(newId), rightSubId);
                    f &= OR({newId, NOT(leftSubId), NOT(rightSubId)});
                    break;

                case LogicAst::Binary::Type::OR:
                    f &= OR(newId, NOT(leftSubId));
                    f &= OR(newId, NOT(rightSubId));
                    f &= OR({NOT(newId), leftSubId, rightSubId});
                    break;

                case LogicAst::Binary::Type::IMPLIES:
                    f &= OR({NOT(leftSubId), rightSubId, NOT(newId)});
                    f &= OR(leftSubId, newId);
                    f &= OR(NOT(rightSubId), newId);
                    break;

                case LogicAst::Binary::Type::IFF:
                    f &= OR({NOT(leftSubId), NOT(rightSubId), newId});
                    f &= OR({leftSubId, rightSubId, newId});
                    f &= OR({leftSubId, NOT(rightSubId), NOT(newId)});
                    f &= OR({NOT(leftSubId), rightSubId, NOT(newId)});
                    break;
                }
                return newId;
            }
            else
                throw invalid_argument("Unimplemented type in Tseitin");
        },
        node->data());
}

Formula Tseytin::Transform(shared_ptr<LogicAst::Node> node)
{
    Formula f;
    auto res = TransformAux(node, f);
    f.addClause(Clause(res));
    return f;
}