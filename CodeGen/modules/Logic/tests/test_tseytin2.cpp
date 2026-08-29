#include <catch2/catch_test_macros.hpp>
#include <vector>

#include "CnfAst.h"
#include "LogicAst.h"
#include "Tseytin.h"

using namespace Cnf;

TEST_CASE("Tseytin complex formula", "[Tseytin]")
{
    const auto varA = LogicAst::VAR("A");
    const auto varB = LogicAst::VAR("B");
    const auto varC = LogicAst::VAR("C");
    const auto varD = LogicAst::VAR("D");

    const Literal lA = Literal("A");
    const Literal lB = Literal("B");
    const Literal lC = Literal("C");
    const Literal lD = Literal("D");

    const auto make_literal = [&](long x)
    { return Literal(Tseytin::HELPER_VAR_PREFIX + std::to_string(x)); };

    SECTION("Tseytin::Transform: (A AND B) => NOT(C OR D)")
    {
        // Assumes that transformation goes from left to right in the LogicAst tree.
        // t1 for (A AND B)
        // t2 for (C OR D)
        // t3 for NOT(t2)
        // t4 for (t1 => t3)

        const long crCounter = Tseytin::GetCounter();
        const Literal t1 = make_literal(crCounter);
        const Literal t2 = make_literal(crCounter + 1);
        const Literal t3 = make_literal(crCounter + 2);
        const Literal t4 = make_literal(crCounter + 3);

        // 1. T1 <=> (A AND B)
        const Clause c1_1 = Clause({NOT(lA), NOT(lB), t1});
        const Clause c1_2 = Clause({lA, NOT(t1)});
        const Clause c1_3 = Clause({lB, NOT(t1)});

        // 2. T2 <=> (C OR D)
        const Clause c2_1 = Clause({lC, lD, NOT(t2)});
        const Clause c2_2 = Clause({NOT(lC), t2});
        const Clause c2_3 = Clause({NOT(lD), t2});

        // 3. T3 <=> NOT(T2)
        const Clause c3_1 = Clause({NOT(t2), NOT(t3)});
        const Clause c3_2 = Clause({t2, t3});

        // 4. T4 <=> (T1 => T3)  -->  T4 <=> (NOT(T1) OR T3)
        const Clause c4_1 = Clause({NOT(t1), t3, NOT(t4)});
        const Clause c4_2 = Clause({t1, t4});
        const Clause c4_3 = Clause({NOT(t3), t4});

        const Clause cRoot = Clause({t4});

        const Formula expected = AND({c1_1, c1_2, c1_3,
                                      c2_1, c2_2, c2_3,
                                      c3_1, c3_2,
                                      c4_1, c4_2, c4_3,
                                      cRoot});

        const auto testComplex = LogicAst::IMPLIES(
            LogicAst::AND(varA, varB),
            LogicAst::NOT(LogicAst::OR(varC, varD)));

        const auto res = Tseytin::Transform(testComplex);

        CHECK(expected == res);
    }
}