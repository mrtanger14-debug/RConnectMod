#pragma once
#include "../abstraction/AbstractInputProcessor.h"
#include "DefaultLobbyProcessor.cpp"

class DefaultInputProcessor : public AbstractInputProcessor {
public:

    DefaultInputProcessor(const std::string& data){
        input_data = data;
    }

    AbstractLobbyProcessor* ProcessInput() override {
        return new DefaultLobbyProcessor();
    }
};