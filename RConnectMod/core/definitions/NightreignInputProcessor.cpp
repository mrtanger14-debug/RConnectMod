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
#include "../utils/INIReader.cpp"

class NightreignInputProcessor : public AbstractInputProcessor {

public:

    NightreignInputProcessor(const std::string& data){
        input_data = data;
    }

public:

    AbstractLobbyProcessor* ProcessInput() override {
        INIReader mod;
        INIReader steamFix;
        if (!mod.load("SeamlessCoop/RConnectMod_settings.ini")) return nullptr;
        NightreignProgramData* data = new NightreignProgramData();
        if (data->Init()) {
            uint32_t appId = steamFix.load("SteamFix.ini") ? std::stoul(steamFix.get("Main", "FakeAppId", "0")) : SteamUtils()->GetAppID();
            std::string lobby_key = mod.get("FILTERS", "lobby_key", "nrsc_lobby_type");
            std::string lobby_version_key = mod.get("FILTERS", "lobby_version_key", "nrsc_lobby_version");
            std::string lobby_key_value = mod.get("FILTERS", "lobby_key_value", "nrsc_ynx3_seamless_master_lobby");
            std::string lobby_version_value = mod.get("FILTERS", "lobby_version_value", "1.0.8");
            return new NightreignLobbyProcessor(
                data,
                appId,
                lobby_key,
                lobby_version_key,
                lobby_key_value,
                lobby_version_value
            );
        } else {
            delete data;
            std::cout << "[PLUGIN] Failed to initialize Nightreign signatures!" << std::endl;
        }
        return nullptr;
    }
};
