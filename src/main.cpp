#include "main.hpp"
#include "Arena.hpp"
#include "Measure.hpp"
#include "Parser.hpp"
#include "SPMSolver.hpp"

#include <iostream>
#include <chrono>

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

void generateResultsTable(int argc, char* argv[])
{
    std::vector<std::string> paths;
    std::vector<std::vector<unsigned>> times;
    std::vector<std::vector<unsigned>> lifts;
    std::vector<std::vector<std::vector<PAPG::Player>>> results;

    for(int i = 1; i < argc; i++){
        const std::string path = argv[i];

        std::cout << "Testing " << path << "..." << std::flush;

        PAPG::Arena arena = PAPG::Parser::parse(path);
        PAPG::SPMSolver solver(arena);

        std::vector<unsigned> localTimes;
        std::vector<unsigned> localLifts;
        std::vector<std::vector<PAPG::Player>> localResults;

        {
            // Input
            auto begin = std::chrono::steady_clock::now();
            auto result = solver.solveInputOrderNonReturning();
            auto end = std::chrono::steady_clock::now();

            localTimes.push_back(std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count());
            localLifts.push_back(solver.getLiftCount());
            localResults.push_back(result);
            solver.resetLiftCount();
        }

        {
            // Random
            auto begin = std::chrono::steady_clock::now();
            auto result = solver.solveRandomOrder();
            auto end = std::chrono::steady_clock::now();

            localTimes.push_back(std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count());
            localLifts.push_back(solver.getLiftCount());
            localResults.push_back(result);
            solver.resetLiftCount();
        }

        {
            // Priority
            auto begin = std::chrono::steady_clock::now();
            auto result = solver.solvePriorityOrderNonReturning();
            auto end = std::chrono::steady_clock::now();

            localTimes.push_back(std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count());
            localLifts.push_back(solver.getLiftCount());
            localResults.push_back(result);
            solver.resetLiftCount();
        }

        {
            // Incoming
            auto begin = std::chrono::steady_clock::now();
            auto result = solver.solveIncomingOrderNonReturning();
            auto end = std::chrono::steady_clock::now();

            localTimes.push_back(std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count());
            localLifts.push_back(solver.getLiftCount());
            localResults.push_back(result);
            solver.resetLiftCount();
        }

        {
            // Recursive
            auto begin = std::chrono::steady_clock::now();
            auto result = solver.solveRecursive();
            auto end = std::chrono::steady_clock::now();

            localTimes.push_back(std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count());
            localLifts.push_back(solver.getLiftCount());
            localResults.push_back(result);
            solver.resetLiftCount();
        }

        {
            // RecursivePriorityOrder
            auto begin = std::chrono::steady_clock::now();
            auto result = solver.solveRecursivePriorityOrder();
            auto end = std::chrono::steady_clock::now();

            localTimes.push_back(std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count());
            localLifts.push_back(solver.getLiftCount());
            localResults.push_back(result);
            solver.resetLiftCount();
        }

        {
            // RecursiveIncomingOrder
            auto begin = std::chrono::steady_clock::now();
            auto result = solver.solveRecursiveIncomingOrder();
            auto end = std::chrono::steady_clock::now();

            localTimes.push_back(std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count());
            localLifts.push_back(solver.getLiftCount());
            localResults.push_back(result);
            solver.resetLiftCount();
        }

        {
            // Growing
            auto begin = std::chrono::steady_clock::now();
            auto result = solver.solveGrowing();
            auto end = std::chrono::steady_clock::now();

            localTimes.push_back(std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count());
            localLifts.push_back(solver.getLiftCount());
            localResults.push_back(result);
            solver.resetLiftCount();
        }

        {
            // GrowingRecursiveHybrid
            auto begin = std::chrono::steady_clock::now();
            auto result = solver.solveGrowingRecursiveHybrid();
            auto end = std::chrono::steady_clock::now();

            localTimes.push_back(std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count());
            localLifts.push_back(solver.getLiftCount());
            localResults.push_back(result);
            solver.resetLiftCount();
        }

        paths.push_back(path);
        times.push_back(localTimes);
        lifts.push_back(localLifts);
        results.push_back(localResults);

        std::cout << "DONE" << std::endl;
    }

    std::cout << std::endl;

    std::string delim = ";";

    std::cout << "LIFTS:" << std::endl;
    std::cout << "input order" << delim << "random order" << delim << "priority order" << delim << "incoming order" << delim << "recursive" << delim << "recursive priority order" << delim << "recursive incoming order" << delim << "growing" << delim << "growing recursive hybrid\n";
    for(size_t game = 0; game < paths.size(); game++){
        std::cout << paths[game];
        for(auto & numLifts : lifts[game]){
            std::cout << delim << numLifts;
        }
        std::cout << std::endl;
    }

    std::cout << "TIMES:" << std::endl;
    std::cout << "input order" << delim << "random order" << delim << "priority order" << delim << "incoming order" << delim << "recursive" << delim << "recursive priority order" << delim << "recursive incoming order" << delim << "growing" << delim << "growing recursive hybrid\n";
    for(size_t game = 0; game < paths.size(); game++){
        std::cout << paths[game];
        for(auto & time : times[game]){
            std::cout << delim << time;
        }
        std::cout << std::endl;
    }

    std::cout << "Result V0:" << std::endl;
    std::cout << "input order" << delim << "random order" << delim << "priority order" << delim << "incoming order" << delim << "recursive" << delim << "recursive priority order" << delim << "recursive incoming order" << delim << "growing" << delim << "growing recursive hybrid\n";
    for(size_t game = 0; game < paths.size(); game++){
        std::cout << paths[game];
        for(auto & result : results[game]){
            std::cout << delim << (result[0] == PAPG::Player::odd);
        }
        std::cout << std::endl;
    }

}

int main(int argc, char* argv[])
{
    if (argc == 1) {
        std::cout << "input plx." << std::endl; // TODO make more sensical usage message
    } else if (argc == 2) {
        processGame(argv[1]);
    } else {
        generateResultsTable(argc, argv);
    }

    return 0;
}