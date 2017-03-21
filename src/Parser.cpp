#include "Parser.hpp"

#include <fstream>
#include <iostream>
#include <regex>
#include <string>

namespace PAPC {
namespace Parser {

    void parse(std::string path)
    {
        std::ifstream file;
        file.open(path);
        if (file.is_open()) {
            std::string line;
            std::regex headerRegex("parity.*[0-9]+;");
            std::regex vertexRegex("[0-9]+ [0-9]+ [0-9]+ ([0-9]+[,]?)+ \\\".*\\\";");

            std::regex identifierRegex("[0-9]+");
            std::regex nameRegex("\\\".*\\\"");

            std::smatch match;

            while (std::getline(file, line)) {
                std::cout << line << " --> ";
                if (std::regex_match(line, headerRegex)) {
                    std::cout << "Found header line!";
                } else if (std::regex_match(line, vertexRegex)) {
                    int matchCounter = 0;
                    while (std::regex_search(line, match, identifierRegex)) {
                        switch (matchCounter) {
                        case 0: // vertex identifier
                            std::cout << "vertex: " << match.str() << "; ";
                            break;
                        case 1: // vertex priority
                            std::cout << "priority: " << match.str() << "; ";
                            break;
                        case 2: // vertex owner
                            std::cout << "owner: " << match.str() << "; ";
                            break;
                        default: // everything from 3 upwards is a successor
                            std::cout << "->: " << match.str() << "; ";
                            break;
                        }

                        line = match.suffix().str();

                        matchCounter++;
                    }

                    if (std::regex_search(line, match, nameRegex)) {
                        std::cout << "name: " << match.str();
                    }
                }

                std::cout << std::endl;
            }

            file.close();
        } else {
            std::cout << "ERR: Failed to open file." << std::endl;
        }
    }

} // Parser

} // PAPC