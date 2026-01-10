#pragma once
#include <iostream>
#include "../abstraction/AbstractLobbyFinder.h"

class RandomLobbyFinder : public AbstractLobbyFinder {

    public:

        RandomLobbyFinder(uint32_t namber, std::map<std::string, std::string> filters) : AbstractLobbyFinder(filters) {
            appId = namber;
        }

        void Update(float deltaTime) override {}

        void LobbyMatchListCount(uint32 count) override {
            int randomIndex = rand() % count;
            CSteamID selectedLobby = SteamMatchmaking()->GetLobbyByIndex(randomIndex);
            if (selectedLobby.IsValid()) {
                ShellExecuteA(nullptr, "open", ("steam://joinlobby/" + std::to_string(appId) + "/" + std::to_string(selectedLobby.ConvertToUint64())).c_str(), nullptr, nullptr, SW_SHOWNORMAL);
                std::cout << "[RandomLobbyFinder] Selected lobby: " << selectedLobby.ConvertToUint64() << ". random index is: " << randomIndex << "." << std::endl;
            }
        }

    private:
        
        uint32_t appId;
};