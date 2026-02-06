#pragma once
#include <steam_api.h>

class AbstractLobbySubProcessor {
    public:
        virtual void Event() = 0;
        virtual bool ProcessorUpdate(float deltaTime) = 0;
        virtual bool OnLobbyCreated(LobbyCreated_t* p) = 0;
        virtual bool OnLobbyEnter(LobbyEnter_t* p) = 0;
        virtual bool OnGameLobbyJoinRequested(GameLobbyJoinRequested_t* p) = 0;
        virtual bool OnLobbyDataUpdate(LobbyDataUpdate_t* p) = 0;
        virtual bool OnLobbyChatUpdate(LobbyChatUpdate_t* p) = 0;
        virtual bool OnLobbyKicked(LobbyKicked_t* p) = 0;
        virtual bool OnLobbyMatchList(LobbyMatchList_t* p) = 0;
        virtual bool OnLobbyGameCreated(LobbyGameCreated_t* p) = 0;
        virtual bool OnP2PSessionRequest(P2PSessionRequest_t* p) = 0;
        virtual bool OnP2PSessionConnectFail(P2PSessionConnectFail_t* p) = 0;
};