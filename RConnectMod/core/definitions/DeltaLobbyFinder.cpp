#pragma once
#include <vector>
#include <steam_api.h>
#include "../abstraction/AbstractLobbyFinder.h"

class DeltaLobbyFinder : public AbstractLobbyFinder {

    public:

        DeltaLobbyFinder(uint32_t namber, std::map<std::string, std::string> filters) : appId(appId), AbstractLobbyFinder(filters) {
            appId = namber;
        }

        void Update(float deltaTime) override {}

        void LobbyMatchListCount(uint32 count) override {
            std::vector<CSteamID> data;
            for (uint32 index = 0; index < count; index++) {
                data.push_back(SteamMatchmaking()->GetLobbyByIndex(index));
            }
            if (!lobbies.empty()) {
                std::vector<CSteamID> newLobbies;
                for (const auto& currentLobby : data) {
                    auto it = std::find(lobbies.begin(), lobbies.end(), currentLobby);
                    if (it == lobbies.end()) {
                        newLobbies.push_back(currentLobby);
                    }
                }
                std::cout << "[DeltaLobbyFinder] Found new lobbies: " << newLobbies.size() << "." << std::endl;
                if (!newLobbies.empty()) {
                    int randomIndex = rand() % newLobbies.size();
                    CSteamID target = newLobbies[randomIndex];
                    ShellExecuteA(nullptr, "open", ("steam://joinlobby/" + std::to_string(appId) + "/" + std::to_string(target.ConvertToUint64())).c_str(), nullptr, nullptr, SW_SHOWNORMAL);
                    std::cout << "[DeltaLobbyFinder] Selected index: " << randomIndex << ". lobby: " << target.ConvertToUint64() << std::endl;
                }
            }
            lobbies = data;
        }

    private:

        uint32_t appId;
        std::vector<CSteamID> lobbies;
};