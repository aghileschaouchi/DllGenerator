#pragma once

#include <unordered_map>
#include <vector>

#include "../GameHackingEngine/include/core/WinCodeCave.h"
#include "../GameHackingEngine/include/helper/Helper.h"

namespace speedHack
{
	const std::unordered_map<unsigned int, DWORD> speedLevels = { {0, 0x0E}, {1, 0x0D}, {2, 0x0C}, {3, 0x0B}, {4, 0x0A}, {5, 0x09} };
	namespace ground
	{
		const std::string xSignature = "\xC1\xF8\x0E\x03\xF0";
		const std::string xMask = "xxxxx";
		const DWORD xDefaultValue = 0x0E; //decrease to speed up
		const std::string ySignature = "\xC1\xF8\x0E\x03\xC8\x89\x4F\x3C";
		const std::string yMask = "xxxxxxxx";
		const DWORD yDefaultValue = 0x0E;
	}
	namespace water
	{
		const std::string xSignature = "";
		const std::string xMask = "";
		const DWORD xDefaultValue = 0x10;
		const std::string ySignature = "";
		const std::string yMask = "";
		const DWORD yDefaultValue = 0x10;
	}
	namespace underwater //default values to be checked
	{
		const std::string xSignature = "";
		const std::string xMask = "";
		const DWORD xDefaultValue = 0x0E;
		const std::string ySignature = "";
		const std::string yMask = "";
		const DWORD yDefaultValue = 0x10;
		const std::string zSignature = "";
		const std::string zMask = "";
		const DWORD zDefaultValue = 0x0E;

	}

	struct signatureMaskAddrs
	{
		const std::string signature;
		const std::string mask;
		DWORD addr;
		DWORD jmpBackAddr;
	};

	//ground
	DWORD xGroundSpeedAddress = 0;
	DWORD xGroundSpeedJmpBack = 0;
	DWORD yGroundSpeedAddress = 0;
	DWORD yGroundSpeedJmpBack = 0;
	//surface water
	DWORD xWaterSpeedAddress = 0;
	DWORD xWaterSpeedJmpBack = 0;
	DWORD yWaterSpeedAddress = 0;
	DWORD yWaterSpeedJmpBack = 0;
	//underwater
	DWORD xUnderwaterSpeedAddress = 0;
	DWORD xUnderwaterSpeedJmpBack = 0;
	DWORD yUnderwaterSpeedAddress = 0;
	DWORD yUnderwaterSpeedJmpBack = 0;
	DWORD zUnderwaterSpeedAddress = 0;
	DWORD zUnderwaterSpeedJmpBack = 0;


	std::vector<speedHack::signatureMaskAddrs> entries = { {ground::xSignature, ground::xMask , xGroundSpeedAddress, xGroundSpeedJmpBack},
														{ ground::ySignature, ground::yMask , yGroundSpeedAddress, yGroundSpeedJmpBack}, 
														{ water::xSignature, water::xMask , xWaterSpeedAddress, xWaterSpeedJmpBack },
														{ water::ySignature, water::ySignature , yWaterSpeedAddress, yWaterSpeedJmpBack}, 
														{ underwater::ySignature, underwater::yMask , yUnderwaterSpeedAddress, yUnderwaterSpeedJmpBack },
														{ underwater::zSignature, underwater::zMask , zUnderwaterSpeedAddress, zUnderwaterSpeedJmpBack}	};

	enum entryType { xGround, yGround, xWater, yWater, xUnderwater, yUnderwater, zUnderwater, SIZE = 7 };

	inline bool patchSpeed(const std::string& baseModuleName, unsigned int speed, DWORD* addr, DWORD* jmpBackAddr, unsigned int index)
	{
		if (index >= entryType::SIZE)
		{
			return false;
		}

		std::vector<speedHack::signatureMaskAddrs>::iterator iterator = entries.begin();
		DWORD speedValueOffset = 0x02; //__asm SAR EAX, SPEED where SPEED is a DWORD shifted by 2 bytes after the instruction start
		(iterator + index)->addr = *addr = ghe::winCodeCave::findPattern(baseModuleName, (iterator + index)->signature, (iterator + index)->mask);
		(iterator + index)->jmpBackAddr = *jmpBackAddr = (iterator + index)->addr + static_cast<DWORD>((iterator + index)->mask.size());
		BYTE _patch[1] = { speedHack::speedLevels.at(speed) };
		return ghe::winCodeCave::patchMemory(static_cast<uintptr_t>((iterator + index)->addr + speedValueOffset), _patch, 1);
	}

	inline bool changeGroundSpeed(const std::string& baseModuleName, unsigned int speed)
	{
		if (speed > 5)
		{
			return false;
		}
		//to be tested
		if (patchSpeed(baseModuleName, speed, &xGroundSpeedAddress, &xGroundSpeedJmpBack, entryType::xGround))
		{
			return patchSpeed(baseModuleName, speed, &yGroundSpeedAddress, &yGroundSpeedJmpBack, entryType::yGround);
		}
		return false;
	}

	inline bool activateWaterSpeedHack(const std::string& baseModuleName)
	{
		return true;
	}

	inline bool activateUnderwaterSpeedHack(const std::string& baseModuleName)
	{
		return true;
	}
}