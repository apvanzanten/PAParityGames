#include "SPMSolver.hpp"

#include <cstdlib>
#include <ctime>

#include <iostream>
#include <stdexcept>

namespace PAPG {

Measure SPMSolver::makeMaxMeasure() const
{
    std::vector<unsigned> priorityOccurences(arena.getMaxPriority() + 1, 0);

    for (const auto& vertex : arena.getVertices()) {
        const size_t priority = vertex.priority;
        if (priority % 2) { // priority is odd
            priorityOccurences[priority]++;
        }
    }

    return Measure(priorityOccurences);
}

void SPMSolver::initializeMeasures(){
    measures.clear();
    measures.reserve(arena.getSize());

    for (size_t i = 0; i < arena.getSize(); i++) {
        measures.emplace_back(Measure(maxMeasure));
    }
}

SPMSolver::SPMSolver(const Arena& arena)
    : arena(arena)
    , maxMeasure(makeMaxMeasure())
{
        
}

Measure SPMSolver::prog(const size_t fromVertex, const size_t toVertex) const
{
    const size_t priority = arena[fromVertex].priority;
    Measure result(maxMeasure);

    // start with partial equal, and increment if necessary
    result.makePartialEqualOf(priority, measures[toVertex]);


    if (!result.isTop() && (priority % 2)) { 
        if(!result.partialIncrementIfAble(priority)){
            // not able to increment, make top instead
            // std::cout << std::endl << "top result found" << std::endl;
            result.makeTop();
        }
    }

    return result;
}

bool SPMSolver::lift(const size_t vertex)
{
    Measure result(maxMeasure);

    // todo this could use some optimisation

    if (arena[vertex].owner == Player::even) {
        result.makeTop();
        for (const size_t successor : arena[vertex].outgoing) {
            auto intermediateResult = prog(vertex, successor);
            if (intermediateResult < result) {
                // todo move semantics
                result = intermediateResult;
            }
        }
    } else {
        for (const size_t successor : arena[vertex].outgoing) {
            // todo consider checking for top
            auto intermediateResult = prog(vertex, successor);
            if (intermediateResult > result) {
                // todo move semantics
                result = intermediateResult;
            }
        }
    }

    if (result != measures[vertex]) {
        measures[vertex] = result;
        return true;
    }
    return false;
}

std::vector<Player> SPMSolver::solveRandomOrder()
{
    initializeMeasures();

    std::srand(std::time(NULL));

    std::vector<bool> isFinished(arena.getSize(), false);
    size_t numFinishedVertices = 0;

    while (numFinishedVertices != arena.getSize()) {
        const size_t chosenVertex = std::rand() % arena.getSize();

        if (!isFinished[chosenVertex]) {
            if (!lift(chosenVertex)) { // no change was made
                isFinished[chosenVertex] = true;
                numFinishedVertices++;
            } else {
                for (size_t i = 0; i < isFinished.size(); i++) {
                    // First thing to do in each iteration of this loop is loudly proclaim your hatred for vector<bool> :(
                    isFinished[i] = false;
                }
                numFinishedVertices = 0;
            }
        }
    }

    std::vector<Player> result;
    result.reserve(arena.getSize());

    for (auto& measure : measures) {
        if (measure.isTop()) {
            result.emplace_back(Player::odd);
        } else {
            result.emplace_back(Player::even);
        }
    }

    return result;
}

std::vector<Player> SPMSolver::solveInputOrder()
{
    initializeMeasures();

    std::vector<bool> isFinished(arena.getSize(), false);

    size_t currentVertex = 0;

    while (currentVertex != arena.getSize()) {
        if (!isFinished[currentVertex]) {
            if (!lift(currentVertex)) { // no change was made
                isFinished[currentVertex] = true;
                currentVertex++;
            } else {
                for (size_t i = 0; i < isFinished.size(); i++) {
                    // First thing to do in each iteration of this loop is loudly proclaim your hatred for vector<bool> :(
                    isFinished[i] = false;
                }
                currentVertex = 0;
            }
        }
    }

    std::vector<Player> result;
    result.reserve(arena.getSize());

    for (auto& measure : measures) {
        if (measure.isTop()) {
            result.emplace_back(Player::odd);
        } else {
            result.emplace_back(Player::even);
        }
    }

    return result;
}

} // PAPG