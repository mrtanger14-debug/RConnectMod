#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <steam_api.h>

struct Lobby {
    
    CSteamID owner;
    CSteamID lobby;
    std::vector<CSteamID> members;
    std::unordered_map<std::string, std::string> parameters;

    Lobby(CSteamID ownerID, CSteamID lobbyID, std::vector<CSteamID> memberList, std::unordered_map<std::string, std::string> data)
        : owner(ownerID), lobby(lobbyID), members(memberList), parameters(data) {}
};
