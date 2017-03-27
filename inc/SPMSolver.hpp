#ifndef SOLVER_HPP
#define SOLVER_HPP

#include "Arena.hpp"
#include "Measure.hpp"

#include <vector>

namespace PAPG {

class SPMSolver {
private:
    const Arena& arena;

    std::vector<Measure> measures;
    const Measure maxMeasure;

    unsigned numLifts;
    unsigned maxRecursionDepth;

    Measure makeMaxMeasure() const;

    void initializeMeasures();

    std::vector<Player> getResult() const;


public:
    explicit SPMSolver(const Arena& arena);

    Measure prog(const size_t fromVertex, const size_t toVertex) const;

    bool lift(const size_t vertex);
    
    inline unsigned getLiftCount() const { return numLifts; }
    inline void resetLiftCount() { numLifts = 0; }

    std::vector<Player> solveInputOrder();
    std::vector<Player> solveInputOrderNonReturning();
    std::vector<Player> solveRandomOrder();
    std::vector<Player> solvePriorityOrder();
    std::vector<Player> solvePriorityOrderNonReturning();

    void liftRecursive(const std::vector<size_t> & subset);
    std::vector<Player> solveRecursive();

    inline unsigned getMaxRecursionDepth() const { return maxRecursionDepth; }
};

} // PAPG

#endif // SOLVER_HPP
