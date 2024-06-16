#include <iostream>
#include <fstream>

#include <IO/System/CommandParser.hpp>
#include <IO/System/PrintDebug.hpp>
#include <IO/Commands/CreateMap.hpp>
#include <IO/Commands/SpawnWarrior.hpp>
#include <IO/Commands/SpawnArcher.hpp>
#include <IO/Commands/March.hpp>
#include <IO/Commands/Wait.hpp>
// #include <IO/System/EventLog.hpp>
// #include <IO/Events/MapCreated.hpp>
// #include <IO/Events/UnitSpawned.hpp>
// #include <IO/Events/MarchStarted.hpp>
// #include <IO/Events/MarchEnded.hpp>
// #include <IO/Events/UnitMoved.hpp>
// #include <IO/Events/UnitDied.hpp>
// #include <IO/Events/UnitAttacked.hpp>

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

    // EventLog eventLog;
    // eventLog.log(1, io::MapCreated{ 10, 10 });
    // eventLog.log(1, io::UnitSpawned{ 1, "Archer", 5, 3 });
    // eventLog.log(1, io::UnitSpawned{ 2, "Warrior", 5, 3 });
    // eventLog.log(2, io::MarchStarted{ 1, 5, 3, 7, 9 });
    // eventLog.log(3, io::UnitMoved{ 1, 6, 4 });
    // eventLog.log(4, io::UnitAttacked{ 1, 2, 5, 0 });
    // eventLog.log(5, io::MarchEnded{ 1, 7, 9 });
    // eventLog.log(6, io::UnitDied{ 1 });

    return 0;
}