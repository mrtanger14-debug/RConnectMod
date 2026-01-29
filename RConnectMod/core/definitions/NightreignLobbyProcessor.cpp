#pragma once
#include <ctime>
#include <string>
#include <iostream>
#include <steam_api.h>
#include "DataLobbyProcessor.cpp"
#include "NightreignProgramData.cpp"
#include "RandomLobbyFinder.cpp"
#include "DeltaLobbyFinder.cpp"
#include "../abstraction/AbstractLobbyFinder.h"

class NightreignLobbyProcessor : public DataLobbyProcessor {

public:

    ~NightreignLobbyProcessor() {
        delete finder;
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
        std::cout << "[NightreignLobbyProcessor] Initialized success." << std::endl;
    }

    void ProcessorUpdate(float deltaTime) override { 
        DataLobbyProcessor::ProcessorUpdate(deltaTime);
        if (finder) finder->Update(deltaTime);
        data->Update();
        HandleAnimation();
    }

    void OnLobbyChatUpdate(LobbyChatUpdate_t* p) override {
        DataLobbyProcessor::OnLobbyChatUpdate(p);
        CSteamID lobbyID(p->m_ulSteamIDLobby);
        CSteamID user(p->m_ulSteamIDUserChanged);
        EChatMemberStateChange change = (EChatMemberStateChange)p->m_rgfChatMemberStateChange;
        if (change & k_EChatMemberStateChangeEntered) {
            std::cout << "[NightreignLobbyProcessor] new user: https://steamcommunity.com/profiles/" << user.ConvertToUint64() << ". in lobby: steam://joinlobby/" << appId << "/" << lobbyID.ConvertToUint64() << std::endl;
        }
    }

    void OnLobbyEnter(LobbyEnter_t* p) override {
        DataLobbyProcessor::OnLobbyEnter(p);
        CSteamID user = SteamUser()->GetSteamID();
        if (user == currentLobby->owner) {
            SteamMatchmaking()->SetLobbyType(currentLobby->lobby, k_ELobbyTypePublic);
        }
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
            std::cout << "[NightreignLobbyProcessor] No lobbies found in the match list." << std::endl;
            return;
        }
        if (currentLobby || lastLobby) {
            std::cout << "[NightreignLobbyProcessor] Already in a lobby, not joining a new one." << std::endl;
            return;
        }
        if (finder) {
            finder->LobbyMatchListCount(p->m_nLobbiesMatching);
        }
    }

private:

    static constexpr const char* JOIN_PARAM_KEY = "joinParam";
    static unsigned char const CONNECT_DISABLE = 0;
    static unsigned char const CONNECT_TO_RANDOMS = 1;
    static unsigned char const CONNECT_TO_NEW_RANDOMS = 2;
    static unsigned char const CONNECT_TO_MOD_USERS = 3;

    uint32_t appId;
    std::string lobby_key;
    std::string lobby_version_key;
    std::string lobby_key_value;
    std::string lobby_version_value;

    AbstractLobbyFinder* finder = nullptr;
    NightreignProgramData* data = nullptr;

    unsigned char connectType = CONNECT_DISABLE;

    void ProcessSessionState() override {
        if (!data->InGame()) return;
        if (currentLobby || lastLobby) {
            UpdateRaidStatus(currentLobby) || UpdateRaidStatus(lastLobby);
            return;
        }
        if (connectType && data->InHub() && finder) {
            finder->LobbyRequest();
            std::cout << "[NightreignLobbyProcessor] Sending new lobby request..." << std::endl;
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
                    std::cout << "[NightreignLobbyProcessor] Updating lobby: " << lobby->lobby.ConvertToUint64() << ". " << raid_key << " from " << lobby->parameters[raid_key] << " to " << desired_status << std::endl;
                    return true;
                }
            } else {
                SteamMatchmaking()->SetLobbyData(lobby->lobby, raid_key.c_str(), desired_status.c_str());
                std::cout << "[NightreignLobbyProcessor] Setting initial data for lobby: " << lobby->lobby.ConvertToUint64() << ". " << raid_key << " to " << desired_status << std::endl;
                return true;
            }
        }
        return false;
    }

    void HandleAnimation() {
        AbstractLobbyFinder* previousFinder = finder;
        unsigned char previousType = connectType;
        switch (data->GetCurrentAnimation()) {
            case 80200: connectType = CONNECT_TO_RANDOMS; break;
            case 80060: connectType = CONNECT_TO_NEW_RANDOMS; break;
            case 80730: connectType = CONNECT_TO_MOD_USERS; break;
            case 80240: connectType = CONNECT_DISABLE; break;
        }
        if (previousType != connectType) {
            switch (connectType) {
                case CONNECT_TO_RANDOMS:
                    finder = new RandomLobbyFinder(appId, {
                        { lobby_key, lobby_key_value },
                        { lobby_version_key, lobby_version_value }
                    }); break;
                case CONNECT_TO_NEW_RANDOMS:
                    finder = new DeltaLobbyFinder(appId, {
                        { lobby_key, lobby_key_value },
                        { lobby_version_key, lobby_version_value }
                    }); break;
                case CONNECT_TO_MOD_USERS:
                    finder = new RandomLobbyFinder(appId, {
                        { lobby_key, lobby_key_value },
                        { lobby_version_key, lobby_version_value },
                        { std::string(JOIN_PARAM_KEY), "1" }
                    }); break;
                case CONNECT_DISABLE: finder = nullptr; break;
            }
            std::cout << "[NightreignLobbyProcessor] Connect type changed to: " << static_cast<int>(connectType) << std::endl;
        }
        if (previousFinder && previousFinder != finder) {
            delete previousFinder;
            std::cout << "[NightreignLobbyProcessor] PreviousFinder been deleted" << std::endl;
        }
    }

};
