#pragma once
#include <iostream>
#include <steam_api.h>
#include "../abstraction/AbstractLobbyProcessor.h"

class DefaultLobbyProcessor : public AbstractLobbyProcessor {

    public:

        void ProcessorUpdate(float deltaTime) override { 
            //std::cout << "[PLUGIN] Processor Update called with deltaTime: " << deltaTime << std::endl;
        }

        void OnLobbyCreated(LobbyCreated_t* p) override {
            CSteamID lobby(p->m_ulSteamIDLobby);
            std::cout << "[PLUGIN] lobby Created. ID: " << lobby.ConvertToUint64() << ", Result: " << p->m_eResult << std::endl;
        }

        void OnLobbyEnter(LobbyEnter_t* p) override {
            CSteamID lobby(p->m_ulSteamIDLobby);
            std::cout << "[PLUGIN] lobby Entered. ID: " << lobby.ConvertToUint64() << ", Result: " << p->m_EChatRoomEnterResponse << std::endl;
        }

        void OnLobbyDataUpdate(LobbyDataUpdate_t* p) override {
            CSteamID lobby(p->m_ulSteamIDLobby);
            CSteamID member(p->m_ulSteamIDMember);
            std::cout << "[PLUGIN] lobby Data Updated. Lobby ID: " << lobby.ConvertToUint64() << ", Success: " << p->m_bSuccess << ", Member ID: " << member.ConvertToUint64() << std::endl;
        }

        void OnGameLobbyJoinRequested(GameLobbyJoinRequested_t* p) override {
            CSteamID lobby(p->m_steamIDLobby);
            CSteamID friendID(p->m_steamIDFriend);
            std::cout << "[PLUGIN] lobby Join Requested. From Friend: " << friendID.ConvertToUint64() << ", Lobby ID: " << lobby.ConvertToUint64() << ".\n";
        }

        void OnLobbyChatUpdate(LobbyChatUpdate_t* p) override {
                CSteamID user = CSteamID(p->m_ulSteamIDUserChanged);
                EChatMemberStateChange change = (EChatMemberStateChange)p->m_rgfChatMemberStateChange;
                std::cout << "[PLUGIN] Lobby ID: " << p->m_ulSteamIDLobby << std::endl;
                std::cout << "[PLUGIN] User ID: " << user.ConvertToUint64() << std::endl;
                if (change & k_EChatMemberStateChangeEntered) {
                    std::cout << "[PLUGIN] event User Entered Lobby" << std::endl;
                }
                if (change & k_EChatMemberStateChangeLeft) {
                    std::cout << "[PLUGIN] event User Left Lobby" << std::endl;
                }
                if (change & k_EChatMemberStateChangeKicked) {
                    std::cout << "[PLUGIN] event User Kicked from Lobby" << std::endl;
                }
                if (change & k_EChatMemberStateChangeBanned) {
                    std::cout << "[PLUGIN] event User Banned from Lobby" << std::endl;
                }
                if (change & k_EChatMemberStateChangeDisconnected) {
                    std::cout << "[PLUGIN] event User Disconnected from Lobby" << std::endl;
                }
            }

        void OnLobbyKicked(LobbyKicked_t* p) override {
            CSteamID lobby(p->m_ulSteamIDLobby);
            CSteamID kicker(p->m_ulSteamIDAdmin);
            std::cout << "[PLUGIN] lobby Kicked! Lobby ID: " << lobby.ConvertToUint64() << ", Kicker ID: " << kicker.ConvertToUint64() << ", Was Kicked: " << p->m_bKickedDueToDisconnect << std::endl;
        }

        void OnLobbyMatchList(LobbyMatchList_t* p) override {
            std::cout << "[PLUGIN] lobby Match list received. Found: " << p->m_nLobbiesMatching << " lobbies.\n";
        }

        void OnLobbyGameCreated(LobbyGameCreated_t* p) override {
            std::cout << "[PLUGIN] lobby Game Created! Lobby ID: " << p->m_ulSteamIDLobby << ", Server ID: " << p->m_ulSteamIDGameServer << ", IP: " << p->m_unIP << ", Port: " << p->m_usPort << std::endl;
        }
};