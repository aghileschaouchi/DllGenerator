#pragma once

#include "../GameHackingEngine/include/core/WinCodeCave.h"
#include "../GameHackingEngine/include/helper/Helper.h"

namespace noClip
{
	const std::string signature = "\x83\xC4\x04\xA8\x04\x75\x24";
	const std::string mask = "xxxxxxx";
}

DWORD collisionJmpBack = 0;
DWORD collisionInsAddress = 0;
const DWORD _bypassCollisionAddress = 0x004276C9;
const DWORD _collisionBlockAddress = 0x004276A3;

extern "C"
{
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

	__declspec(naked) void activateNoClip()
	{
		//the jump should ignore the 3 other instructions but we kept them to better understanding codecaving
		//since our sig scan has "4" as a mask, starting from the second instruction (+2) and counting 4 bytes, all those inscrutions will be erased
		//check back the video to better understand
		__asm ADD ESP, 4
		__asm TEST AL, 04
		__asm JMP[_bypassCollisionAddress]
	}

	__declspec(naked) void desactivateNoClip()
	{
		__asm ADD ESP, 4
		__asm TEST AL, 04
		__asm JMP[_collisionBlockAddress]
	}
}

