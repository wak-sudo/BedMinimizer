#include <catch2/catch_test_macros.hpp>
#include <vector>

#include "CnfAst.h"
#include "LogicAst.h"
#include "Tseytin.h"

using namespace Cnf;

TEST_CASE("Tseytin basics", "[Tseytin]")
{
    const auto varA = LogicAst::VAR("A");
    const auto varB = LogicAst::VAR("B");
    const auto testAnd = LogicAst::AND(varA, varB);
    const auto testOr = LogicAst::OR(varA, varB);
    const auto testNot = LogicAst::NOT(varA);
    const auto testIf = LogicAst::IMPLIES(varA, varB);
    const auto testIff = LogicAst::IFF(varA, varB);

    const Literal lA = Literal("A");
    const Literal lB = Literal("B");
    
    auto crCounter = [&]() { return Literal(Tseytin::HELPER_VAR_PREFIX + std::to_string(Tseytin::GetCounter())); };

    SECTION("Tseytin::Transform basics")
    {
        const Literal fstT = crCounter();

        const Clause fAndC1 = Clause({NOT(lA), NOT(lB), fstT});
        const Clause fAndC2 = Clause({lA, NOT(fstT)});
        const Clause fAndC3 = Clause({lB, NOT(fstT)});
        const Formula fAnd = AND({fAndC1, fAndC2, fAndC3, Clause(fstT)});

        const auto res = Tseytin::Transform(testAnd);

        CHECK(fAnd == res);
    }

    SECTION("Tseytin::Transform - NOT")
    {
        const Literal fstT = crCounter();
        
        // T <=> NOT(A)
        const Clause fNotC1 = Clause({NOT(lA), NOT(fstT)});
        const Clause fNotC2 = Clause({lA, fstT});
        const Formula fNot = AND({fNotC1, fNotC2, Clause(fstT)});

        const auto res = Tseytin::Transform(testNot);

        CHECK(fNot == res);
    }

    SECTION("Tseytin::Transform - OR")
    {
        const Literal fstT = crCounter();

        // T <=> (A OR B)
        const Clause fOrC1 = Clause({lA, lB, NOT(fstT)});
        const Clause fOrC2 = Clause({NOT(lA), fstT});
        const Clause fOrC3 = Clause({NOT(lB), fstT});
        const Formula fOr = AND({fOrC1, fOrC2, fOrC3, Clause(fstT)});

        const auto res = Tseytin::Transform(testOr);

        CHECK(fOr == res);
    }

    SECTION("Tseytin::Transform - IF (Implies)")
    {
        const Literal fstT = crCounter();

        // T <=> (A => B) co jest równoważne T <=> (NOT(A) OR B)
        const Clause fIfC1 = Clause({NOT(lA), lB, NOT(fstT)});
        const Clause fIfC2 = Clause({lA, fstT});
        const Clause fIfC3 = Clause({NOT(lB), fstT});
        const Formula fIf = AND({fIfC1, fIfC2, fIfC3, Clause(fstT)});

        const auto res = Tseytin::Transform(testIf);

        CHECK(fIf == res);
    }

    SECTION("Tseytin::Transform - IFF (Equivalence)")
    {
        const Literal fstT = crCounter();

        // T <=> (A <=> B)
        const Clause fIffC1 = Clause({NOT(lA), NOT(lB), fstT});
        const Clause fIffC2 = Clause({lA, lB, fstT});
        const Clause fIffC3 = Clause({lA, NOT(lB), NOT(fstT)});
        const Clause fIffC4 = Clause({NOT(lA), lB, NOT(fstT)});
        const Formula fIff = AND({fIffC1, fIffC2, fIffC3, fIffC4, Clause(fstT)});

        const auto res = Tseytin::Transform(testIff);

        CHECK(fIff == res);
    }
}