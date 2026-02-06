#include <chrono>
#include <vector>
#include <iostream>
#include <steam_api.h>
#include "abstraction/AbstractLobbyProcessor.h"

class LobbyHandler
{
public:
    
    LobbyHandler() :
        m_CallbackLobbyCreated(this, &LobbyHandler::OnLobbyCreated),
        m_CallbackLobbyEnter(this, &LobbyHandler::OnLobbyEnter),
        m_CallbackGameLobbyJoinRequested(this, &LobbyHandler::OnGameLobbyJoinRequested),
        m_CallbackLobbyDataUpdate(this, &LobbyHandler::OnLobbyDataUpdate),
        m_CallbackLobbyChatUpdate(this, &LobbyHandler::OnLobbyChatUpdate),
        m_CallbackLobbyKicked(this, &LobbyHandler::OnLobbyKicked),
        m_CallbackLobbyMatchList(this, &LobbyHandler::OnLobbyMatchList),
        m_CallbackLobbyGameCreated(this, &LobbyHandler::OnLobbyGameCreated),
        m_CallbackP2PSessionRequest(this, &LobbyHandler::OnP2PSessionRequest),
        m_CallbackP2PSessionConnectFail(this, &LobbyHandler::OnP2PSessionConnectFail)
    {}

    void LobbyProcessorUpdate() {
        using clock = std::chrono::high_resolution_clock;
        static auto last = clock::now();
        auto now = clock::now();
        float dt = std::chrono::duration<float>(now - last).count();
        last = now;
        if (this->processor) {
            ProcessData(lobbyCreatedEvents, &AbstractLobbyProcessor::OnLobbyCreated);
            ProcessData(lobbyEnterEvents, &AbstractLobbyProcessor::OnLobbyEnter);
            ProcessData(gameLobbyJoinRequestedEvents, &AbstractLobbyProcessor::OnGameLobbyJoinRequested);
            ProcessData(lobbyDataUpdateEvents, &AbstractLobbyProcessor::OnLobbyDataUpdate);
            ProcessData(lobbyChatUpdateEvents, &AbstractLobbyProcessor::OnLobbyChatUpdate);
            ProcessData(lobbyKickedEvents, &AbstractLobbyProcessor::OnLobbyKicked);
            ProcessData(lobbyMatchListEvents, &AbstractLobbyProcessor::OnLobbyMatchList);
            ProcessData(lobbyGameCreatedEvents, &AbstractLobbyProcessor::OnLobbyGameCreated);
            ProcessData(p2pSessionRequestEvents, &AbstractLobbyProcessor::OnP2PSessionRequest);
            ProcessData(p2pSessionConnectFailEvents, &AbstractLobbyProcessor::OnP2PSessionConnectFail);
            this->processor->ProcessorUpdate(dt);
        }
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

    std::vector<LobbyCreated_t> lobbyCreatedEvents;
    std::vector<LobbyEnter_t> lobbyEnterEvents;
    std::vector<GameLobbyJoinRequested_t> gameLobbyJoinRequestedEvents;
    std::vector<LobbyDataUpdate_t> lobbyDataUpdateEvents;
    std::vector<LobbyChatUpdate_t> lobbyChatUpdateEvents;
    std::vector<LobbyKicked_t> lobbyKickedEvents;
    std::vector<LobbyMatchList_t> lobbyMatchListEvents;
    std::vector<LobbyGameCreated_t> lobbyGameCreatedEvents;
    std::vector<P2PSessionRequest_t> p2pSessionRequestEvents;
    std::vector<P2PSessionConnectFail_t> p2pSessionConnectFailEvents;

    template<typename T, typename Func>
    void ProcessData(std::vector<T>& events, Func handler) {
        while (!events.empty()) {
            T data = events.front();
            events.erase(events.begin());
            (this->processor->*handler)(&data);
        }
    }

    void OnLobbyCreated(LobbyCreated_t* p) {
        lobbyCreatedEvents.push_back(*p);
    }

    void OnLobbyEnter(LobbyEnter_t* p) {
        lobbyEnterEvents.push_back(*p);
    }

    void OnGameLobbyJoinRequested(GameLobbyJoinRequested_t* p) {
        gameLobbyJoinRequestedEvents.push_back(*p);
    }

    void OnLobbyDataUpdate(LobbyDataUpdate_t* p) {
        lobbyDataUpdateEvents.push_back(*p);
    }

    void OnLobbyChatUpdate(LobbyChatUpdate_t* p) {
        lobbyChatUpdateEvents.push_back(*p);
    }

    void OnLobbyKicked(LobbyKicked_t* p) {
        lobbyKickedEvents.push_back(*p);
    }

    void OnLobbyMatchList(LobbyMatchList_t* p) {
        lobbyMatchListEvents.push_back(*p);
    }

    void OnLobbyGameCreated(LobbyGameCreated_t* p) {
        lobbyGameCreatedEvents.push_back(*p);
    }

    void OnP2PSessionRequest(P2PSessionRequest_t* p) {
        p2pSessionRequestEvents.push_back(*p);
    }

    void OnP2PSessionConnectFail(P2PSessionConnectFail_t* p) {
        p2pSessionConnectFailEvents.push_back(*p);
    }

    CCallback<LobbyHandler, LobbyCreated_t> m_CallbackLobbyCreated;
    CCallback<LobbyHandler, LobbyEnter_t> m_CallbackLobbyEnter;
    CCallback<LobbyHandler, GameLobbyJoinRequested_t> m_CallbackGameLobbyJoinRequested;
    CCallback<LobbyHandler, LobbyDataUpdate_t> m_CallbackLobbyDataUpdate;
    CCallback<LobbyHandler, LobbyChatUpdate_t> m_CallbackLobbyChatUpdate;
    CCallback<LobbyHandler, LobbyKicked_t> m_CallbackLobbyKicked;
    CCallback<LobbyHandler, LobbyMatchList_t> m_CallbackLobbyMatchList;
    CCallback<LobbyHandler, LobbyGameCreated_t> m_CallbackLobbyGameCreated;
    CCallback<LobbyHandler, P2PSessionRequest_t> m_CallbackP2PSessionRequest;
    CCallback<LobbyHandler, P2PSessionConnectFail_t> m_CallbackP2PSessionConnectFail;
};