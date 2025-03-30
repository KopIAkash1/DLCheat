#include "Player.h"
#include "../mem/memory.h"


bool Player::healthInfinity()
{
	std::vector<DWORD> offsets = { 0x01C399D8, 0x0, 0x8, 0x0, 0x900, 0x50, 0x18, 0x92C };
	//std::cout << memory::readBaseModule("gamedll_x64_rwdi.dll") << std::endl;
	uintptr_t hp_pointer = memory::readPointerFromVector(offsets, "gamedll_x64_rwdi.dll");
	memory::writePointerValue(hp_pointer, 250.0f);
	return true;
}

bool Player::staminaInfinity()
{
	float stamina = 2.0f;
	std::vector<DWORD> offsets = { 0x01D0E8A0, 0x4D8, 0x48, 0x898, 0x10 };
	uintptr_t stamina_pointer = memory::readPointerFromVector(offsets, "gamedll_x64_rwdi.dll");
	memory::writePointerValue(stamina_pointer, 2.0f);
	return true;
}