#include <iostream>
#include <string>
#include <functional>
#include <unordered_map>
#include "abstraction/AbstractInputProcessor.h"
#include "abstraction/AbstractLobbyProcessor.h"
#include "definitions/DefaultInputProcessor.cpp"
#include "definitions/NightreignInputProcessor.cpp"

class InputHandler {
public:
    using Factory = std::function<AbstractInputProcessor*(const std::string&)>;

    std::unordered_map<std::string, Factory> commands;

    InputHandler() {
        commands["default"] = [](const std::string& data){ return new DefaultInputProcessor(data); };
        commands["nightreign"] = [](const std::string& data){ return new NightreignInputProcessor(data); };
    }

    AbstractLobbyProcessor* HandleInput(const std::string& input) {
        std::string cmd = input.substr(0, input.find(' '));
        if (commands.count(cmd)) {
            AbstractInputProcessor* inpytProcessor = commands[cmd](input);
            AbstractLobbyProcessor* lobbyProcessor = inpytProcessor->ProcessInput();
            delete inpytProcessor;
            return lobbyProcessor;
        }
        std::cout << "[PLUGIN] unknown command: " << cmd << std::endl;
        return nullptr;
    }
};
