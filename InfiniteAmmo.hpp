#pragma once

#include <vector>

#include "../GameHackingEngine/include/core/WinCodeCave.h"
#include "../GameHackingEngine/include/helper/Helper.h"

namespace infiniteAmmo
{
	namespace bigWeapons
	{
		const std::string signature = "\xFF\x0D\xB0\x07\x52\x00";
		const std::string mask = "xxxxx?";
	}
	
	namespace harpoon
	{
		const std::string signature = "\xFF\x0D\xAC\x07\x52\x00";
		const std::string mask = "xxxxx?";
	}

	namespace weapons
	{
		const std::string signature = "\x49\x89\x08";
		const std::string mask = "xxx";
	}
	
	namespace flaresMedics
	{
		const std::string signature = "\x66\xFF\x0C\x55\xE8\x54\x46\x00";
		const std::string mask = "xxxxxxx?";
	}


struct signatureMaskAddrs
{
	const std::string signature;
	const std::string mask;
	DWORD addr;
	DWORD jmpBackAddr;
};

//big weapons
DWORD bigWeaponsDecAmmoAddress = 0;
DWORD bigWeaponsDecAmmoJmpBack = 0;
//weapons
DWORD weaponsDecAmmoAddress = 0;
DWORD weaponsDecAmmoJmpBack = 0;
//medic packs and flares
DWORD flaresMedicsDecAmmoAddress = 0;
DWORD flaresMedicsDecAmmoJmpBack = 0;
//harpoon
DWORD harpoonDecAmmoAddress = 0;
DWORD harpoonDecAmmoJmpBack = 0;

std::vector<signatureMaskAddrs> items = { {bigWeapons::signature, bigWeapons::mask, bigWeaponsDecAmmoAddress, bigWeaponsDecAmmoJmpBack}, 
	{weapons::signature, weapons::mask, weaponsDecAmmoAddress, weaponsDecAmmoJmpBack}, 
	{flaresMedics::signature, flaresMedics::mask, flaresMedicsDecAmmoAddress, flaresMedicsDecAmmoJmpBack}, 
	{harpoon::signature, harpoon::mask, harpoonDecAmmoAddress, harpoonDecAmmoJmpBack} };

bool activateInfiniteAmmo(const std::string& baseModuleName)
{
	for (auto&& item : items)
	{
		item.addr = ghe::winCodeCave::findPattern(baseModuleName, item.signature, item.mask);
		item.jmpBackAddr = item.addr + static_cast<DWORD>(item.mask.size());
		ghe::winCodeCave::fillWithNop(static_cast<uintptr_t>(item.addr), static_cast<DWORD>(item.mask.size()));
	}
	return true;
}
}