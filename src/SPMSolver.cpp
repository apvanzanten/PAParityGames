#include "SPMSolver.hpp"

#include <cstdlib>
#include <ctime>

#include <algorithm>
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

void SPMSolver::initializeMeasures()
{
    measures.clear();
    measures.reserve(arena.getSize());

    for (size_t i = 0; i < arena.getSize(); i++) {
        measures.emplace_back(Measure(maxMeasure));
    }
}

std::vector<Player> SPMSolver::getResult() const
{

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

SPMSolver::SPMSolver(const Arena& arena)
    : arena(arena)
    , maxMeasure(makeMaxMeasure())
    , numLifts(0)
{
}

Measure SPMSolver::prog(const size_t fromVertex, const size_t toVertex) const
{
    const size_t priority = arena[fromVertex].priority;
    Measure result(maxMeasure);

    // start with partial equal, and increment if necessary
    result.makePartialEqualOf(priority, measures[toVertex]);

    if (!result.isTop() && (priority % 2)) {
        if (!result.partialIncrementIfAble(priority)) {
            // not able to increment, make top instead
            // std::cout << std::endl << "top result found" << std::endl;
            result.makeTop();
        }
    }

    return result;
}

bool SPMSolver::lift(const size_t vertex)
{
    // std::cerr << "lift(" << vertex << ")" << std::endl;
    numLifts++;

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

std::vector<Player> SPMSolver::solveInputOrder()
{
    initializeMeasures();

    std::vector<bool> isFinished(arena.getSize(), false);

    size_t currentVertex = 0;

    while (currentVertex != arena.getSize()) {
        if (!isFinished[currentVertex]) {
            if (measures[currentVertex].isTop() || !lift(currentVertex)) { // no change was made
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

    return getResult();
}

std::vector<Player> SPMSolver::solveInputOrderNonReturning()
{
    initializeMeasures();

    std::vector<bool> isFinished(arena.getSize(), false);

    while(std::count(isFinished.begin(), isFinished.end(), false)){
        for (size_t i = 0; i < isFinished.size(); i++) {
            // First thing to do in each iteration of this loop is loudly proclaim your hatred for vector<bool> :(
            isFinished[i] = false;
        }

        for (size_t currentVertex = 0; currentVertex < arena.getSize(); currentVertex++){
            if (measures[currentVertex].isTop() || !lift(currentVertex)) { // no change was made
                isFinished[currentVertex] = true;
            }
        }

    }
    
    return getResult();
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
            if (measures[chosenVertex].isTop() || !lift(chosenVertex)) { // no change was made
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

    return getResult();
}

std::vector<Player> SPMSolver::solvePriorityOrder()
{
    initializeMeasures();

    std::vector<bool> isFinished(arena.getSize(), false);

    std::vector<size_t> priorityOrderMap;
    priorityOrderMap.reserve(arena.getSize());

    for (size_t i = 0; i < arena.getSize(); i++) {
        priorityOrderMap.emplace_back(i);
    }

    std::sort(priorityOrderMap.begin(), priorityOrderMap.end(), [this](size_t a, size_t b) {
        return arena[a].priority < arena[b].priority;
    });

    auto vertexIterator = priorityOrderMap.begin();

    while (vertexIterator != priorityOrderMap.end()) {
        const size_t currentVertex = *vertexIterator;

        if (!isFinished[currentVertex]) {
            if (measures[currentVertex].isTop() || !lift(currentVertex)) { // no change was made
                isFinished[currentVertex] = true;
                vertexIterator++;
            } else {
                for (size_t i = 0; i < isFinished.size(); i++) {
                    // First thing to do in each iteration of this loop is loudly proclaim your hatred for vector<bool> :(
                    isFinished[i] = false;
                }
                vertexIterator = priorityOrderMap.begin();
            }
        }
    }

    return getResult();
}

std::vector<Player> SPMSolver::solvePriorityOrderNonReturning()
{
    initializeMeasures();

    std::vector<bool> isFinished(arena.getSize(), false);

    std::vector<size_t> priorityOrderMap;
    priorityOrderMap.reserve(arena.getSize());

    for (size_t i = 0; i < arena.getSize(); i++) {
        priorityOrderMap.emplace_back(i);
    }

    std::sort(priorityOrderMap.begin(), priorityOrderMap.end(), [this](size_t a, size_t b) {
        return arena[a].priority < arena[b].priority;
    });

    while (std::count(isFinished.begin(), isFinished.end(), false)) {
        for (size_t i = 0; i < isFinished.size(); i++) {
            // First thing to do in each iteration of this loop is loudly proclaim your hatred for vector<bool> :(
            isFinished[i] = false;
        }

        for (auto& mapIndex : priorityOrderMap) {
            const size_t currentVertex = priorityOrderMap[mapIndex];

            if (measures[currentVertex].isTop() || !lift(currentVertex)) { // no change was made
                isFinished[currentVertex] = true;
            }
        }
    }

    return getResult();
}

} // PAPG