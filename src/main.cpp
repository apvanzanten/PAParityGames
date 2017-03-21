#include "main.hpp"
#include "Parser.hpp"
#include "Arena.hpp"

#include <iostream>

void processGame(char path[])
{
    PAPG::Arena arena = PAPG::Parser::parse(path);

    for(size_t i = 0; i < arena.getSize(); i++){
        std::cout << "id:" << arena[i].id << " owner:" << (arena[i].owner == PAPG::Player::odd) << " priority:" << arena[i].priority << " successors:{ ";
        for(size_t successor : arena[i].outgoing){
            std::cout << successor << " ";
        }
        std::cout << "}" << std::endl;
    }
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