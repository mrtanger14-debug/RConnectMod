#include <chrono>
#include <iostream>
#include <steam_api.h>
#include "abstraction/AbstractLobbyProcessor.h"

class LobbyHandler
{
public:
    
    LobbyHandler() :
        m_CallbackLobbyCreated(this, &LobbyHandler::OnLobbyCreated),
        m_CallbackLobbyEnter(this, &LobbyHandler::OnLobbyEnter),
        m_CallbackLobbyDataUpdate(this, &LobbyHandler::OnLobbyDataUpdate),
        m_CallbackGameLobbyJoinRequested(this, &LobbyHandler::OnGameLobbyJoinRequested),
        m_CallbackLobbyChatUpdate(this, &LobbyHandler::OnLobbyChatUpdate),
        m_CallbackLobbyKicked(this, &LobbyHandler::OnLobbyKicked),
        m_CallbackLobbyMatchList(this, &LobbyHandler::OnLobbyMatchList),
        m_CallbackLobbyGameCreated(this, &LobbyHandler::OnLobbyGameCreated)
    {}

    void LobbyProcessorUpdate() {
        using clock = std::chrono::high_resolution_clock;
        static auto last = clock::now();
        auto now = clock::now();
        float dt = std::chrono::duration<float>(now - last).count();
        last = now;
        if (this->processor) this->processor->ProcessorUpdate(dt);
    }

    void SetProcessor(AbstractLobbyProcessor* ptr_processor) {
        if (this->processor) {
            delete this->processor;
            this->processor = nullptr;
        }
        if (ptr_processor) this->processor = ptr_processor;
    }

private:

    AbstractLobbyProcessor* processor = nullptr;

    void OnLobbyCreated(LobbyCreated_t* p) {
        if (this->processor) this->processor->OnLobbyCreated(p);
    }

    void OnLobbyEnter(LobbyEnter_t* p) {
        if (this->processor) this->processor->OnLobbyEnter(p);
    }

    void OnLobbyDataUpdate(LobbyDataUpdate_t* p) {
        if (this->processor) this->processor->OnLobbyDataUpdate(p);
    }

    void OnGameLobbyJoinRequested(GameLobbyJoinRequested_t* p) {
        if (this->processor) this->processor->OnGameLobbyJoinRequested(p);
    }

    void OnLobbyChatUpdate(LobbyChatUpdate_t* p) {
        if (this->processor) this->processor->OnLobbyChatUpdate(p);
    }

    void OnLobbyKicked(LobbyKicked_t* p) {
        if (this->processor) this->processor->OnLobbyKicked(p);
    }

    void OnLobbyMatchList(LobbyMatchList_t* p) {
        if (this->processor) this->processor->OnLobbyMatchList(p);
    }

    void OnLobbyGameCreated(LobbyGameCreated_t* p) {
        if (this->processor) this->processor->OnLobbyGameCreated(p);
    }

    CCallback<LobbyHandler, LobbyCreated_t> m_CallbackLobbyCreated;
    CCallback<LobbyHandler, LobbyEnter_t> m_CallbackLobbyEnter;
    CCallback<LobbyHandler, LobbyDataUpdate_t> m_CallbackLobbyDataUpdate;
    CCallback<LobbyHandler, GameLobbyJoinRequested_t> m_CallbackGameLobbyJoinRequested;
    CCallback<LobbyHandler, LobbyChatUpdate_t> m_CallbackLobbyChatUpdate;
    CCallback<LobbyHandler, LobbyKicked_t> m_CallbackLobbyKicked;
    CCallback<LobbyHandler, LobbyMatchList_t> m_CallbackLobbyMatchList;
    CCallback<LobbyHandler, LobbyGameCreated_t> m_CallbackLobbyGameCreated;
};