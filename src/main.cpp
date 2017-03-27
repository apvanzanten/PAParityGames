#include "main.hpp"
#include "Arena.hpp"
#include "Measure.hpp"
#include "Parser.hpp"
#include "SPMSolver.hpp"

#include <iostream>

void testMeasure()
{
    PAPG::Measure max(std::vector<unsigned>({ 0, 2, 0, 3 }));
    PAPG::Measure m1(max);
    PAPG::Measure m2(max);
    PAPG::Measure m3(max);
    PAPG::Measure m4(max);

    if (!m1.setValue(1, 1)) {
        std::cout << "set(1,1) failed" << std::endl;
    }

    std::cout << "max: " << max << std::endl;
    std::cout << "m1: " << m1 << std::endl;
    
    m2.makePartialEqualOf(1,m1);
    m2.partialIncrementIfAble(1);

    std::cout << "m2: " << m2 << std::endl;
    
    m3.makePartialEqualOf(3, max);
    m4.makePartialEqualOf(2, m3);

    std::cout << "m3: " << m3 << std::endl;
    
    std::cout << "m4: " << m4 << std::endl;
    
    std::cout << "m1 >=3 m2: " << m1.partialGreaterOrEqual(3, m2) << std::endl;

    std::cout << "m2 >=2 m1: " << m2.partialGreaterOrEqual(2, m1) << std::endl;

    std::cout << "m1 == m2: " << (m1 == m2) << std::endl;
    std::cout << "m1 < m2: " << (m1 < m2) << std::endl;
    std::cout << "m1 > m2: " << (m1 > m2) << std::endl;
    std::cout << "m1 <= m2: " << (m1 <= m2) << std::endl;
    std::cout << "m1 >= m2: " << (m1 >= m2) << std::endl;

    std::cout << "m1 == m1: " << (m1 == m1) << std::endl;
    std::cout << "m1 < m1: " << (m1 < m1) << std::endl;
    std::cout << "m1 > m1: " << (m1 > m1) << std::endl;
    std::cout << "m1 <= m1: " << (m1 <= m1) << std::endl;
    std::cout << "m1 >= m1: " << (m1 >= m1) << std::endl;

    std::cout << "m2 == m2: " << (m2 == m2) << std::endl;
    std::cout << "m2 < m2: " << (m2 < m2) << std::endl;
    std::cout << "m2 > m2: " << (m2 > m2) << std::endl;
    std::cout << "m2 <= m2: " << (m2 <= m2) << std::endl;
    std::cout << "m2 >= m2: " << (m2 >= m2) << std::endl;

    std::cout << "m3 == max: " << (m3 == max) << std::endl;
    std::cout << "m3 == m4: " << (m3 == m4) << std::endl;

    std::cout << "m3 < m4: " << (m3 < m4) << std::endl;
    std::cout << "m3 > m4: " << (m3 > m4) << std::endl;
    std::cout << "m3 <= m4: " << (m3 <= m4) << std::endl;
    std::cout << "m3 >= m4: " << (m3 >= m4) << std::endl;
}

void printResults(PAPG::Arena & arena, std::vector<PAPG::Player> results)
{
    std::cout << "results:{ ";
    for (size_t i = 0; i < results.size(); i++) {
        std::cout << arena[i].id << ":" << (results[i] == PAPG::Player::odd) << " ";
    }
    std::cout << "}" << std::endl;
}

void processGame(char path[])
{
    PAPG::Arena arena = PAPG::Parser::parse(path);


    for (size_t i = 0; i < arena.getSize(); i++) {
        std::cout << "id:" << arena[i].id << " owner:" << (arena[i].owner == PAPG::Player::odd) << " priority:" << arena[i].priority << " successors:{ ";
        for (size_t successor : arena[i].outgoing) {
            std::cout << successor << " ";
        }
        std::cout << "}" << std::endl;
    }
    
    PAPG::SPMSolver solver(arena);

    std::cout << "input order " << std::flush;
    printResults(arena, solver.solveInputOrder());

    std::cout << "# lifts: " << solver.getLiftCount() << std::endl;
    solver.resetLiftCount();

    std::cout << "random order " << std::flush;
    printResults(arena, solver.solveRandomOrder());

    std::cout << "# lifts: " << solver.getLiftCount() << std::endl;
    solver.resetLiftCount();

    std::cout << "priority order " << std::flush;
    printResults(arena, solver.solvePriorityOrder());

    std::cout << "# lifts: " << solver.getLiftCount() << std::endl;
    solver.resetLiftCount();    
}

int main(int argc, char* argv[])
{
    if (argc == 1) {
        std::cout << "input plx." << std::endl; // TODO make more sensical usage message

        testMeasure();
    } else if (argc == 2) {
        processGame(argv[1]);
    } else {
        std::cout << "less input plx." << std::endl; // TODO make more sensical
    }

    return 0;
}