#include <thread>
#include "core/LobbyHandler.cpp"
#include "core/InputHandler.cpp"
#include "core/utils/INIReader.cpp"

#pragma comment(lib, "Shell32.lib")

bool RESULT = TRUE;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        std::thread([]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(2500));

            INIReader settings;
            
            if (!settings.load("SeamlessCoop/RConnectMod_settings.ini")) {
                RESULT = FALSE;
                return;
            }

            if (settings.get("PROGRAM", "console") == "1") {
                AllocConsole();
                FILE *fOut; freopen_s(&fOut, "CONOUT$", "w", stdout);
                FILE *fIn;  freopen_s(&fIn, "CONIN$", "r", stdin);
                FILE *fErr; freopen_s(&fErr, "CONOUT$", "w", stderr);
                std::ios::sync_with_stdio(false);
            }

            HANDLE hProcess = GetCurrentProcess();
            static LobbyHandler lobbyHandler;
            static InputHandler inputHandler;

            lobbyHandler.SetProcessor(inputHandler.HandleInput("nightreign"));

            while (WaitForSingleObject(hProcess, 0) == WAIT_TIMEOUT) {
                lobbyHandler.LobbyProcessorUpdate();
                std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 10));
            }
        }).detach();
        break;
    }
    return RESULT;
}
