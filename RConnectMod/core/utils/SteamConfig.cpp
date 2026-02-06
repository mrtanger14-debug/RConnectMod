#pragma once
#include <steam_api.h>
#include "INIReader.cpp"

class SteamConfig {

    public:

        static const SteamConfig& instance() {
            static SteamConfig inst;
            return inst;
        }

        uint32_t appId;
        uint64 userId;
        std::string index;
        std::string modIndex;
        std::string indexValue;
        std::map<std::string, std::string> filters;

    private:

        SteamConfig () {
            INIReader steamfix;
            INIReader settings;
            settings.load("SeamlessCoop/RConnectMod_settings.ini");
            appId = (steamfix.load("SteamFix.ini") || steamfix.load("OnlineFix.ini")) ? std::stoul(steamfix.get("Main", "FakeAppId", "0")) : SteamUtils()->GetAppID();
            userId = SteamUser()->GetSteamID().ConvertToUint64();
            index = settings.get("DATA", "lobby_main_index");
            modIndex = settings.get("DATA", "lobby_mod_index");
            indexValue = settings.get("FILTERS", index);
            filters = settings.section("FILTERS");
        }
};