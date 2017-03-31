# PAParityGames
Parity game solver using small progress measures, developed by Pieter Derks and Arjen van Zanten.

# Platform
The given Makefile uses various linux utilities (e.g. rm, tail, find, grep, diff) for the test and clean targets, this may not work well on other platforms. Other than that this project probably runs fine on most other platforms (completely untested).

# Compilation
Requires a C++14 capable version of gcc. Makefile should take care of the rest.

# Running
Binary executable will be located at build/papg.

Usage: input one file for (somewhat) human-readable output or multiple files for ';'-delimited tables. i.e.:
papg <PGSolver min parity game>
        e.g. papg testcases/1.gm
        Will run all strategies on the given game and output basic results and measurements in a (somewhat) human-readable format.
papg <PGSolver min parity game> <PGSolver min parity game>+
        e.g. papg testcases/1.gm testcases/2.gm testcases/3.gm
        Will run all stratgeies on the given games and output results and measurements in ';'-delimited tables.

# Testing
### make test
The Makefile target "test" will run the 9 provided test games in the testcases folder and diff the results with the expected results (these are also stored in the testcases folder). It then lists whether or not the test succeeded (and if not, why) on a file-by-file basis.

### make testview
The Makefile target "testview" will run the same 9 test games, and then print the output of the runs.

### make perf
The Makefile target "perf" will run the same 9 test games but this time print only the performance-related output of the runs.