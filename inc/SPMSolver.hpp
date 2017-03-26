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

    Measure makeMaxMeasure() const;

    void initializeMeasures();

public:
    explicit SPMSolver(const Arena& arena);

    Measure prog(const size_t fromVertex, const size_t toVertex) const;

    bool lift(const size_t vertex);
    
    std::vector<Player> solveRandomOrder();

    std::vector<Player> solveInputOrder();
};

} // PAPG

#endif // SOLVER_HPP
