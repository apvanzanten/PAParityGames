#include "main.hpp"
#include "Arena.hpp"
#include "Measure.hpp"
#include "Parser.hpp"
#include "SPMSolver.hpp"

#include <chrono>
#include <iostream>

void printResults(PAPG::Arena& arena, std::vector<PAPG::Player> results)
{
    std::cout << "results:{ ";
    for (size_t i = 0; i < results.size(); i++) {
        std::cout << arena[i].id << ":" << (results[i] == PAPG::Player::odd) << " ";

        if (i == 15) {
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

        if (i == 15) {
            std::cout << "... and " << arena.getSize() - i - 1 << " others" << std::endl;
            break;
        }
    }

    std::chrono::steady_clock::time_point begin;
    std::chrono::steady_clock::time_point end;

    PAPG::SPMSolver solver(arena);

    std::vector<PAPG::Player> results;

    // Input order
    std::cout << "input order " << std::flush;
    begin = std::chrono::steady_clock::now();
    results = solver.solveInputOrder();
    end = std::chrono::steady_clock::now();

    auto inputOrderNonReturningTime = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
    auto inputOrderNonReturningLifts = solver.getLiftCount();

    printResults(arena, results);
    solver.resetLiftCount();

    // Random order
    std::cout << "random order " << std::flush;
    begin = std::chrono::steady_clock::now();
    results = solver.solveRandomOrder();
    end = std::chrono::steady_clock::now();

    auto randomOrderTime = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
    auto randomOrderLifts = solver.getLiftCount();

    printResults(arena, results);
    solver.resetLiftCount();

    // Priority order
    std::cout << "priority order " << std::flush;
    begin = std::chrono::steady_clock::now();
    results = solver.solvePriorityOrder();
    end = std::chrono::steady_clock::now();

    auto priorityOrderNonReturningTime = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
    auto priorityOrderNonReturningLifts = solver.getLiftCount();

    printResults(arena, results);
    solver.resetLiftCount();

    // Incoming order
    std::cout << "incoming order " << std::flush;
    begin = std::chrono::steady_clock::now();
    results = solver.solveIncomingOrder();
    end = std::chrono::steady_clock::now();

    auto incomingOrderNonReturningTime = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
    auto incomingOrderNonReturningLifts = solver.getLiftCount();

    printResults(arena, results);
    solver.resetLiftCount();

    // Recursive
    std::cout << "recursive " << std::flush;
    begin = std::chrono::steady_clock::now();
    results = solver.solveRecursive();
    end = std::chrono::steady_clock::now();

    auto recursiveTime = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
    auto recursiveLifts = solver.getLiftCount();

    printResults(arena, results);
    std::cout << "# Recursive strategy max recursion depth: " << solver.getMaxRecursionDepth() << std::endl;

    solver.resetMaxRecursionDepth();
    solver.resetLiftCount();

    // Recursive priority order
    std::cout << "recursive priority order " << std::flush;
    begin = std::chrono::steady_clock::now();
    results = solver.solveRecursivePriorityOrder();
    end = std::chrono::steady_clock::now();

    auto recursivePriorityOrderTime = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
    auto recursivePriorityOrderLifts = solver.getLiftCount();

    printResults(arena, results);
    std::cout << "# Recurisve priority order strategy max recursion depth: " << solver.getMaxRecursionDepth() << std::endl;

    solver.resetMaxRecursionDepth();
    solver.resetLiftCount();

    // Recursive incoming order
    std::cout << "recursive incoming order " << std::flush;
    begin = std::chrono::steady_clock::now();
    results = solver.solveRecursiveIncomingOrder();
    end = std::chrono::steady_clock::now();

    auto recursiveIncomingOrderTime = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
    auto recursiveIncomingOrderLifts = solver.getLiftCount();

    printResults(arena, results);
    std::cout << "# Recursive incoming order max recursion depth: " << solver.getMaxRecursionDepth() << std::endl;

    solver.resetMaxRecursionDepth();
    solver.resetLiftCount();

    // Propagation
    std::cout << "propagation " << std::flush;
    begin = std::chrono::steady_clock::now();
    results = solver.solvePropagation();
    end = std::chrono::steady_clock::now();

    auto propagationTime = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
    auto propagationLifts = solver.getLiftCount();

    printResults(arena, results);
    std::cout << "# Propagation max recursion depth: " << solver.getMaxRecursionDepth() << std::endl;

    solver.resetMaxRecursionDepth();
    solver.resetLiftCount();

    // Propagation recursive hybrid
    std::cout << "propagation recursive hybrid " << std::flush;
    begin = std::chrono::steady_clock::now();
    results = solver.solvePropagationRecursiveHybrid();
    end = std::chrono::steady_clock::now();

    auto propagationRecursiveHybridTime = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
    auto propagationRecursiveHybridLifts = solver.getLiftCount();

    printResults(arena, results);
    std::cout << "# Propagation recursive hybrid max recursion depth: " << solver.getMaxRecursionDepth() << std::endl;

    solver.resetMaxRecursionDepth();
    solver.resetLiftCount();

    std::cout << "# input order / random order / priority order / incoming order / recursive / recursive priority order / recursive incoming order / propagation / propagation recursive hybrid\n";
    std::cout << "# lifts:\t" << inputOrderNonReturningLifts << " / " << randomOrderLifts << " / " << priorityOrderNonReturningLifts << " / " << incomingOrderNonReturningLifts << " / " << recursiveLifts << " / " << recursivePriorityOrderLifts << " / " << recursiveIncomingOrderLifts << " / " << propagationLifts << " / " << propagationRecursiveHybridLifts << std::endl;
    std::cout << "# time (µS):\t" << inputOrderNonReturningTime << " / " << randomOrderTime << " / " << priorityOrderNonReturningTime << " / " << incomingOrderNonReturningTime << " / " << recursiveTime << " / " << recursivePriorityOrderTime << " / " << recursiveIncomingOrderTime << " / " << propagationTime << " / " << propagationRecursiveHybridTime << std::endl;

    std::cout << "# total vertices: " << arena.getSize() << std::endl;
}

void generateResultsTable(int argc, char* argv[])
{
    std::vector<std::string> paths;
    std::vector<std::vector<unsigned long long> > times;
    std::vector<std::vector<unsigned long long> > lifts;
    std::vector<std::vector<std::vector<PAPG::Player> > > results;

    for (int i = 1; i < argc; i++) {
        const std::string path = argv[i];

        std::cerr << "Testing " << path << "..." << std::flush;

        PAPG::Arena arena = PAPG::Parser::parse(path);
        PAPG::SPMSolver solver(arena);

        std::vector<unsigned long long> localTimes;
        std::vector<unsigned long long> localLifts;
        std::vector<std::vector<PAPG::Player> > localResults;

        {
            // Input
            auto begin = std::chrono::steady_clock::now();
            auto result = solver.solveInputOrder();
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
            auto result = solver.solvePriorityOrder();
            auto end = std::chrono::steady_clock::now();

            localTimes.push_back(std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count());
            localLifts.push_back(solver.getLiftCount());
            localResults.push_back(result);
            solver.resetLiftCount();
        }

        {
            // Incoming
            auto begin = std::chrono::steady_clock::now();
            auto result = solver.solveIncomingOrder();
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
            // Recursive Priority Order
            auto begin = std::chrono::steady_clock::now();
            auto result = solver.solveRecursivePriorityOrder();
            auto end = std::chrono::steady_clock::now();

            localTimes.push_back(std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count());
            localLifts.push_back(solver.getLiftCount());
            localResults.push_back(result);
            solver.resetLiftCount();
        }

        {
            // Recursive Incoming Order
            auto begin = std::chrono::steady_clock::now();
            auto result = solver.solveRecursiveIncomingOrder();
            auto end = std::chrono::steady_clock::now();

            localTimes.push_back(std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count());
            localLifts.push_back(solver.getLiftCount());
            localResults.push_back(result);
            solver.resetLiftCount();
        }

        {
            // propagation
            auto begin = std::chrono::steady_clock::now();
            auto result = solver.solvePropagation();
            auto end = std::chrono::steady_clock::now();

            localTimes.push_back(std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count());
            localLifts.push_back(solver.getLiftCount());
            localResults.push_back(result);
            solver.resetLiftCount();
        }

        {
            // propagationRecursiveHybrid
            auto begin = std::chrono::steady_clock::now();
            auto result = solver.solvePropagationRecursiveHybrid();
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

        std::cerr << "DONE" << std::endl;
    }

    std::cout << std::endl;

    std::string delim = ";";

    std::string header = "path";
    header += delim + "input order" + delim + "random order" + delim + "priority order" + delim + "incoming order"; 
    header += delim + "recursive" + delim + "recursive priority order" + delim + "recursive incoming order" + delim + "propagation" + delim + "propagation recursive hybrid";

    std::cout << "Lifts:" << std::endl;
    std::cout << header << std::endl;
    for (size_t game = 0; game < paths.size(); game++) {
        std::cout << paths[game];
        for (auto& numLifts : lifts[game]) {
            std::cout << delim << numLifts;
        }
        std::cout << std::endl;
    }

    std::cout << "Time (µS):" << std::endl;
    std::cout << header << std::endl;
    for (size_t game = 0; game < paths.size(); game++) {
        std::cout << paths[game];
        for (auto& time : times[game]) {
            std::cout << delim << time;
        }
        std::cout << std::endl;
    }

    std::cout << "Outcome V0:" << std::endl;
    std::cout << header << std::endl;
    for (size_t game = 0; game < paths.size(); game++) {
        std::cout << paths[game];
        for (auto& result : results[game]) {
            std::cout << delim << (result[0] == PAPG::Player::odd);
        }
        std::cout << std::endl;
    }
}

int main(int argc, char* argv[])
{
    if (argc == 1) {
        std::cout << "Usage: input one file for (somewhat) human-readable output or multiple files for ';'-delimited tables. i.e.:" << std::endl;
        std::cout << "papg <PGSolver min parity game>" << std::endl;
        std::cout << "\te.g. papg testcases/1.gm" << std::endl;
        std::cout << "\tWill run all strategies on the given game and output basic results and measurements in a (somewhat) human-readable format." << std::endl;
        std::cout << "papg <PGSolver min parity game> <PGSolver min parity game>+" << std::endl;
        std::cout << "\te.g. papg testcases/1.gm testcases/2.gm testcases/3.gm" << std::endl;
        std::cout << "\tWill run all stratgeies on the given games and output results and measurements in ';'-delimited tables." << std::endl;
    } else if (argc == 2) {
        processGame(argv[1]);
    } else {
        generateResultsTable(argc, argv);
    }

    return 0;
}