#include <iostream>
#include <fstream>

#include <IO/System/CommandParser.hpp>
#include <IO/System/PrintDebug.hpp>
#include <IO/Commands/CreateMap.hpp>
#include <IO/Commands/SpawnWarrior.hpp>
#include <IO/Commands/SpawnArcher.hpp>
#include <IO/Commands/March.hpp>
#include <IO/Commands/Wait.hpp>

#include "Game/Battle.hpp"


int main(int argc, char** argv)
{
    using namespace sw;

    if (argc != 2) {
        throw std::runtime_error("Error: No file specified in command line argument");
    }

    std::ifstream file(argv[1]);
    if (!file) {
        throw std::runtime_error("Error: File not found - " + std::string(argv[1]));
    }

    std::cout << "Commands:\n";
    sw::Battle game;
    io::CommandParser parser;
    parser.add<io::CreateMap>(
        [&game](auto command)
        {
            printDebug(std::cout, command);
            game.addCommand(std::move(command));
        }).add<io::SpawnWarrior>(
        [&game](auto command)
        {
            printDebug(std::cout, command);
            game.addCommand(std::move(command));
        }).add<io::SpawnArcher>(
        [&game](auto command)
        {
            printDebug(std::cout, command);
            game.addCommand(std::move(command));
        }).add<io::March>(
        [&game](auto command)
        {
            printDebug(std::cout, command);
            game.addCommand(std::move(command));
        }).add<io::Wait>(
        [&game](auto command)
        {
            printDebug(std::cout, command);
            game.addCommand(std::move(command));
        });

    parser.parse(file);

    std::cout << "\n\nEvents:\n";
    game.run();

    return 0;
}