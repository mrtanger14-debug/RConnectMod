#pragma once
#include <iostream>
#include <steam_api.h>
#include "../abstraction/AbstractLobbyProcessor.h"

class DefaultLobbyProcessor : public AbstractLobbyProcessor {

    public:

        void ProcessorUpdate(float deltaTime) override { 
            //std::cout << "[DefaultLobbyProcessor] Processor Update called with deltaTime: " << deltaTime << std::endl;
        }

        void OnLobbyCreated(LobbyCreated_t* p) override {
            CSteamID lobby(p->m_ulSteamIDLobby);
            std::cout << "[DefaultLobbyProcessor] lobby Created. ID: " << lobby.ConvertToUint64() << ", Result: " << p->m_eResult << std::endl;
        }

        void OnLobbyEnter(LobbyEnter_t* p) override {
            CSteamID lobby(p->m_ulSteamIDLobby);
            std::cout << "[DefaultLobbyProcessor] lobby Entered. ID: " << lobby.ConvertToUint64() << ", Result: " << p->m_EChatRoomEnterResponse << std::endl;
        }

        void OnLobbyDataUpdate(LobbyDataUpdate_t* p) override {
            CSteamID lobby(p->m_ulSteamIDLobby);
            CSteamID member(p->m_ulSteamIDMember);
            std::cout << "[DefaultLobbyProcessor] lobby Data Updated. Lobby ID: " << lobby.ConvertToUint64() << ", Success: " << p->m_bSuccess << ", Member ID: " << member.ConvertToUint64() << std::endl;
        }

        void OnGameLobbyJoinRequested(GameLobbyJoinRequested_t* p) override {
            CSteamID lobby(p->m_steamIDLobby);
            CSteamID friendID(p->m_steamIDFriend);
            std::cout << "[DefaultLobbyProcessor] lobby Join Requested. From Friend: " << friendID.ConvertToUint64() << ", Lobby ID: " << lobby.ConvertToUint64() << ".\n";
        }

        void OnLobbyChatUpdate(LobbyChatUpdate_t* p) override {
                CSteamID user = CSteamID(p->m_ulSteamIDUserChanged);
                EChatMemberStateChange change = (EChatMemberStateChange)p->m_rgfChatMemberStateChange;
                std::cout << "[DefaultLobbyProcessor] Lobby ID: " << p->m_ulSteamIDLobby << std::endl;
                std::cout << "[DefaultLobbyProcessor] User ID: " << user.ConvertToUint64() << std::endl;
                if (change & k_EChatMemberStateChangeEntered) {
                    std::cout << "[DefaultLobbyProcessor] event User Entered Lobby" << std::endl;
                }
                if (change & k_EChatMemberStateChangeLeft) {
                    std::cout << "[DefaultLobbyProcessor] event User Left Lobby" << std::endl;
                }
                if (change & k_EChatMemberStateChangeKicked) {
                    std::cout << "[DefaultLobbyProcessor] event User Kicked from Lobby" << std::endl;
                }
                if (change & k_EChatMemberStateChangeBanned) {
                    std::cout << "[DefaultLobbyProcessor] event User Banned from Lobby" << std::endl;
                }
                if (change & k_EChatMemberStateChangeDisconnected) {
                    std::cout << "[DefaultLobbyProcessor] event User Disconnected from Lobby" << std::endl;
                }
            }

        void OnLobbyKicked(LobbyKicked_t* p) override {
            CSteamID lobby(p->m_ulSteamIDLobby);
            CSteamID kicker(p->m_ulSteamIDAdmin);
            std::cout << "[DefaultLobbyProcessor] lobby Kicked! Lobby ID: " << lobby.ConvertToUint64() << ", Kicker ID: " << kicker.ConvertToUint64() << ", Was Kicked: " << p->m_bKickedDueToDisconnect << std::endl;
        }

        void OnLobbyMatchList(LobbyMatchList_t* p) override {
            std::cout << "[DefaultLobbyProcessor] lobby Match list received. Found: " << p->m_nLobbiesMatching << " lobbies.\n";
        }

        void OnLobbyGameCreated(LobbyGameCreated_t* p) override {
            std::cout << "[DefaultLobbyProcessor] lobby Game Created! Lobby ID: " << p->m_ulSteamIDLobby << ", Server ID: " << p->m_ulSteamIDGameServer << ", IP: " << p->m_unIP << ", Port: " << p->m_usPort << std::endl;
        }

        void OnP2PSessionRequest(P2PSessionRequest_t* p) override {
            CSteamID remoteID(p->m_steamIDRemote);
            std::cout << "[DefaultLobbyProcessor] P2P Session Request from: " << remoteID.ConvertToUint64() << std::endl;
        }

        void OnP2PSessionConnectFail(P2PSessionConnectFail_t* p) override {
            CSteamID remoteID(p->m_steamIDRemote);
            std::cout << "[DefaultLobbyProcessor] P2P Connection Failed! Remote ID: " << remoteID.ConvertToUint64() << ", Error: " << (int)p->m_eP2PSessionError << std::endl;
        }
};