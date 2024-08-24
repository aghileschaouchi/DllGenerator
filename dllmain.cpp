// dllmain.cpp : Defines the entry point for the DLL application.

#include "../GameHackingEngine/include/core/WinCodeCave.h"
#include "../GameHackingEngine/include/helper/Helper.h"

#include "NoClip.hpp"
//#include "InfiniteAmmo.hpp"

namespace
{
	const std::string baseModuleName("Tomb2.exe");
	const std::string messageBoxMessage("Injection location address");
	const std::string messageBoxMessageDetach("Detaching in");
	const std::string messageBoxMessageAttach("Attaching in");
}

void initiateHooks()
{
	//Noclip
	collisionInsAddress = ghe::winCodeCave::findPattern(baseModuleName, noClip::signature, noClip::mask);
	collisionJmpBack = collisionInsAddress + static_cast<DWORD>(noClip::mask.size());
	//for debug
   helper::debugMessageBox<decltype(collisionInsAddress)>(messageBoxMessage, collisionInsAddress);
   helper::debugMessageBox<decltype(collisionJmpBack)>(messageBoxMessage, collisionJmpBack);
   ghe::winCodeCave::codeCave1<ghe::winCodeCave::ASM::JMP>(static_cast<uintptr_t>(collisionInsAddress), reinterpret_cast<DWORD>(activateNoClip),
		static_cast<DWORD>(noClip::mask.size()));

	//Infinite ammo
	/*decAmmoAddress = ghe::winCodeCave::findPattern(baseModuleName, infiniteAmmo::signature, infiniteAmmo::mask);
	decAmmoJmpBack = decAmmoAddress + static_cast<DWORD>(infiniteAmmo::mask.size());
	//for debug
	helper::debugMessageBox<decltype(decAmmoAddress)>(messageBoxMessage, decAmmoAddress);
	helper::debugMessageBox<decltype(decAmmoJmpBack)>(messageBoxMessage, decAmmoJmpBack);*/

	/*ghe::winCodeCave::codeCave1<ghe::winCodeCave::ASM::JMP>(static_cast<uintptr_t>(decAmmoAddress), reinterpret_cast<DWORD>(activateInfiniteAmmo),
		static_cast<DWORD>(infiniteAmmo::mask.size()));*/
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
		initiateHooks();
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		//helper::debugMessageBox<decltype(collisionInsAddress)>(messageBoxMessageDetach, collisionInsAddress);
		//terminate and free process
		break;
	}
	return TRUE;
}

