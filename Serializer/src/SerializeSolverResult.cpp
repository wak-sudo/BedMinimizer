#include "Serializer.h"
#include "SerializerPrivate.h"

#include "SolverResult.h"

using namespace std;

#include <fstream>

SolverResult SerializerPrivate::deserializeSolverResult(
    const nlohmann::json &jSolverRes)
{
    return SolverResult{
        .trans =
            jSolverRes.at("trans").get<std::vector<std::vector<long>>>(),
        .origin =
            jSolverRes.at("origin").get<long>(),
        .acceptingStates =
            jSolverRes.at("accepting_states").get<std::vector<long>>(),
        .dagNodes =
            jSolverRes.at("dag_nodes").get<std::vector<std::vector<long>>>(),
        .covers =
            jSolverRes.at("covers").get<std::vector<std::vector<long>>>(),
        .coveringPaths =
            jSolverRes.at("covering_paths").get<std::vector<std::vector<long>>>()};
}

nlohmann::json SerializerPrivate::serialize(
    const SolverResult &solverRes)
{
    return nlohmann::json{
        {"trans", solverRes.trans},
        {"origin", solverRes.origin},
        {"accepting_states", solverRes.acceptingStates},
        {"dag_nodes", solverRes.dagNodes},
        {"covers", solverRes.covers},
        {"covering_paths", solverRes.coveringPaths}};
}