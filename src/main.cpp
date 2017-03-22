#include "main.hpp"
#include "Parser.hpp"
#include "Arena.hpp"
#include "Measure.hpp"

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

        PAPC::Measure max(std::vector<unsigned>({0,2,0,3}));
        PAPC::Measure m1(max);
        PAPC::Measure m2(max);

        if(!m1.setValue(1,1)){
            std::cout << "set(1,1) failed" << std::endl;
        }

        std::cout << "max: ";
        for(size_t i = 0; i < max.getSize(); i++){
            std::cout << max.getValue(i) << " ";
        }
        std::cout << std::endl;

        std::cout << "m1: ";
        for(size_t i = 0; i < m1.getSize(); i++){
            std::cout << m1.getValue(i) << " ";
        }
        std::cout << std::endl;

        if(!m2.makePartialSuccessorOf(1,m1)){
            std::cout << "make partial failed." << std::endl;
        }

        std::cout << "m2: ";
        for(size_t i = 0; i < m2.getSize(); i++){
            std::cout << m2.getValue(i) << " ";
        }
        std::cout << std::endl;

        std::cout << "m1 >=3 m2: " << m1.partialGreaterOrEqual(3,m2) << std::endl;

        std::cout << "m2 >=2 m1: " << m2.partialGreaterOrEqual(2,m1) << std::endl;


    } else if (argc == 2) {
        processGame(argv[1]);
    } else {
        std::cout << "less input plx." << std::endl; // TODO make more sensical
    }

    return 0;
}