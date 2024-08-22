// dllmain.cpp : Defines the entry point for the DLL application.

#include "NoClip.hpp"

namespace
{
	const std::string messageBoxMessage("Injection location address");
	const std::string messageBoxMessageDetach("Detaching in");
	const std::string messageBoxMessageAttach("Attaching in");
	const enum HookTypes { DESACTIVATE, ACTIVATE };
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

