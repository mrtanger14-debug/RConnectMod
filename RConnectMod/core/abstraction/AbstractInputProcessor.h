#pragma once
#include <string>
#include "AbstractLobbyProcessor.h"

class AbstractInputProcessor {
    protected:
        std::string input_data;

    public:
        virtual AbstractLobbyProcessor* ProcessInput() = 0;
};