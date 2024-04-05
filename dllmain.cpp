// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "../GameHackingEngine/include/core/WinCodeCave.h"
#include "../GameHackingEngine/include/helper/Helper.h"

#include <string>

DWORD collisionJmpBack = 0;
DWORD _instrucitonAddress = 0x004276C9;

extern "C"
{
    __declspec(naked) void activateNoClip()
    {
        //the jump should ignore the 3 other instructions but we kept them to better understanding codecaving
        //since our sig scan has "4" as a mask, starting from the second instruction (+2) and countring 4 bytes, all those inscrutions will be erased
        //check back the video to better understand
        __asm JMP [_instrucitonAddress]
        __asm PUSH EDI
        __asm PUSH ESI
        __asm JMP[collisionJmpBack]
    }
    
    __declspec(naked) void desactivateNoClip()
    {
        //replace by JNE
        __asm JMP [_instrucitonAddress]
        __asm PUSH EDI
        __asm PUSH ESI
        __asm JMP[collisionJmpBack]
    }
}

const enum HookTypes { DESACTIVATE, ACTIVATE };
template<int TYPE = ACTIVATE>
void initiateHooks()
{
    DWORD collisionInsAddress = ghe::codeCave::findPattern(baseModuleName, "\xA8\x04\x75\x24", "xxxx") + 2;
    collisionJmpBack = collisionInsAddress + 0x4;

    static const std::string messageBoxMessage("Injection location address : ");
    helper::debugMessageBox<decltype(collisionInsAddress)>(messageBoxMessage, collisionInsAddress);

    if constexpr (ACTIVATE)
    {
        ghe::codeCave::jumpCodeCave(reinterpret_cast<BYTE*>(collisionInsAddress), reinterpret_cast<DWORD>(activateNoClip), 4);
    }

    else if constexpr (DESACTIVATE)
    {
        ghe::codeCave::jumpCodeCave(reinterpret_cast<BYTE*>(collisionInsAddress), reinterpret_cast<DWORD>(desactivateNoClip), 4);
    }
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        initiateHooks<ACTIVATE>();
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        initiateHooks<DESACTIVATE>();
        break;
    }
    return TRUE;
}

