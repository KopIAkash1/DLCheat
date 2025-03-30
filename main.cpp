#include "includes.h"
#include "cheat/base_func/health.h"
#include <stdio.h>
#include <iostream>
#include <WindowsX.h>
#include <dinput.h>
#pragma comment(lib, "Dinput8.lib")
#pragma comment(lib, "Dxguid.lib")

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

Present oPresent;
HWND window = NULL;
WNDPROC oWndProc;
ID3D11Device* pDevice = NULL;
ID3D11DeviceContext* pContext = NULL;
ID3D11RenderTargetView* mainRenderTargetView;

bool infinity_health = false;

bool init = false;
bool acitive_menu = false;

IDirectInput8* dinput = nullptr;
LPDIRECTINPUTDEVICE8 lpdiMouse;
DIMOUSESTATE mouseState;


void InitImGui()
{
	ImGui::CreateContext();
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX11_Init(pDevice, pContext);
}


LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	ImGui::GetIO().WantCaptureMouse = true;
	if (true && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	if (!init)
	{
		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)& pDevice)))
		{
			pDevice->GetImmediateContext(&pContext);
			DXGI_SWAP_CHAIN_DESC sd;
			pSwapChain->GetDesc(&sd);
			window = sd.OutputWindow;
			ID3D11Texture2D* pBackBuffer;
			pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)& pBackBuffer);
			pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
			pBackBuffer->Release();
			oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
			InitImGui();
			init = true;
		}

		else
			return oPresent(pSwapChain, SyncInterval, Flags);
	}

	if (GetAsyncKeyState(VK_END))
	{
		kiero::shutdown();
	}
	if (GetAsyncKeyState(VK_INSERT) & 1)
	{
		acitive_menu = !acitive_menu;
		if (acitive_menu)
			ImGui::GetIO().MouseDrawCursor = true;
		else
			ImGui::GetIO().MouseDrawCursor = false;
	}
	ImGuiIO& io = ImGui::GetIO();
	// 
	//  io.MouseDrawCursor = acitive_menu;


	if (!dinput) {
		std::cout << "Cannot create dinput" << std::endl;
	}
	else {
		if (acitive_menu) {
			if (lpdiMouse)
			{
				// Read mouse input via DirectInput

				if (SUCCEEDED(lpdiMouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouseState)))
				{
					if (mouseState.rgbButtons[0] & 0x80) 
					{
						PostMessage(window, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(mouseState.lX, mouseState.lY));
					}
					else {
						PostMessage(window, WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM(mouseState.lX, mouseState.lY));
					}
					PostMessage(window, WM_MOUSEMOVE, 0, MAKELPARAM(mouseState.lX, mouseState.lY));
				}
			}
			else {
				std::cout << "Cannot create DEVICE" << std::endl;
				dinput->Release();
				dinput = nullptr;
			}
		}
	}
	// CHECKBOXES
	if (infinity_health)
	{
		Health::healthInfinity();
	}
	if (acitive_menu) {
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("Kop1Akash1 | MALWARE");
		ImGui::Checkbox("Infinity Medicine", &infinity_health);
		ImGui::End();

		ImGui::Render();

		pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}
	
	return oPresent(pSwapChain, SyncInterval, Flags);
}

DWORD WINAPI MainThread(LPVOID lpReserved)
{
	bool init_hook = false;
	do
	{
		if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success)
		{
			kiero::bind(8, (void**)& oPresent, hkPresent);
			init_hook = true;
		}
	} while (!init_hook);
	return TRUE;
}

BOOL WINAPI DllMain(HMODULE hMod, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hMod);
		AllocConsole();
		freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
		freopen_s((FILE**)stdin, "CONIN$", "r", stdin);
		std::cout << "[+] Cheat injected successfully!\n";
		DirectInput8Create(hMod, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dinput, NULL);
		if (SUCCEEDED(dinput->CreateDevice(GUID_SysMouse, &lpdiMouse, NULL)))
		{
			lpdiMouse->SetDataFormat(&c_dfDIMouse);
			lpdiMouse->SetCooperativeLevel(window, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
			lpdiMouse->Acquire();
		}
		CreateThread(nullptr, 0, MainThread, hMod, 0, nullptr);
		break;
	case DLL_PROCESS_DETACH:
		kiero::shutdown();
		break;
	}
	return TRUE;
}

