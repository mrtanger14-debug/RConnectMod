#pragma once
#include <map>
#include <string>
#include <iostream>
#include "DefaultLobbySubProcessor.cpp"
#include "../utils/SteamConfig.cpp"

class RandomLobbySubProcessor : public DefaultLobbySubProcessor {

    public:

        RandomLobbySubProcessor(std::map<std::string, std::string> filters) : filters(filters) {}

        void Event() override {
            for (auto it = filters.begin(); it != filters.end(); ++it) {
                SteamMatchmaking()->AddRequestLobbyListStringFilter(
                    it->first.c_str(), 
                    it->second.c_str(), 
                    k_ELobbyComparisonEqual
                );
            }
            SteamMatchmaking()->RequestLobbyList();
            std::cout << "[RandomLobbySubProcessor] Sending new lobby request..." << std::endl;
            
        }

        bool OnLobbyMatchList(LobbyMatchList_t* p) override {
            if (p->m_nLobbiesMatching == 0) {
                std::cout << "[RandomLobbySubProcessor] No lobbies found in the match list." << std::endl;
                return true;
            }
            int randomIndex = rand() % p->m_nLobbiesMatching;
            CSteamID lobby = SteamMatchmaking()->GetLobbyByIndex(randomIndex);
            if (lobby.IsValid()) {
                uint64 selectedLobby = lobby.ConvertToUint64();
                ShellExecuteA(nullptr, "open", ("steam://joinlobby/" + std::to_string(SteamConfig::instance().appId) + "/" + std::to_string(selectedLobby)).c_str(), nullptr, nullptr, SW_SHOWNORMAL);
                std::cout << "[RandomLobbySubProcessor] Try to join selected lobby: " << selectedLobby << ". random index is: " << randomIndex << "." << std::endl;
            }
            return true;
        }

    private:

        std::map<std::string, std::string> filters;
};