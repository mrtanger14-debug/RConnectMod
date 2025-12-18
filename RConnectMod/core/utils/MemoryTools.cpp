#pragma once
#include <windows.h>
#include <psapi.h>
#include <vector>
#include <string>
#include <sstream>

HANDLE hProc = GetCurrentProcess();

class MemoryTools {

    private:

        MemoryTools() {}

    public:

        template<typename T>
        static bool Read(uintptr_t address, T& out) {
            __try {
                return address && ReadProcessMemory(hProc, (LPCVOID)address, &out, sizeof(T), nullptr);
            }
            __except (EXCEPTION_EXECUTE_HANDLER) {
                return false;
            }
        }

        static uintptr_t Pointer(uintptr_t address, const std::vector<uintptr_t>& offsets) {
            uintptr_t result = address;
            __try {
                for (uintptr_t offset : offsets) {
                    if (!result) return 0;
                    result = *(uintptr_t*)result;
                    result += offset;
                }
            } __except (EXCEPTION_EXECUTE_HANDLER) {
                return 0;
            }
            return result;
        }

        static uintptr_t RIP(uintptr_t instrAddr) {
            uint8_t* instr = (uint8_t*)instrAddr;
            if (instr[0] == 0x48 && instr[1] == 0x8B && instr[2] == 0x0D) {
                int32_t ripOffset = *(int32_t*)(instr + 3);
                return (uintptr_t)(instr + 7 + ripOffset);
            }
            if (instr[0] == 0x48 && instr[1] == 0x8B && instr[2] == 0x05) {
                int32_t ripOffset = *(int32_t*)(instr + 3);
                return (uintptr_t)(instr + 7 + ripOffset);
            }
            if (instr[0] == 0xE8) {
                int32_t ripOffset = *(int32_t*)(instr + 1);
                return (uintptr_t)(instr + 5 + ripOffset);
            }
            if (instr[0] == 0xE9) {
                int32_t ripOffset = *(int32_t*)(instr + 1);
                return (uintptr_t)(instr + 5 + ripOffset);
            }
            return 0;
        }

        static uintptr_t AOB(const std::string& signature) {
            HMODULE hMod = GetModuleHandle(nullptr);
            MODULEINFO info{};
            GetModuleInformation(hProc, hMod, &info, sizeof(info));
            auto base = (uint8_t*)info.lpBaseOfDll;
            auto size = info.SizeOfImage;
            std::vector<int> pattern;
            std::istringstream iss(signature);
            std::string s;
            while (iss >> s) pattern.push_back(s == "??" ? -1 : strtoul(s.c_str(), 0, 16));
            size_t patSize = pattern.size();
            for (size_t i = 0; i <= size - patSize; ++i) {
                bool found = true;
                for (size_t j = 0; j < patSize; ++j) {
                    if (pattern[j] != -1 && base[i + j] != pattern[j]) {
                        found = false;
                        break;
                    }
                }
                if (found) return (uintptr_t)(base + i);
            }
            return 0;
        }

        static uintptr_t GetModuleAddress(const char* module) {
            return (uintptr_t)GetModuleHandleA(module);
        }
};