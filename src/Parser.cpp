#include "Parser.hpp"
#include "Arena.hpp"

#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <algorithm>
#include <stdexcept>

namespace PAPG {
namespace Parser {

    Arena parse(std::string path)
    {
        Arena arena(0); // start with size 0, we'll set size accordingly later

        std::ifstream file;
        file.open(path);
        if (file.is_open()) {
            std::string line;
            std::regex headerRegex("parity.*[0-9]+;[ ]*");
            std::regex vertexRegex("[0-9]+ [0-9]+ [0-9]+ ([0-9]+[,]?)+( \\\".*\\\")?;[ ]*");

            std::regex identifierRegex("[0-9]+");
            std::regex nameRegex("\\\".*\\\"");

            std::smatch match;

            if(std::getline(file, line)){
                // first line has to be header
                if (std::regex_match(line, headerRegex)
                    && std::regex_search(line, match, identifierRegex)) {
                    arena.resize(std::stoi(match.str()) + 1);
                } else {
                    throw std::invalid_argument("ERR: bad input file, missing or malformed header.");
                }
            }

            while (std::getline(file, line)) {
                // all lines after first line have to be vertex declarations
                if (std::regex_match(line, vertexRegex)) {
                    int matchCounter = 0;
                    size_t currentVertex = 0;
                    while (std::regex_search(line, match, identifierRegex)) {
                        switch (matchCounter) {
                        case 0: // vertex identifier
                            currentVertex = std::stoi(match.str());
                            if(!arena.clearVertex(currentVertex)){
                                std::cout << "line: " << line << std::endl; 
                                std::cout << "currentVertex: " << currentVertex << std::endl;
                                std::cout << "match: " << match.str() << std::endl;
                                throw std::invalid_argument("ERR: bad input file");
                            }
                            break;
                        case 1: // vertex priority
                            if(!arena.setVertexPriority(currentVertex, std::stoi(match.str()))){
                                std::cout << "line: " << line << std::endl; 
                                std::cout << "currentVertex: " << currentVertex << std::endl;
                                std::cout << "match: " << match.str() << std::endl;
                                throw std::invalid_argument("ERR: bad input file");
                            }
                            break;
                        case 2: // vertex owner
                            if(match.str() == "0"){
                                if(!arena.setVertexOwner(currentVertex, Player::even)){
                                    std::cout << "line: " << line << std::endl; 
                                    std::cout << "currentVertex: " << currentVertex << std::endl;
                                    std::cout << "match: " << match.str() << std::endl;
                                    throw std::invalid_argument("ERR: bad input file");
                                }
                            } else {
                                if(!arena.setVertexOwner(currentVertex, Player::odd)){
                                    std::cout << "line: " << line << std::endl; 
                                    std::cout << "currentVertex: " << currentVertex << std::endl;
                                    std::cout << "match: " << match.str() << std::endl;
                                    throw std::invalid_argument("ERR: bad input file");
                                }
                            }
                            break;
                        default: // everything from 3 upwards is a successor
                            if(!arena.addEdge(currentVertex, std::stoi(match.str()))){
                                std::cout << "line: " << line << std::endl; 
                                std::cout << "currentVertex: " << currentVertex << std::endl;
                                std::cout << "match: " << match.str() << std::endl;
                                throw std::invalid_argument("ERR: bad input file");
                            }
                            break;
                        }

                        line = match.suffix().str();

                        matchCounter++;
                    }

                    if (std::regex_search(line, match, nameRegex)) {
                        if(!arena.setVertexLabel(currentVertex, match.str())){
                            std::cout << "line: " << line << std::endl; 
                            std::cout << "currentVertex: " << currentVertex << std::endl;
                            std::cout << "match: " << match.str() << std::endl;
                            throw std::invalid_argument("ERR: bad input file");
                        }
                    }
                } else {
                    std::cout << "line: " << line << std::endl;
                    throw std::invalid_argument("ERR: bad input file; line doesn't match regex.");
                }
            }

            file.close();
        } else {
            std::cout << "ERR: Failed to open file." << std::endl;
        }

        return arena;
    }

} // Parser

} // PAPC