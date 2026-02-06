#pragma once
#include <ctime>
#include <string>
#include "DataLobbyProcessor.cpp"
#include "NightreignProgramData.cpp"
#include "DefaultLobbySubProcessor.cpp"
// ehehehehehehe
#include "RandomLobbySubProcessor.cpp"
#include "../abstraction/AbstractLobbySubProcessor.h"

class NightreignLobbyProcessor : public DataLobbyProcessor {

public:

    ~NightreignLobbyProcessor() {
        delete data;
        delete subprocessor;
    }

    NightreignLobbyProcessor(NightreignProgramData* data) : data(data) {
        subprocessor = new DefaultLobbySubProcessor();
        std::cout << "[NightreignLobbyProcessor] Initialized success." << std::endl;
    }

    void ProcessorUpdate(float deltaTime) override {
        if (!subprocessor->ProcessorUpdate(deltaTime)) return; 
        DataLobbyProcessor::ProcessorUpdate(deltaTime);
        data->Update();
        HandleAnimation();
    }

    void OnLobbyCreated(LobbyCreated_t* p) override {
        if (!subprocessor->OnLobbyCreated(p)) return; 
        DataLobbyProcessor::OnLobbyCreated(p);
    }

    void OnLobbyEnter(LobbyEnter_t* p) override {
        if (!subprocessor->OnLobbyEnter(p)) return; 
        DataLobbyProcessor::OnLobbyEnter(p);
        CSteamID user(SteamConfig::instance().userId);
        if (user == currentLobby->owner) {
            SteamMatchmaking()->SetLobbyType(currentLobby->lobby, k_ELobbyTypePublic);
        }
        if (currentLobby && currentLobby->owner != user) {
            std::time_t currentTime = std::time(nullptr);
            std::cout << "--- Lobby Info ---" << std::endl;
            std::cout << "Timestamp: " << std::ctime(&currentTime);
            std::cout << "Lobby ID: steam://joinlobby/" << SteamConfig::instance().appId << "/" << currentLobby->lobby.ConvertToUint64() << "/" << std::endl;
            std::cout << "Owner ID: https://steamcommunity.com/profiles/" << currentLobby->owner.ConvertToUint64() << "/" << std::endl;
            std::cout << "Members (" << currentLobby->members.size() << "):" << std::endl;
            for (const auto& memberID : currentLobby->members) {
                if (user != memberID) std::cout << "https://steamcommunity.com/profiles/" << memberID.ConvertToUint64() << "/" << std::endl;
            }
            std::cout << "------------------" << std::endl;
        }
    }

    void OnGameLobbyJoinRequested(GameLobbyJoinRequested_t* p) override {
        if (!subprocessor->OnGameLobbyJoinRequested(p)) return;
        DefaultLobbyProcessor::OnGameLobbyJoinRequested(p);
    }

    void OnLobbyDataUpdate(LobbyDataUpdate_t* p) override {
        if (!subprocessor->OnLobbyDataUpdate(p)) return;
        DataLobbyProcessor::OnLobbyDataUpdate(p);
    }

    void OnLobbyChatUpdate(LobbyChatUpdate_t* p) override {
        if (!subprocessor->OnLobbyChatUpdate(p)) return;
        DataLobbyProcessor::OnLobbyChatUpdate(p);
    }

    void OnLobbyKicked(LobbyKicked_t* p) override {
        if (!subprocessor->OnLobbyKicked(p)) return;
        DataLobbyProcessor::OnLobbyKicked(p);
    }

    void OnLobbyMatchList(LobbyMatchList_t* p) override {
        if (!subprocessor->OnLobbyMatchList(p)) return;
        DefaultLobbyProcessor::OnLobbyMatchList(p);
    }

    void OnLobbyGameCreated(LobbyGameCreated_t* p) override {
        if (!subprocessor->OnLobbyGameCreated(p)) return;
        DefaultLobbyProcessor::OnLobbyGameCreated(p);
    }

    void OnP2PSessionRequest(P2PSessionRequest_t* p) override {
        if (!subprocessor->OnP2PSessionRequest(p)) return;
        DefaultLobbyProcessor::OnP2PSessionRequest(p);
    }

    void OnP2PSessionConnectFail(P2PSessionConnectFail_t* p) override {
        if (!subprocessor->OnP2PSessionConnectFail(p)) return;
        DefaultLobbyProcessor::OnP2PSessionConnectFail(p);
    }

private:

    static unsigned char const CONNECT_DISABLE = 0;
    static unsigned char const CONNECT_TO_RANDOMS = 1;
    static unsigned char const CONNECT_TO_NEW_RANDOMS = 2;
    static unsigned char const CONNECT_TO_MOD_USERS = 3;

    NightreignProgramData* data = nullptr;
    AbstractLobbySubProcessor* subprocessor = nullptr;

    unsigned char connectType = CONNECT_DISABLE;

    void ProcessSessionState() override {
        if (!data->InGame()) return;
        if (currentLobby || lastLobby) {
            UpdateRaidStatus(currentLobby) || UpdateRaidStatus(lastLobby);
            return;
        }
        if (data->InHub()) {
            subprocessor->Event();
        }
    }

    bool UpdateRaidStatus(Lobby* lobby) {
        if (!lobby || lobby->owner != CSteamID(SteamConfig::instance().userId)) return false;
        if (lobby->parameters.count(SteamConfig::instance().index)) {
            const std::string desired_status = data->InHub() ? "1" : "0";
            const std::string raid_key = SteamConfig::instance().modIndex;
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
        AbstractLobbySubProcessor* previousSubprocessor = subprocessor;
        unsigned char previousConnectType = connectType;
        switch (data->GetCurrentAnimation()) {
            case 80240: connectType = CONNECT_DISABLE; break;
            case 80200: connectType = CONNECT_TO_RANDOMS; break;
            //case 80060: connectType = CONNECT_TO_NEW_RANDOMS; break;
            case 80730: connectType = CONNECT_TO_MOD_USERS; break;
        }
        if (previousConnectType != connectType) {
            switch (connectType) {
                case CONNECT_DISABLE: subprocessor = new DefaultLobbySubProcessor(); break;
                case CONNECT_TO_RANDOMS: subprocessor = new RandomLobbySubProcessor(SteamConfig::instance().filters); break;
                case CONNECT_TO_NEW_RANDOMS: break; // eheheheheheheh
                case CONNECT_TO_MOD_USERS:
                    std::map<std::string, std::string> data = SteamConfig::instance().filters;
                    data[SteamConfig::instance().modIndex] = "1";
                    subprocessor = new RandomLobbySubProcessor(data);
                    break;
            }
            std::cout << "[NightreignLobbyProcessor] Connect type changed to: " << static_cast<int>(connectType) << std::endl;
        }
        if (previousSubprocessor != subprocessor) {
            delete previousSubprocessor;
            std::cout << "[NightreignLobbyProcessor] PreviousSubprocessor been deleted" << std::endl;
        }
    }
};