#pragma once
#include <ctime>
#include <string>
#include <iostream>
#include <steam_api.h>
#include "DataLobbyProcessor.cpp"
#include "NightreignProgramData.cpp"

class NightreignLobbyProcessor : public DataLobbyProcessor {

public:

    ~NightreignLobbyProcessor() {
        delete data;
    }

    NightreignLobbyProcessor(
        NightreignProgramData* data,
        uint32_t appId,
        const std::string& lobby_key,
        const std::string& lobby_version_key,
        const std::string& lobby_key_value,
        const std::string& lobby_version_value
    ) : data(data), appId(appId), lobby_key(lobby_key), lobby_version_key(lobby_version_key), lobby_key_value(lobby_key_value), lobby_version_value(lobby_version_value) {
        std::cout << "[PLUGIN] NightreignLobbyProcessor initialized success." << std::endl;
    }

    void ProcessorUpdate(float deltaTime) override { 
        DataLobbyProcessor::ProcessorUpdate(deltaTime);
        data->Update();
        HandleAnimation();
    }

    void OnLobbyChatUpdate(LobbyChatUpdate_t* p) override {
        DataLobbyProcessor::OnLobbyChatUpdate(p);
        CSteamID lobbyID(p->m_ulSteamIDLobby);
        CSteamID user(p->m_ulSteamIDUserChanged);
        EChatMemberStateChange change = (EChatMemberStateChange)p->m_rgfChatMemberStateChange;
        if (change & k_EChatMemberStateChangeEntered) {
            std::cout << "[PLUGIN] new user: https://steamcommunity.com/profiles/" << user.ConvertToUint64() << ". in lobby: steam://joinlobby/" << appId << "/" << lobbyID.ConvertToUint64() << std::endl;
        }
    }

    void OnLobbyEnter(LobbyEnter_t* p) override {
        DataLobbyProcessor::OnLobbyEnter(p);
        CSteamID user = SteamUser()->GetSteamID();
        if (currentLobby && currentLobby->owner != user) {
            std::time_t currentTime = std::time(nullptr);
            std::cout << "--- Lobby Info ---" << std::endl;
            std::cout << "Timestamp: " << std::ctime(&currentTime);
            std::cout << "Lobby ID: steam://joinlobby/" << appId << "/" << currentLobby->lobby.ConvertToUint64() << "/" << std::endl;
            std::cout << "Owner ID: https://steamcommunity.com/profiles/" << currentLobby->owner.ConvertToUint64() << "/" << std::endl;
            std::cout << "Members (" << currentLobby->members.size() << "):" << std::endl;
            for (const auto& memberID : currentLobby->members) {
                if (user != memberID) std::cout << "https://steamcommunity.com/profiles/" << memberID.ConvertToUint64() << "/" << std::endl;
            }
            std::cout << "------------------" << std::endl;
        }
    }

    void OnLobbyMatchList(LobbyMatchList_t* p) override {
        DefaultLobbyProcessor::OnLobbyMatchList(p);
        if (p->m_nLobbiesMatching == 0) {
            std::cout << "[PLUGIN] No lobbies found in the match list." << std::endl;
            return;
        }
        if (currentLobby || lastLobby) {
            std::cout << "[PLUGIN] Already in a lobby, not joining a new one." << std::endl;
            return;
        }
        int randomIndex = rand() % p->m_nLobbiesMatching;
        CSteamID selectedLobby = SteamMatchmaking()->GetLobbyByIndex(randomIndex);
        if (selectedLobby.IsValid()) {
            if (SteamMatchmaking()->GetLobbyMemberLimit(selectedLobby) <= SteamMatchmaking()->GetNumLobbyMembers(selectedLobby)) {
                std::cout << "[PLUGIN] cant join to: " << selectedLobby.ConvertToUint64() << " lobby is full." << std::endl;
                return;
            }
            ShellExecuteA(nullptr, "open", ("steam://joinlobby/" + std::to_string(appId) + "/" + std::to_string(selectedLobby.ConvertToUint64())).c_str(), nullptr, nullptr, SW_SHOWNORMAL);
            std::cout << "[PLUGIN] Attempting to join Lobby ID: " << selectedLobby.ConvertToUint64() << ". random index is: " << randomIndex << "." << std::endl;
        }
    }

    void SendLobbyRequest() {
        if (connectType == CONNECT_TO_MOD_USERS) SteamMatchmaking()->AddRequestLobbyListStringFilter(JOIN_PARAM_KEY, "1", k_ELobbyComparisonEqual);
        SteamMatchmaking()->AddRequestLobbyListStringFilter(lobby_key.c_str(), lobby_key_value.c_str(), k_ELobbyComparisonEqual);
        SteamMatchmaking()->AddRequestLobbyListStringFilter(lobby_version_key.c_str(), lobby_version_value.c_str(), k_ELobbyComparisonEqual);
        SteamMatchmaking()->RequestLobbyList();
    }

private:

    static constexpr const char* JOIN_PARAM_KEY = "joinParam";
    static unsigned char const CONNECT_DISABLE = 0;
    static unsigned char const CONNECT_TO_RANDOMS = 1;
    static unsigned char const CONNECT_TO_MOD_USERS = 2;

    uint32_t appId;
    std::string lobby_key;
    std::string lobby_version_key;
    std::string lobby_key_value;
    std::string lobby_version_value;

    NightreignProgramData* data = nullptr;

    unsigned char connectType = CONNECT_DISABLE;

    void ProcessSessionState() override {
        if (!data->InGame()) return;
        if (currentLobby || lastLobby) {
            UpdateRaidStatus(currentLobby) || UpdateRaidStatus(lastLobby);
            return;
        }
        if (connectType && data->InHub()) {
            SendLobbyRequest();
            std::cout << "[PLUGIN] Sending new lobby request..." << std::endl;
        }
    }

    bool UpdateRaidStatus(Lobby* lobby) {
        if (!lobby || lobby->owner != SteamUser()->GetSteamID()) return false;
        if (lobby->parameters.count(lobby_key)) {
            const std::string desired_status = data->InHub() ? "1" : "0";
            const std::string raid_key = JOIN_PARAM_KEY;
            if (lobby->parameters.count(raid_key)) {
                if (lobby->parameters[raid_key] != desired_status) {
                    SteamMatchmaking()->SetLobbyData(lobby->lobby, raid_key.c_str(), desired_status.c_str());
                    std::cout << "[PLUGIN] Updating lobby: " << lobby->lobby.ConvertToUint64() << ". " << raid_key << " from " << lobby->parameters[raid_key] << " to " << desired_status << std::endl;
                    return true;
                }
            } else {
                SteamMatchmaking()->SetLobbyData(lobby->lobby, raid_key.c_str(), desired_status.c_str());
                std::cout << "[PLUGIN] Setting initial data for lobby: " << lobby->lobby.ConvertToUint64() << ". " << raid_key << " to " << desired_status << std::endl;
                return true;
            }
        }
        return false;
    }

    void HandleAnimation() {
        unsigned char previousType = connectType;
        switch (data->GetCurrentAnimation()) {
            case 80200: connectType = CONNECT_TO_RANDOMS; break;
            case 80730: connectType = CONNECT_TO_MOD_USERS; break;
            case 80240: connectType = CONNECT_DISABLE; break;
        }
        if (previousType != connectType) {
            std::cout << "[PLUGIN] Connect type changed to: " << static_cast<int>(connectType) << std::endl;
        }
    }
};