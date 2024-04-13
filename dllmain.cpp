// dllmain.cpp : Defines the entry point for the DLL application.

#include "../GameHackingEngine/include/core/WinCodeCave.h"
#include "../GameHackingEngine/include/helper/Helper.h"

namespace
{
	const std::string messageBoxMessage("Injection location address");
	const std::string messageBoxMessageDetach("Detaching in");
	const std::string messageBoxMessageAttach("Attaching in");
	const enum HookTypes { DESACTIVATE, ACTIVATE };

	const std::string baseModuleName("Tomb2.exe");
	const std::string signature = "\x83\xC4\x04\xA8\x04\x75\x24";
	const std::string mask = "xxxxxxx";
	const DWORD _jumpAddress = 0x004276C9;
}

DWORD collisionJmpBack = 0;
DWORD collisionInsAddress = 0;

extern "C"
{
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

	__declspec(naked) void activateNoClip()
	{
		//the jump should ignore the 3 other instructions but we kept them to better understanding codecaving
		//since our sig scan has "4" as a mask, starting from the second instruction (+2) and countring 4 bytes, all those inscrutions will be erased
		//check back the video to better understand
		__asm ADD ESP, 4
		__asm TEST AL, 04
		__asm JMP[_jumpAddress]
	}

	__declspec(naked) void desactivateNoClip()
	{
		__asm ADD ESP, 4
		__asm TEST AL, 04
		__asm JNE SHORT _jumpAddress
	}
}

template<int TYPE = ACTIVATE>
void initiateHooks()
{
	collisionInsAddress = ghe::winCodeCave::findPattern(baseModuleName, signature, mask);
	collisionJmpBack = collisionInsAddress + static_cast<DWORD>(mask.size());

	//for debug
   //helper::debugMessageBox<decltype(collisionInsAddress)>(messageBoxMessage, collisionInsAddress);

	if constexpr (TYPE == ACTIVATE)
	{
		ghe::winCodeCave::codeCave1<ghe::winCodeCave::ASM::JMP>(static_cast<uintptr_t>(collisionInsAddress), reinterpret_cast<DWORD>(activateNoClip),
			static_cast<DWORD>(mask.size()));
	}

	else if constexpr (TYPE == DESACTIVATE)
	{
		ghe::winCodeCave::codeCave1<ghe::winCodeCave::ASM::JMP>(static_cast<uintptr_t>(collisionInsAddress), reinterpret_cast<DWORD>(desactivateNoClip),
			static_cast<DWORD>(mask.size()));
	}
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		//helper::debugMessageBox<decltype(collisionInsAddress)>(messageBoxMessageAttach, collisionInsAddress); 
		initiateHooks<ACTIVATE>();
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		//helper::debugMessageBox<decltype(collisionInsAddress)>(messageBoxMessageDetach, collisionInsAddress);
		initiateHooks<DESACTIVATE>();
		break;
	}
	return TRUE;
}

