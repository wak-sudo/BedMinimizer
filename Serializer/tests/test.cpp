#include <catch2/catch_test_macros.hpp>
#include <nlohmann/json.hpp>
#include <fstream>
#include <filesystem>

#include "Serializer.h"
#include "DFA.h"

TEST_CASE("Serializer - DFA serialization and deserialization", "[Serializer]") {

    std::vector<std::vector<long>> transitionFunction = {
        {0, 1},
        {1, 0}
    };
    std::vector<long> acceptingStates = {1};
    long initialState = 0;

    DFA originalDfa(transitionFunction, acceptingStates, initialState);

    SECTION("Serialize DFA to JSON object") {
        nlohmann::json j = Serializer::serialize(originalDfa);

        CHECK(j["states_count"] == 2);
        CHECK(j["alphabet_size"] == 2);
        CHECK(j["initial_state"] == 0);
        
        auto actualAcceptStates = j["accept_states"].get<std::vector<long>>();
        REQUIRE(actualAcceptStates.size() == 1);
        CHECK(actualAcceptStates[0] == 1);

        // Zmiana: przejścia są teraz macierzą 2D [stany][symbole]
        REQUIRE(j["transitions"].is_array());
        CHECK(j["transitions"].size() == 2); // 2 stany

        auto transMatrix = j["transitions"].get<std::vector<std::vector<long>>>();
        CHECK(transMatrix == transitionFunction);
    }

    SECTION("Round-Trip: SaveToFile -> Deserialize -> Compare") {
        const std::filesystem::path tempPath = "temp_dfa_test.json";

        struct FileCleaner {
            std::filesystem::path path;
            ~FileCleaner() { std::filesystem::remove(path); }
        } cleaner{tempPath};

        Serializer::saveToFile(tempPath.string(), originalDfa);

        DFA restoredDfa = Serializer::deserializeDfaFromFile(tempPath.string());

        CHECK(restoredDfa.statesCount() == originalDfa.statesCount());
        CHECK(restoredDfa.alphabetSize() == originalDfa.alphabetSize());
        CHECK(restoredDfa.originalState() == originalDfa.originalState());
        CHECK(restoredDfa.acceptingStates() == originalDfa.acceptingStates());
        CHECK(restoredDfa.transFun() == originalDfa.transFun());
    }
}