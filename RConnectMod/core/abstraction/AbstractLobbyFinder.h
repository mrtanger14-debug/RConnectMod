#pragma once
#include <map>
#include <string>
#include <steam_api.h>

class AbstractLobbyFinder {

    public:

        AbstractLobbyFinder(std::map<std::string, std::string> filters) : filters(filters) {}

        virtual void Update(float deltaTime) = 0;
        virtual void LobbyMatchListCount(uint32 count) = 0;
        
        void LobbyRequest() {
            for (auto it = filters.begin(); it != filters.end(); ++it) {
                SteamMatchmaking()->AddRequestLobbyListStringFilter(
                    it->first.c_str(), 
                    it->second.c_str(), 
                    k_ELobbyComparisonEqual
                );
            }
            SteamMatchmaking()->RequestLobbyList();
        }

    protected:

        std::map<std::string, std::string> filters;

};
