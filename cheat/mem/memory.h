#pragma once
#include <Windows.h>
#include <vector>
#include <string>


namespace memory
{
	HANDLE getHandle();
	uintptr_t readBaseModule(std::string base_module_name);
	uintptr_t readPointerFromVector(std::vector<DWORD> pointerOffsets, std::string base_module_name);
	template <typename T>
	void writePointerValue(uintptr_t pointer, T value) {
		HANDLE handle = memory::getHandle();
		WriteProcessMemory(handle, (LPVOID)pointer, &value, sizeof(T), nullptr);
	}
};

