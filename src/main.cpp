#include "main.hpp"
#include "Arena.hpp"
#include "Measure.hpp"
#include "Parser.hpp"
#include "SPMSolver.hpp"

#include <iostream>
#include <chrono>

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

        if(i == 15){
            std::cout << "... ";
            break;
        }

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

        if(i == 15){
            std::cout << "... and " << arena.getSize() - i - 1 << " others" << std::endl;
            break;
        }
    }
    
    std::chrono::steady_clock::time_point begin;
    std::chrono::steady_clock::time_point end;


    PAPG::SPMSolver solver(arena);

    std::vector<PAPG::Player> results;


    std::cout << "input order non-returning " << std::flush;
    begin = std::chrono::steady_clock::now();
    results = solver.solveInputOrderNonReturning();
    end = std::chrono::steady_clock::now();
    
    auto inputOrderNonReturningTime = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();            
    auto inputOrderNonReturningLifts = solver.getLiftCount();

    solver.resetLiftCount();

    printResults(arena, results);


    std::cout << "random order " << std::flush;
    begin = std::chrono::steady_clock::now();
    results = solver.solveRandomOrder();
    end = std::chrono::steady_clock::now();
    
    auto randomOrderTime = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();            
    auto randomOrderLifts = solver.getLiftCount();

    solver.resetLiftCount();

    printResults(arena, results);


    std::cout << "priority order non-returning " << std::flush;
    begin = std::chrono::steady_clock::now();
    results = solver.solvePriorityOrderNonReturning();
    end = std::chrono::steady_clock::now();
    
    auto priorityOrderNonReturningTime = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();            
    auto priorityOrderNonReturningLifts = solver.getLiftCount();

    solver.resetLiftCount();
    
    printResults(arena, results);


    std::cout << "incoming order non-returning " << std::flush;
    begin = std::chrono::steady_clock::now();
    results = solver.solveIncomingOrderNonReturning();
    end = std::chrono::steady_clock::now();
    
    auto incomingOrderNonReturningTime = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();            
    auto incomingOrderNonReturningLifts = solver.getLiftCount();

    solver.resetLiftCount();
    
    printResults(arena, results);


    std::cout << "recursive " << std::flush;
    begin = std::chrono::steady_clock::now();
    results = solver.solveRecursive();
    end = std::chrono::steady_clock::now();
    
    auto recursiveTime = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();            
    auto recursiveLifts = solver.getLiftCount();

    solver.resetLiftCount();
    
    printResults(arena, results);


    std::cout << "recursive priority order " << std::flush;
    begin = std::chrono::steady_clock::now();
    results = solver.solveRecursivePriorityOrder();
    end = std::chrono::steady_clock::now();
    
    auto recursivePriorityOrderTime = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();            
    auto recursivePriorityOrderLifts = solver.getLiftCount();

    solver.resetLiftCount();
    
    printResults(arena, results);


    std::cout << "recursive incoming order " << std::flush;
    begin = std::chrono::steady_clock::now();
    results = solver.solveRecursiveIncomingOrder();
    end = std::chrono::steady_clock::now();
    
    auto recursiveIncomingOrderTime = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();            
    auto recursiveIncomingOrderLifts = solver.getLiftCount();

    solver.resetLiftCount();
    
    printResults(arena, results);


    std::cout << "growing " << std::flush;
    begin = std::chrono::steady_clock::now();
    results = solver.solveGrowing();
    end = std::chrono::steady_clock::now();
    
    auto growingTime = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();            
    auto growingLifts = solver.getLiftCount();

    solver.resetLiftCount();
    
    printResults(arena, results);


    std::cout << "growing recursive hybrid " << std::flush;
    begin = std::chrono::steady_clock::now();
    results = solver.solveGrowingRecursiveHybrid();
    end = std::chrono::steady_clock::now();
    
    auto growingRecursiveHybridTime = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();            
    auto growingRecursiveHybridLifts = solver.getLiftCount();

    solver.resetLiftCount();
    
    printResults(arena, results);


    

    std::cout << "# input non-returning / random / priority non-returning / incoming non-returning / recursive / recursive priority order / recursive incoming order / growing / growing recursive hybrid\n";
    std::cout << "# lifts:\t" << inputOrderNonReturningLifts << " / " << randomOrderLifts << " / " << priorityOrderNonReturningLifts << " / " << incomingOrderNonReturningLifts << " / " << recursiveLifts << " / " << recursivePriorityOrderLifts << " / " << recursiveIncomingOrderLifts << " / " << growingLifts << " / " << growingRecursiveHybridLifts << std::endl;
    std::cout << "# time (µS):\t" << inputOrderNonReturningTime << " / " << randomOrderTime << " / " << priorityOrderNonReturningTime << " / " << incomingOrderNonReturningTime << " / " << recursiveTime << " / " << recursivePriorityOrderTime << " / " << recursiveIncomingOrderTime << " / " << growingTime << " / " << growingRecursiveHybridTime << std::endl;

    std::cout << "# total vertices: " << arena.getSize() << std::endl;
    std::cout << "# max recursion depth: " << solver.getMaxRecursionDepth() << std::endl;
    std::cout << "# num locked vertices: " << solver.getNumLockedVertices() << std::endl;

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