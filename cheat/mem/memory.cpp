#include "memory.h"
#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>

HANDLE memory::getHandle()
{
    HWND hwnd = FindWindowA(NULL, "Dying Light");
    if (!hwnd)
    {
        hwnd = FindWindowA(NULL, "Dying Light (inactive)");
    }
    DWORD procID;
    GetWindowThreadProcessId(hwnd, &procID);
    HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procID);
    return handle;
}

uintptr_t memory::readBaseModule(std::string base_module_name)
{
    HWND hwnd = FindWindowA(NULL, "Dying Light");
    if (!hwnd)
    {
        hwnd = FindWindowA(NULL, "Dying Light (inactive)");
    }
    DWORD procID;
    GetWindowThreadProcessId(hwnd, &procID);
    
    uintptr_t moduleBaseAddress = 0;
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procID);
    if (snapshot != INVALID_HANDLE_VALUE) {
        MODULEENTRY32 moduleEntry;
        moduleEntry.dwSize = sizeof(moduleEntry);
        if (Module32First(snapshot, &moduleEntry)) {
            do {
                if (!_stricmp(moduleEntry.szModule, base_module_name.c_str())) {
                    moduleBaseAddress = (uintptr_t)moduleEntry.modBaseAddr;
                    break;
                }
            } while (Module32Next(snapshot, &moduleEntry));
        }
    }
    CloseHandle(snapshot);

    return moduleBaseAddress;
}

uintptr_t memory::readPointerFromVector(std::vector<DWORD> pointerOffsets, std::string base_module_name)
{
    std::cout << "START" << std::endl;
    uintptr_t address = readBaseModule(base_module_name);
    HANDLE handle = getHandle();
    for (int i = 0; i < pointerOffsets.size() - 1; i++)
    {
        ReadProcessMemory(handle, (LPCVOID)(address + pointerOffsets.at(i)), &address, sizeof(address), 0);
        std::cout << "Buff: " << address << std::endl;
    }
    return address + pointerOffsets.back();
}

template <typename T>
void writePointerValue(uintptr_t pointer, T value) {

}