#include "health.h"
#include <iostream>
#include "../mem/memory.h"


float Health::healthInfinity()
{
	float hp = 0.0f;
	std::vector<DWORD> offsets = { 0x01C399D8, 0x0, 0x8, 0x0, 0x900, 0x50, 0x18, 0x92C };
	//std::cout << memory::readBaseModule("gamedll_x64_rwdi.dll") << std::endl;
	uintptr_t hp_pointer = memory::readPointerFromVector(offsets, "gamedll_x64_rwdi.dll");
	memory::writePointerValue(hp_pointer, 250.0f);
	return hp;
}
