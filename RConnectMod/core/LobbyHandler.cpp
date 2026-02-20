#include <mutex>
#include <deque>
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
            ProcessData(lobbyCreatedEvents, lobbyCreatedMutex, &AbstractLobbyProcessor::OnLobbyCreated);
            ProcessData(lobbyEnterEvents, lobbyEnterMutex, &AbstractLobbyProcessor::OnLobbyEnter);
            ProcessData(gameLobbyJoinRequestedEvents, gameLobbyJoinRequestedMutex, &AbstractLobbyProcessor::OnGameLobbyJoinRequested);
            ProcessData(lobbyDataUpdateEvents, lobbyDataUpdateMutex, &AbstractLobbyProcessor::OnLobbyDataUpdate);
            ProcessData(lobbyChatUpdateEvents, lobbyChatUpdateMutex, &AbstractLobbyProcessor::OnLobbyChatUpdate);
            ProcessData(lobbyKickedEvents, lobbyKickedMutex, &AbstractLobbyProcessor::OnLobbyKicked);
            ProcessData(lobbyMatchListEvents, lobbyMatchListMutex, &AbstractLobbyProcessor::OnLobbyMatchList);
            ProcessData(lobbyGameCreatedEvents, lobbyGameCreatedMutex, &AbstractLobbyProcessor::OnLobbyGameCreated);
            ProcessData(p2pSessionRequestEvents, p2pSessionRequestMutex, &AbstractLobbyProcessor::OnP2PSessionRequest);
            ProcessData(p2pSessionConnectFailEvents, p2pSessionConnectFailMutex, &AbstractLobbyProcessor::OnP2PSessionConnectFail);
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

    template<typename T>
    using SafeQueue = std::deque<T>;

    template<typename T>
    void ProcessData(SafeQueue<T>& queue, std::mutex& mtx, void(AbstractLobbyProcessor::*handler)(T*)) {
        SafeQueue<T> local;
        {
            std::lock_guard<std::mutex> lock(mtx);
            local.swap(queue);
        }
        for (auto& e : local) (this->processor->*handler)(&e);
    }

    SafeQueue<LobbyCreated_t> lobbyCreatedEvents;
    std::mutex lobbyCreatedMutex;

    SafeQueue<LobbyEnter_t> lobbyEnterEvents;
    std::mutex lobbyEnterMutex;

    SafeQueue<GameLobbyJoinRequested_t> gameLobbyJoinRequestedEvents;
    std::mutex gameLobbyJoinRequestedMutex;

    SafeQueue<LobbyDataUpdate_t> lobbyDataUpdateEvents;
    std::mutex lobbyDataUpdateMutex;

    SafeQueue<LobbyChatUpdate_t> lobbyChatUpdateEvents;
    std::mutex lobbyChatUpdateMutex;

    SafeQueue<LobbyKicked_t> lobbyKickedEvents;
    std::mutex lobbyKickedMutex;

    SafeQueue<LobbyMatchList_t> lobbyMatchListEvents;
    std::mutex lobbyMatchListMutex;

    SafeQueue<LobbyGameCreated_t> lobbyGameCreatedEvents;
    std::mutex lobbyGameCreatedMutex;

    SafeQueue<P2PSessionRequest_t> p2pSessionRequestEvents;
    std::mutex p2pSessionRequestMutex;

    SafeQueue<P2PSessionConnectFail_t> p2pSessionConnectFailEvents;
    std::mutex p2pSessionConnectFailMutex;

    void OnLobbyCreated(LobbyCreated_t* p) { AddEvent(lobbyCreatedEvents, lobbyCreatedMutex, *p); }
    void OnLobbyEnter(LobbyEnter_t* p) { AddEvent(lobbyEnterEvents, lobbyEnterMutex, *p); }
    void OnGameLobbyJoinRequested(GameLobbyJoinRequested_t* p) { AddEvent(gameLobbyJoinRequestedEvents, gameLobbyJoinRequestedMutex, *p); }
    void OnLobbyDataUpdate(LobbyDataUpdate_t* p) { AddEvent(lobbyDataUpdateEvents, lobbyDataUpdateMutex, *p); }
    void OnLobbyChatUpdate(LobbyChatUpdate_t* p) { AddEvent(lobbyChatUpdateEvents, lobbyChatUpdateMutex, *p); }
    void OnLobbyKicked(LobbyKicked_t* p) { AddEvent(lobbyKickedEvents, lobbyKickedMutex, *p); }
    void OnLobbyMatchList(LobbyMatchList_t* p) { AddEvent(lobbyMatchListEvents, lobbyMatchListMutex, *p); }
    void OnLobbyGameCreated(LobbyGameCreated_t* p) { AddEvent(lobbyGameCreatedEvents, lobbyGameCreatedMutex, *p); }
    void OnP2PSessionRequest(P2PSessionRequest_t* p) { AddEvent(p2pSessionRequestEvents, p2pSessionRequestMutex, *p); }
    void OnP2PSessionConnectFail(P2PSessionConnectFail_t* p) { AddEvent(p2pSessionConnectFailEvents, p2pSessionConnectFailMutex, *p); }

    template<typename T>
    void AddEvent(SafeQueue<T>& queue, std::mutex& mtx, const T& e) {
        std::lock_guard<std::mutex> lock(mtx);
        queue.push_back(e);
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
