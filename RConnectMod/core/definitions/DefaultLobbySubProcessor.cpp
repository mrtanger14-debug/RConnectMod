#pragma once
#include "../abstraction/AbstractLobbySubProcessor.h"

class DefaultLobbySubProcessor : public AbstractLobbySubProcessor {

    public:

        void Event() override { 
            return;
        }

        bool ProcessorUpdate(float deltaTime) override { 
            return true;
        }

        bool OnLobbyCreated(LobbyCreated_t* p) override {
            return true;
        }

        bool OnLobbyEnter(LobbyEnter_t* p) override {
            return true;
        }

        bool OnGameLobbyJoinRequested(GameLobbyJoinRequested_t* p) override {
            return true;
        }

        bool OnLobbyDataUpdate(LobbyDataUpdate_t* p) override {
            return true;
        }

        bool OnLobbyChatUpdate(LobbyChatUpdate_t* p) override {
            return true;
        }

        bool OnLobbyKicked(LobbyKicked_t* p) override {
            return true;
        }

        bool OnLobbyMatchList(LobbyMatchList_t* p) override {
            return true;
        }

        bool OnLobbyGameCreated(LobbyGameCreated_t* p) override {
            return true;
        }

        bool OnP2PSessionRequest(P2PSessionRequest_t* p) override {
            return true;
        }

        bool OnP2PSessionConnectFail(P2PSessionConnectFail_t* p) override {
            return true;
        }
};