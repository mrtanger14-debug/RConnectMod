#pragma once
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <Windows.h>
#include <steam_api.h>
#include "NightreignProgramData.cpp"
#include "NightreignLobbyProcessor.cpp"
#include "../abstraction/AbstractInputProcessor.h"

class NightreignInputProcessor : public AbstractInputProcessor {

public:

    NightreignInputProcessor(const std::string& data){
        input_data = data;
    }

public:

    AbstractLobbyProcessor* ProcessInput() override {
        NightreignProgramData* data = new NightreignProgramData();
        if (data->Init()) {
            return new NightreignLobbyProcessor(data);
        } else {
            delete data;
            std::cout << "[NightreignInputProcessor] Failed to initialize Nightreign signatures!" << std::endl;
        }
        return nullptr;
    }
};
