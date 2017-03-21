#include "main.hpp"
#include "Parser.hpp"

#include <iostream>

void processGame(char path[])
{
    PAPG::Parser::parse(path);
}

int main(int argc, char* argv[])
{
    if (argc == 1) {
        std::cout << "input plx." << std::endl; // TODO make more sensical usage message
    } else if (argc == 2) {
        processGame(argv[1]);
    } else {
        std::cout << "less input plx." << std::endl; // TODO make more sensical
    }

    return 0;
}