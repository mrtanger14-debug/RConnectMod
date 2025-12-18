#pragma once
#include <vector>
#include <algorithm>
#include <steam_api.h>
#include "DefaultLobbyProcessor.cpp"
#include "../data/Lobby.cpp"

class DataLobbyProcessor : public DefaultLobbyProcessor {

    public:

        ~DataLobbyProcessor() {
            delete currentLobby;
            delete lastLobby;
        }

        void ProcessorUpdate(float deltaTime) override {
            updateTimer += deltaTime;
            if (updateTimer < PROCESSOR_UPDATE_TIME) {
                return;
            }
            DefaultLobbyProcessor::ProcessorUpdate(updateTimer);
            UpdateSessionState();
            ProcessSessionState();
            updateTimer = 0.0f;
        }

        void OnLobbyCreated(LobbyCreated_t* p) override {
            DefaultLobbyProcessor::OnLobbyCreated(p);
            if (p->m_eResult == k_EResultOK) {
                CSteamID lobby(p->m_ulSteamIDLobby);
                CSteamID user = SteamUser()->GetSteamID();
                Lobby* newLobby = new Lobby(user, lobby, { user }, {});
                delete lastLobby;
                lastLobby = currentLobby;
                currentLobby = newLobby;
            }
        }

        void OnLobbyEnter(LobbyEnter_t* p) override {
            DefaultLobbyProcessor::OnLobbyEnter(p);
            if (p->m_EChatRoomEnterResponse == k_EChatRoomEnterResponseSuccess) {
                CSteamID lobbyID(p->m_ulSteamIDLobby);
                CSteamID ownerID = SteamMatchmaking()->GetLobbyOwner(lobbyID);
                if (currentLobby && currentLobby->lobby == lobbyID) {
                    return;
                }
                int count = SteamMatchmaking()->GetNumLobbyMembers(lobbyID);
                std::vector<CSteamID> members;
                for (int i = 0; i < count; ++i) members.push_back(SteamMatchmaking()->GetLobbyMemberByIndex(lobbyID, i));
                Lobby* newLobby = new Lobby(ownerID, lobbyID, members, {});
                delete lastLobby;
                lastLobby = currentLobby;
                currentLobby = newLobby;
            }
        }

        void OnLobbyDataUpdate(LobbyDataUpdate_t* p) override {
            CSteamID lobbyID(p->m_ulSteamIDLobby);
            CSteamID memberID(p->m_ulSteamIDMember);
            auto dataUpdate = [&](Lobby* lobby) {
                if (!lobby || lobby->lobby != lobbyID) return;
                lobby->parameters.clear();
                int count = SteamMatchmaking()->GetLobbyDataCount(lobby->lobby);
                for (int i = 0; i < count; ++i) {
                    char key[256], value[256];
                    if (SteamMatchmaking()->GetLobbyDataByIndex(lobby->lobby, i, key, sizeof(key), value, sizeof(value))) lobby->parameters[std::string(key)] = std::string(value);
                }
            };
            if (!p->m_bSuccess) return;
            if (lobbyID != memberID) return;
            dataUpdate(currentLobby);
            dataUpdate(lastLobby);
        }


        void OnLobbyChatUpdate(LobbyChatUpdate_t* p) override {
            DefaultLobbyProcessor::OnLobbyChatUpdate(p);
            CSteamID lobbyID(p->m_ulSteamIDLobby);
            CSteamID user(p->m_ulSteamIDUserChanged);
            EChatMemberStateChange change = (EChatMemberStateChange)p->m_rgfChatMemberStateChange;
            auto updateLobby = [&](Lobby*& lobby) {
                if (!lobby || lobby->lobby != lobbyID) return;
                if (change & k_EChatMemberStateChangeEntered) {
                    lobby->members.push_back(user);
                    return;
                }
                if (change & (
                    k_EChatMemberStateChangeLeft |
                    k_EChatMemberStateChangeKicked |
                    k_EChatMemberStateChangeBanned |
                    k_EChatMemberStateChangeDisconnected
                )) {
                        if (user == SteamUser()->GetSteamID()) {
                            delete lobby;
                            lobby = nullptr;
                            return;
                        }
                        auto& members = lobby->members;
                        members.erase(std::remove(members.begin(), members.end(), user), members.end());
                        if (lobby->owner == user) lobby->owner = SteamMatchmaking()->GetLobbyOwner(lobby->lobby);
                   }
            };
            updateLobby(currentLobby);
            updateLobby(lastLobby);
        }

        void OnLobbyKicked(LobbyKicked_t* p) override {
            DefaultLobbyProcessor::OnLobbyKicked(p);
            CSteamID kickedLobbyID(p->m_ulSteamIDLobby);
            auto kicked = [&](Lobby*& lobby) {
                if (lobby && lobby->lobby == kickedLobbyID) {
                    delete lobby;
                    lobby = nullptr;
                }
            };
            kicked(currentLobby);
            kicked(lastLobby);
        }

    protected:

        Lobby* lastLobby = nullptr;
        Lobby* currentLobby = nullptr;

        virtual void ProcessSessionState() = 0;

    private:

        const float PROCESSOR_UPDATE_TIME = 5.0f;
        float updateTimer = 0.0f;

        void UpdateSessionState() {
            auto updateLobbyState = [&](Lobby*& lobby) {
                if (lobby) {
                    if (!SteamMatchmaking()->GetLobbyMemberByIndex(lobby->lobby, 0).IsValid()) {
                        delete lobby;
                        lobby = nullptr;
                    }
                }
            };
            updateLobbyState(currentLobby);
            updateLobbyState(lastLobby);
            if (!currentLobby && lastLobby){
                currentLobby = lastLobby;
                lastLobby = nullptr;
            }
        }
};