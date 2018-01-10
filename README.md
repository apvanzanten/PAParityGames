# PAParityGames
Parity game solver using the [Small Progress Measures](https://link.springer.com/chapter/10.1007%2F3-540-46541-3_24) algorithm, with a number of different lifting strategies. Developed by Pieter Derks and Arjen van Zanten.

### Strategies
* Input order: Repeatedly iterates over the vertices in the order in which they were given, lifting each vertex if able, until no more lifting can be done.
* Priority order: Repeatedly iterates over the vertices ordered by their priority (ascending), lifting each vertex if able, until no more lifting can be done.
* Incoming order: Repeatedly iterates over the vertices ordered by the number of incoming transitions (descending), lifting each vertex if able, until no more lifting can be done.
* Random order: Repeatedly picks a vertex from the set at random and attempts to lift it, removing the vertex from the set if it could not be lifted, and adding all previously removed vertices back to the set if it was.
* Recursive: Repeatedly iterates over the set of vertices in order (can be Input, Priority, Incoming, or Random order), lifting each vertex if able, recursing on the subset of vertices it was able to lift when it reaches the end of the set, until no more lifting can be done.
* Propagation: Attempts to find 'seed' vertices that can be lifted to their final value immediately, after which it attempts to lift any vertices whose ability to lift depends (directly or indirectly) on that original seed to their final value as well. Seeds are found based on self-loops, depedent vertices are found based on incoming transitions. When no more new seeds can be found and no more dependent vertices can be lifted, the rest of the arena is finished off through Recursive strategy.
* Propagation Recursive Hybrid: Combined Propagation and Recursive by running the Propagation on each recursion.

## Platform Requirements
The given Makefile uses various linux utilities (e.g. rm, tail, find, grep, diff) for the test and clean targets, this may not work well on other platforms. Other than that this project probably runs fine on most other platforms (completely untested).

## Build Requirements
Requires a C++14 capable version of gcc. Makefile should take care of the rest.

## Running
Binary executable will be located at build/papg.

Usage: input one file for (somewhat) human-readable output or multiple files for ';'-delimited tables. i.e.:
papg <PGSolver min parity game>
        e.g. papg testcases/1.gm
        Will run all strategies on the given game and output basic results and measurements in a (somewhat) human-readable format.
papg <PGSolver min parity game> <PGSolver min parity game>+
        e.g. papg testcases/1.gm testcases/2.gm testcases/3.gm
        Will run all stratgeies on the given games and output results and measurements in ';'-delimited tables.

## Testing
### make test
The Makefile target "test" will run the 9 provided test games in the testcases folder and diff the results with the expected results (these are also stored in the testcases folder). It then lists whether or not the test succeeded (and if not, why) on a file-by-file basis.

### make testview
The Makefile target "testview" will run the same 9 test games, and then print the output of the runs.

### make perf
The Makefile target "perf" will run the same 9 test games but this time print only the performance-related output of the runs.