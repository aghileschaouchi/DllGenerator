// dllmain.cpp : Defines the entry point for the DLL application.

#include "../GameHackingEngine/include/core/WinCodeCave.h"
#include "../GameHackingEngine/include/helper/Helper.h"

//#include "NoClip.hpp"
#include "InfiniteAmmo.hpp"

namespace
{
	const std::string baseModuleName("Tomb2.exe");
	const std::string messageBoxMessage("Injection location address");
	const std::string messageBoxMessageDetach("Detaching in");
	const std::string messageBoxMessageAttach("Attaching in");
}

void initiateHooks()
{
	////Noclip
	//collisionInsAddress = ghe::winCodeCave::findPattern(baseModuleName, noClip::signature, noClip::mask);
	//collisionJmpBack = collisionInsAddress + static_cast<DWORD>(noClip::mask.size());
	////for debug
 //  helper::debugMessageBox<decltype(collisionInsAddress)>(messageBoxMessage, collisionInsAddress);
 //  helper::debugMessageBox<decltype(collisionJmpBack)>(messageBoxMessage, collisionJmpBack);
  // ghe::winCodeCave::codeCave1<ghe::winCodeCave::ASM::JMP>(static_cast<uintptr_t>(collisionInsAddress), reinterpret_cast<DWORD>(activateNoClip),
		//static_cast<DWORD>(noClip::mask.size()));

	//Infinite ammo
	//shotgun and grenade launcher
	bigWeaponsDecAmmoAddress = ghe::winCodeCave::findPattern(baseModuleName, infiniteAmmo::bigWeapons::signature, infiniteAmmo::bigWeapons::mask);
	bigWeaponsDecAmmoJmpBack = bigWeaponsDecAmmoAddress + static_cast<DWORD>(infiniteAmmo::bigWeapons::mask.size());
	ghe::winCodeCave::fillWithNop(static_cast<uintptr_t>(bigWeaponsDecAmmoAddress), static_cast<DWORD>(infiniteAmmo::bigWeapons::mask.size()));
	//uzi auto pistols m16
	weaponsDecAmmoAddress = ghe::winCodeCave::findPattern(baseModuleName, infiniteAmmo::weapons::signature, infiniteAmmo::weapons::mask);
	weaponsDecAmmoJmpBack = weaponsDecAmmoAddress + static_cast<DWORD>(infiniteAmmo::weapons::mask.size());
	ghe::winCodeCave::fillWithNop(static_cast<uintptr_t>(weaponsDecAmmoAddress), static_cast<DWORD>(infiniteAmmo::weapons::mask.size()));
	//medic packs and flares
	flaresMedicsDecAmmoAddress = ghe::winCodeCave::findPattern(baseModuleName, infiniteAmmo::flaresMedics::signature, infiniteAmmo::flaresMedics::mask);
	flaresMedicsDecAmmoJmpBack = flaresMedicsDecAmmoAddress + static_cast<DWORD>(infiniteAmmo::flaresMedics::mask.size());
	ghe::winCodeCave::fillWithNop(static_cast<uintptr_t>(flaresMedicsDecAmmoAddress), static_cast<DWORD>(infiniteAmmo::flaresMedics::mask.size()));
	//harpoon
	harpoonDecAmmoAddress = ghe::winCodeCave::findPattern(baseModuleName, infiniteAmmo::harpoon::signature, infiniteAmmo::harpoon::mask);
	harpoonDecAmmoJmpBack = harpoonDecAmmoAddress + static_cast<DWORD>(infiniteAmmo::harpoon::mask.size());
	ghe::winCodeCave::fillWithNop(static_cast<uintptr_t>(harpoonDecAmmoAddress), static_cast<DWORD>(infiniteAmmo::harpoon::mask.size()));
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

