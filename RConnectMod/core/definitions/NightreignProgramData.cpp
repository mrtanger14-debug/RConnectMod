#pragma once
#include "../utils/MemoryTools.cpp"
#include "../abstraction/AbstractProgramData.h"

class NightreignProgramData : public AbstractProgramData {

    protected:

        const uintptr_t MAIN_PLAYER_OFFSET = 0x174e8;

        uintptr_t SeamlessCoop = NULL;
        uintptr_t WorldCharacterManager = NULL;
        uintptr_t GameDataManager = NULL;
        //uintptr_t ClientSystemWindow = NULL;
        //uintptr_t ClientSystemFlipper = NULL;
        //uintptr_t SoloParameterRepository = NULL;
        //uintptr_t ClientSystemGameItem = NULL;
        //uintptr_t MenuManager = NULL;
        //uintptr_t SessionManager = NULL;
        //uintptr_t FieldArea = NULL;
        //uintptr_t WorldCharacterManagerDebug = NULL;
        //uintptr_t EventManager = NULL;
        //uintptr_t ClientSystemTrophy = NULL;
        //uintptr_t LockTargetManager = NULL;

    public:

        bool Init() override {
            this->SeamlessCoop = MemoryTools::GetModuleAddress("nrsc.dll");
            this->WorldCharacterManager = MemoryTools::AOB("48 8B 05 ?? ?? ?? ?? 0F 28 F1 48 85 C0");
            this->GameDataManager = MemoryTools::AOB("48 8B 0D ?? ?? ?? ?? F3 48 0F 2C C0");
            //this->ClientSystemWindow = MemoryTools::AOB("48 8B 0D ?? ?? ?? ?? 0F 5B C0 0F 5B C9");
            //this->ClientSystemFlipper = MemoryTools::AOB("48 8B 05 ?? ?? ?? ?? 48 85 C0 ?? ?? 4C 8B 05 ?? ?? ?? ?? 4C 89 44 24 60 8D 50 08 B9");
            //this->SoloParameterRepository = MemoryTools::AOB("48 8B ?? ?? ?? ?? ?? 48 85 ?? 75 ?? 48 8D ?? ?? ?? ?? ?? E8 ?? ?? ?? ?? 4C 8B ?? 4C 8D ?? ?? ?? ?? ?? BA ?? ?? ?? ?? 48 8D ?? ?? ?? ?? ?? E8 ?? ?? ?? ?? 48 8B ?? ?? ?? ?? ?? 45 33 ?? 41 8D ?? ?? E8 ?? ?? ?? ?? 48 85 ?? 0F 84 ?? ?? ?? ?? 48 8B ?? ?? ?? ?? ?? 48 8B ?? ?? ?? ?? ?? 48 8D ?? ?? ?? ?? ?? 48 89 ?? ?? 48 89");
            //this->ClientSystemGameItem = MemoryTools::AOB("8B 03 89 44 24 40 48 8B 0D ?? ?? ?? ?? 48 85 C9");
            //this->MenuManager = MemoryTools::AOB("?? 8B 3D ?? ?? ?? ?? 48 85 FF 74 32 BA ?? ?? ?? ?? 48 8D 4C 24");
            //this->SessionManager = MemoryTools::AOB("?? 8B EC 48 83 EC ?? 48 8B F9 C7 45 F0 00 00 00 00 48 8B 0D");
            //this->FieldArea = MemoryTools::AOB("48 8B 05 ?? ?? ?? ?? 48 85 C0 ?? ?? ?? ?? ?? ?? 4C 8B 68");
            //this->WorldCharacterManagerDebug = MemoryTools::AOB("40 53 48 83 EC 20 80 B9 ?? ?? ?? ?? 00 48 8B D9 7C ?? 48 8B 49 ?? 48");
            //this->EventManager = MemoryTools::AOB("4C 8B F1 C7 44 24 ?? FE FF FF FF 48 8B 0D");
            //this->ClientSystemTrophy = MemoryTools::AOB("48 8B 0D ?? ?? ?? ?? 48 85 C9 ?? ?? 48 8D 55 10");
            //this->LockTargetManager = MemoryTools::AOB("48 8B 35 ?? ?? ?? ?? 48 81 C6 ?? ?? ?? ?? 4C 8B 2D ?? ?? ?? ?? 4C 89 6C 24 ?? 4D 85 ED");
            return (
                this->SeamlessCoop &&
                this->WorldCharacterManager &&
                this->GameDataManager /*&&
                this->ClientSystemWindow &&
                this->ClientSystemFlipper &&
                this->SoloParameterRepository &&
                this->ClientSystemGameItem &&
                this->MenuManager &&
                this->SessionManager &&
                this->FieldArea &&
                this->WorldCharacterManagerDebug &&
                this->EventManager &&
                this->ClientSystemTrophy &&
                this->LockTargetManager*/
            );
        }

        void Update() override {}

        int GetCurrentAnimation() {
            int animation;
            if (MemoryTools::Read(MemoryTools::Pointer(MemoryTools::RIP(this->WorldCharacterManager), {MAIN_PLAYER_OFFSET, 0x1B8, 0x80, 0x98}), animation)) {
                return animation;
            }
            return 0; 
        }

        bool InGame() {
            int temp_dummy;
            return MemoryTools::Read(MemoryTools::Pointer(MemoryTools::RIP(this->WorldCharacterManager), { MAIN_PLAYER_OFFSET, 0x8 }), temp_dummy);
        }

        bool InHub() {
            unsigned char type;
            if (MemoryTools::Read(MemoryTools::Pointer(MemoryTools::RIP(this->GameDataManager), { 0xD8 }), type)) {
                return type == 0;
            }
            return false;
        }
};