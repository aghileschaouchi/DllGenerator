#pragma once

#include "../GameHackingEngine/include/core/WinCodeCave.h"
#include "../GameHackingEngine/include/helper/Helper.h"

namespace infiniteAmmo
{
	const std::string signature = "\xFF\x0D\xB0\x07\x52\x00";
	const std::string mask = "xxxxx?";
}

DWORD decAmmoJmpBack = 0;
DWORD decAmmoAddress = 0;
const DWORD _decAmmoInsAddress = 0x0042C5D9;
const DWORD _grenadeLauncherAmmoAddress = 0x005207;

extern "C"
{
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

	__declspec(naked) void activateInfiniteAmmo()
	{
		__asm INC[_grenadeLauncherAmmoAddress];
		__asm JMP[decAmmoJmpBack]
	}
}

