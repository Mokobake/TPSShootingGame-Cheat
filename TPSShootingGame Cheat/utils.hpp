uintptr_t GetPID(const wchar_t* processName)
{
	PROCESSENTRY32 procEntry32;
	uintptr_t pID = 0;

	procEntry32.dwSize = sizeof(PROCESSENTRY32);

	HANDLE hProcSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (!hProcSnap || hProcSnap == INVALID_HANDLE_VALUE)
		return 0;

	while (Process32Next(hProcSnap, &procEntry32))
	{
		if (!wcscmp(processName, procEntry32.szExeFile))
		{
			pID = procEntry32.th32ProcessID;

			CloseHandle(hProcSnap);
		}
	}

	CloseHandle(hProcSnap);
	return pID;
}

HANDLE GetProcessHandle(DWORD PID)
{
	return OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
}

uintptr_t GetBaseAddress(const wchar_t* name, DWORD PID)
{
	MODULEENTRY32 me32;
	me32.dwSize = sizeof(MODULEENTRY32);
	HANDLE hDump = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, PID);
	if (Module32First(hDump, &me32))
	{
		do {
			if (!wcscmp(name, me32.szModule))
			{
				CloseHandle(hDump);
				return (uintptr_t)me32.modBaseAddr;
				break;//dumb code
			}
		} while (Module32Next(hDump, &me32));
	}
	return -1337;
}

template <typename T>
T read(HANDLE ProcessHandle, uintptr_t Address)
{
	T Buffer;
	ReadProcessMemory(ProcessHandle, LPCVOID(Address), &Buffer, sizeof(Buffer), 0);
	return Buffer;
}

template <typename T>
void write(HANDLE ProcessHandle, uintptr_t Address, T Buffer)
{
	WriteProcessMemory(ProcessHandle, LPVOID(Address), &Buffer, sizeof(Buffer), 0);
}