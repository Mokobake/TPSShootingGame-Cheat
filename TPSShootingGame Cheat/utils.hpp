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

class Vector3
{
public:
	Vector3() : x(0.f), y(0.f), z(0.f)
	{

	}

	Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z)
	{

	}
	~Vector3()
	{

	}

	float x;
	float y;
	float z;

	inline float Dot(Vector3 v)
	{
		return x * v.x + y * v.y + z * v.z;
	}

	inline float Distance(Vector3 v)
	{
		return float(sqrtf(powf(v.x - x, 2.0) + powf(v.y - y, 2.0) + powf(v.z - z, 2.0)));
	}

	Vector3 operator+(Vector3 v)
	{
		return Vector3(x + v.x, y + v.y, z + v.z);
	}

	Vector3 operator-(Vector3 v)
	{
		return Vector3(x - v.x, y - v.y, z - v.z);
	}

	Vector3 operator*(float number) const {
		return Vector3(x * number, y * number, z * number);
	}

	float Length()
	{
		return sqrt((x * x) + (y * y) + (z * z));
	}
};