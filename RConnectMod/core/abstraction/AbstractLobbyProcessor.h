#pragma once
#include <steam_api.h>

class AbstractLobbyProcessor {
    public:
        virtual void ProcessorUpdate(float deltaTime) = 0;
        virtual void OnLobbyCreated(LobbyCreated_t* p) = 0;
        virtual void OnLobbyEnter(LobbyEnter_t* p) = 0;
        virtual void OnLobbyDataUpdate(LobbyDataUpdate_t* p) = 0;
        virtual void OnGameLobbyJoinRequested(GameLobbyJoinRequested_t* p) = 0;
        virtual void OnLobbyChatUpdate(LobbyChatUpdate_t* p) = 0;
        virtual void OnLobbyKicked(LobbyKicked_t* p) = 0;
        virtual void OnLobbyMatchList(LobbyMatchList_t* p) = 0;
        virtual void OnLobbyGameCreated(LobbyGameCreated_t* p) = 0;
};