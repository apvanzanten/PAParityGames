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
    , maxRecursionDepth(0)
{
}

Measure SPMSolver::prog(const size_t fromVertex, const size_t toVertex) const
{
    const size_t priority = arena[fromVertex].priority;
    Measure result(maxMeasure);

    // start with partial equal, and increment if necessary
    result.makePartialEqualOf(priority, measures[toVertex]);

    if (!result.isTop() && (priority % 2)) { 
    // result is not already top, and fromVertex priority is odd
        if (!result.partialIncrementIfAble(priority)) {
            // not able to increment, make top instead
            result.makeTop();
        }
    }

    return result;
}

bool SPMSolver::lift(const size_t vertex)
{
    // std::cerr << "lift(" << vertex << "): " << measures[vertex] << " -> ";
    numLifts++;

    Measure result(maxMeasure);

    if (arena[vertex].owner == Player::even) {
        result.makeTop();
        for (const size_t successor : arena[vertex].outgoing) {
            auto intermediateResult = prog(vertex, successor);
            if (intermediateResult < result) {
                result = intermediateResult;
            }
        }
    } else {
        for (const size_t successor : arena[vertex].outgoing) {
            auto intermediateResult = prog(vertex, successor);
            if (intermediateResult > result) {
                result = intermediateResult;
            }
        }
    }

    // std::cerr << measures[vertex] << std::endl;

    if (result != measures[vertex]) {
        measures[vertex] = result;
        return true;
    }
    return false;
}

std::vector<Player> SPMSolver::solveInputOrder()
{
    initializeMeasures(); // set all measures to (0,..,0)

    std::vector<bool> isFinished(arena.getSize(), false);

    while (std::count(isFinished.begin(), isFinished.end(), false)) {
        for (size_t i = 0; i < isFinished.size(); i++) {
            // First thing to do in each iteration of this loop is loudly proclaim your hatred for vector<bool> :(
            isFinished[i] = false;
        }

        for (size_t currentVertex = 0; currentVertex < arena.getSize(); currentVertex++) {
            if (measures[currentVertex].isTop() || !lift(currentVertex)) { // no change was made
                isFinished[currentVertex] = true;
            }
        }
    }

    return getResult(); // get results from measures table
}

std::vector<Player> SPMSolver::solveRandomOrder()
{
    initializeMeasures(); // set all measures to (0,..,0)

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

    return getResult(); // get results from measures table
}

std::vector<Player> SPMSolver::solvePriorityOrder()
{
    initializeMeasures(); // set all measures to (0,..,0)

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
        // there are unfinished vertices left
        for (size_t i = 0; i < isFinished.size(); i++) {
            isFinished[i] = false;
        }

        for (const auto& currentVertex : priorityOrderMap) {
            if (measures[currentVertex].isTop() || !lift(currentVertex)) { // no change was made
                isFinished[currentVertex] = true;
            }
        }
    }

    return getResult(); // get results from measures table
}

std::vector<Player> SPMSolver::solveIncomingOrder()
{
    initializeMeasures(); // set all measures to (0,..,0)

    std::vector<bool> isFinished(arena.getSize(), false);

    std::vector<size_t> incomingOrderMap;
    incomingOrderMap.reserve(arena.getSize());

    for (size_t i = 0; i < arena.getSize(); i++) {
        incomingOrderMap.emplace_back(i);
    }

    std::sort(incomingOrderMap.begin(), incomingOrderMap.end(), [this](size_t a, size_t b) {
        return arena[a].incoming.size() > arena[b].incoming.size();
    });

    while (std::count(isFinished.begin(), isFinished.end(), false)) {
        for (size_t i = 0; i < isFinished.size(); i++) {
            isFinished[i] = false;
        }

        for (auto& currentVertex : incomingOrderMap) {
            if (measures[currentVertex].isTop() || !lift(currentVertex)) { // no change was made
                isFinished[currentVertex] = true;
            }
        }
    }

    return getResult(); // get results from measures table
}

void SPMSolver::liftRecursive(const std::vector<size_t>& subset)
{
    static unsigned recursionDepth = 0;

    if (recursionDepth > maxRecursionDepth)
        maxRecursionDepth = recursionDepth;

    std::vector<size_t> liftedVertices;
    liftedVertices.reserve(subset.size());

    while (true) {
        for (auto& currentVertex : subset) {
            if (!measures[currentVertex].isTop() && lift(currentVertex)) { // a change was made
                liftedVertices.emplace_back(currentVertex);
            }
        }

        if (liftedVertices.empty()){
            // we were unable to lift anything
            return;
        }

        // If liftedVertices is the same size as our input, that means we
        // lifted all our vertices and a recursive call would have the exact
        // same effect as simply iterating again, therefore only recurse if
        // this is not the case.
        if (liftedVertices.size() != subset.size()) {
            recursionDepth++;
            liftRecursive(liftedVertices);
            recursionDepth--;
        }

        liftedVertices.clear();
    }
}

std::vector<Player> SPMSolver::solveRecursive()
{
    initializeMeasures(); // set all measures to (0,..,0)

    std::vector<size_t> fullSet;
    fullSet.reserve(arena.getSize());
    for (size_t i = 0; i < arena.getSize(); i++) {
        fullSet.emplace_back(i);
    }

    liftRecursive(fullSet);

    return getResult(); // get results from measures table
}

std::vector<Player> SPMSolver::solveRecursivePriorityOrder(){
    initializeMeasures(); // set all measures to (0,..,0)

    std::vector<size_t> fullSet;
    fullSet.reserve(arena.getSize());
    for (size_t i = 0; i < arena.getSize(); i++) {
        fullSet.emplace_back(i);
    }
    
    std::sort(fullSet.begin(), fullSet.end(), [this](size_t a, size_t b) {
        return arena[a].priority < arena[b].priority;
    });

    liftRecursive(fullSet);

    return getResult(); // get results from measures table   
}

std::vector<Player> SPMSolver::solveRecursiveIncomingOrder(){
    initializeMeasures(); // set all measures to (0,..,0)

    std::vector<size_t> fullSet;
    fullSet.reserve(arena.getSize());
    for (size_t i = 0; i < arena.getSize(); i++) {
        fullSet.emplace_back(i);
    }
    
    std::sort(fullSet.begin(), fullSet.end(), [this](size_t a, size_t b) {
        return arena[a].incoming.size() > arena[b].incoming.size();
    });

    liftRecursive(fullSet);

    return getResult(); // get results from measures table   
}


bool SPMSolver::checkForSelfLoop(const Vertex& vertex) const
{
    if (vertex.incoming.size() < vertex.outgoing.size()) {
        return std::find(vertex.incoming.begin(), vertex.incoming.end(), vertex.id) != vertex.incoming.end();
    } else {
        return std::find(vertex.outgoing.begin(), vertex.outgoing.end(), vertex.id) != vertex.outgoing.end();
    }
}

void SPMSolver::lockPredecessorsIfAble(const size_t vertexId, std::vector<size_t> & lockedVertices)
{   
    const Vertex& vertex = arena[vertexId];
    const Measure & vertexMeasure = measures[vertexId];

    for (auto& predecessorId : vertex.incoming) {

        if(std::find(lockedVertices.begin(), lockedVertices.end(), predecessorId) != lockedVertices.end()){
            // predecessor is already locked, next
            continue;
        }

        const Vertex& predecessor = arena[predecessorId];

        if((vertexMeasure.isTop() 
            && predecessor.owner == Player::even 
            && predecessor.outgoing.size() > 1)
            || (!vertexMeasure.isTop()
                && predecessor.owner == Player::odd
                && predecessor.outgoing.size() > 1)){
            // case B,H -> do nothing
            continue;
        }

        // the rest is of case A,C,D,E,F, or G, lift and lock!
        
        while(lift(predecessorId)){
        }
        lockedVertices.emplace_back(predecessorId);
    }
}

std::vector<Player> SPMSolver::solvePropagation()
{
    initializeMeasures(); // set all measures to (0,..,0)

    std::vector<size_t> lockedVertices; // vertices we know will lift no more
    lockedVertices.reserve(arena.getSize());

    // initial pass, lifting cases A,B,D,E,F and G.
    for (auto& vertex : arena.getVertices()) {
        const Player owner = vertex.owner;
        const bool evenPriority = !(vertex.priority % 2);

        if (vertex.outgoing.size() > 1) { // if there are more than 1 outgoing edges, we definitely have an edge that is not a self-loop
            if ((owner == Player::odd && evenPriority) || (owner == Player::even && !evenPriority)) {
                // cases C & H, and also some cases where there is no self-loop at all
                continue;
            }
        }

        if (!checkForSelfLoop(vertex)) {
            // all remaining cases without a self-loop
            continue;
        }

        // At this point we know the vertex is either of case A,B,D,E,F or G, meaning it
        // can be lifted as much as possible and then locked in.

        while (lift(vertex.id)) {
        }

        lockedVertices.emplace_back(vertex.id);
    }

    size_t oldLockedVerticesSize = 0; 

    // repeatedly pass over locked vertices, lift and lock direct attracteds
    // stop when lockedVertices stops growing
    do {
        oldLockedVerticesSize = lockedVertices.size();

        for (auto& vertexId : lockedVertices) {
            lockPredecessorsIfAble(vertexId, lockedVertices);
        }
    } while(lockedVertices.size() != oldLockedVerticesSize);

    std::vector<size_t> unlockedVertices;
    unlockedVertices.reserve(arena.getSize() - lockedVertices.size());

    for(auto & vertex : arena.getVertices()){
        if(std::find(lockedVertices.begin(), lockedVertices.end(), vertex.id) == lockedVertices.end()){
            unlockedVertices.emplace_back(vertex.id);
        }
    }

    // call liftRecursive on what's left
    liftRecursive(unlockedVertices);

    return getResult(); // get results from measures table
}


void SPMSolver::liftPropagationRecursiveHybrid(std::vector<size_t> & subset, std::vector<size_t> & lockedVertices){
    std::vector<size_t> liftedVertices;
    liftedVertices.reserve(subset.size());

    while (true) {
        for (auto& currentVertex : subset) {
            if (!measures[currentVertex].isTop() && lift(currentVertex)) { // a change was made
                if(measures[currentVertex].isTop()) { // vertex was just lifted to top
                    lockPredecessorsIfAble(currentVertex, lockedVertices); // adds any vertices that are locked in the function to the lockedVertices vector
                    lockedVertices.emplace_back(currentVertex);
                }
                liftedVertices.emplace_back(currentVertex);
            }
        }

        if (liftedVertices.empty())
            return;

        if (liftedVertices.size() != subset.size()) {
            liftPropagationRecursiveHybrid(liftedVertices, lockedVertices);
        }

        liftedVertices.clear();
    }
}

std::vector<Player> SPMSolver::solvePropagationRecursiveHybrid(){
    initializeMeasures(); // set all measures to (0,..,0)

    std::vector<size_t> fullSet;
    std::vector<size_t> lockedVertices;
    fullSet.reserve(arena.getSize());
    lockedVertices.reserve(arena.getSize());
    for (size_t i = 0; i < arena.getSize(); i++) {
        fullSet.emplace_back(i);
    }

    // initial pass, lifting cases A,B,D,E,F and G.
    for (auto& vertex : arena.getVertices()) {
        const Player owner = vertex.owner;
        const bool evenPriority = !(vertex.priority % 2);

        if (vertex.outgoing.size() > 1) { // if there are more than 1 outgoing edges, we definitely have an edge that is not a self-loop
            if ((owner == Player::odd && evenPriority) || (owner == Player::even && !evenPriority)) {
                // cases C & H, and also some cases where there is no self-loop at all
                continue;
            }
        }

        if (!checkForSelfLoop(vertex)) {
            // all remaining cases without a self-loop
            continue;
        }

        // At this point we know the vertex is either of case A,B,D,E,F or G, meaning it
        // can be lifted as much as possible and then locked in.

        while (lift(vertex.id)) {
        }

        lockedVertices.emplace_back(vertex.id);
    }

    size_t oldLockedVerticesSize = 0; 

    // repeatedly pass over locked vertices, lift and lock direct attracteds
    // stop when lockedVertices stops growing
    do {
        oldLockedVerticesSize = lockedVertices.size();

        for (auto& vertexId : lockedVertices) {
            lockPredecessorsIfAble(vertexId, lockedVertices);
        }
    } while(lockedVertices.size() != oldLockedVerticesSize);

    std::vector<size_t> unlockedVertices;
    unlockedVertices.reserve(arena.getSize() - lockedVertices.size());

    for(auto & vertex : arena.getVertices()){
        if(std::find(lockedVertices.begin(), lockedVertices.end(), vertex.id) == lockedVertices.end()){
            unlockedVertices.emplace_back(vertex.id);
        }
    }


    liftPropagationRecursiveHybrid(unlockedVertices, lockedVertices);

    return getResult(); // get results from measures table
}


} // PAPG