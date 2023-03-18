#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include "utils.hpp"


DWORD PID;
HANDLE ProcessHandle;
uintptr_t BaseAddress;
bool fly;

int main()
{
	SetConsoleTitle(L"TPSShootingGame Cheat");
	std::cout << "\n\n  Waiting TPSShootingGame" << std::endl;
	while (true)
	{
		if (FindWindow(L"UnrealWindow", L"TPSShootingGame  "))
		{
			break;
		}
	}
	system("cls");
	std::cout << "\n\n  TPSShootingGame Found." << std::endl;
	Sleep(500);
	PID = GetPID(L"UE4Game-Win64-Shipping.exe");
	std::cout << "\n  PID: " << PID << std::endl;
	ProcessHandle = GetProcessHandle(PID);
	std::cout << "  ProcessHandle: " << ProcessHandle << std::endl;
	BaseAddress = GetBaseAddress(L"UE4Game-Win64-Shipping.exe", PID);
	printf("  BaseAddress: 0x%llX", BaseAddress);

	while (true)
	{
		uintptr_t UWorld = read<uintptr_t>(ProcessHandle, BaseAddress + 0x44F0AE0);
		uintptr_t OwningGameInstance = read<uintptr_t>(ProcessHandle, UWorld + 0x180);
		uintptr_t LocalPlayer = read<uintptr_t>(ProcessHandle, read<uintptr_t>(ProcessHandle, OwningGameInstance + 0x38));
		uintptr_t PlayerController = read<uintptr_t>(ProcessHandle, LocalPlayer + 0x30);
		uintptr_t PlayerCameraManager = read<uintptr_t>(ProcessHandle, PlayerController + 0x2b8);
		uintptr_t Pawn = read<uintptr_t>(ProcessHandle, PlayerController + 0x250);
		DWORD_PTR RootComponent = read<DWORD_PTR>(ProcessHandle, Pawn + 0x130);
		Vector3 PawnRelativeLocation = read<Vector3>(ProcessHandle, RootComponent + 0x11c);

		if (GetAsyncKeyState(VK_LSHIFT))
		{
			write<float>(ProcessHandle, Pawn + 0x98, 2);
		}
		else if (read<float>(ProcessHandle, Pawn + 0x98) != 1)
		{
			write<float>(ProcessHandle, Pawn + 0x98, 1);
		}

		write<float>(ProcessHandle, PlayerCameraManager + 0x238 + 0x4, 130);

		if (GetAsyncKeyState(VK_NUMPAD1) & 1)
		{
			fly = !fly;

			if (fly)
			{
				Beep(500, 500);
			}
			else
			{
				Beep(300, 500);
			}
		}

		if (fly)
		{
			if (GetAsyncKeyState('W'))
			{
				PawnRelativeLocation.x += 0.1;
			}
			if (GetAsyncKeyState('S'))
			{
				PawnRelativeLocation.x -= 0.1;
			}
			if (GetAsyncKeyState('A'))
			{
				PawnRelativeLocation.y -= 0.1;
			}
			if (GetAsyncKeyState('D'))
			{
				PawnRelativeLocation.y += 0.1;
			}
			if (GetAsyncKeyState(VK_SPACE))
			{
				PawnRelativeLocation.z += 0.1;
			}
			if (GetAsyncKeyState(VK_LSHIFT))
			{
				PawnRelativeLocation.z -= 0.1;
			}

			write<Vector3>(ProcessHandle, RootComponent + 0x11c, PawnRelativeLocation);
			write<char>(ProcessHandle, RootComponent + 0x14c, 0);
		}
	}
}