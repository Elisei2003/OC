#include <windows.h>
#include <conio.h>
#include <iostream>
using namespace std;
int main()
{
	int n;
	cout << "Input size of the array:" << endl;
	cin >> n;
	char* arr = new char[n];
	char lpszComLine[80];
	HANDLE hRead2;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	HANDLE hWritePipe, hReadPipe;
	SECURITY_ATTRIBUTES sa;
	hRead2 = CreateEvent(NULL, FALSE, FALSE, "Read2");
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;
	if (!CreatePipe(&hReadPipe, &hWritePipe, &sa, 0))
	{
		_cputs("Create pipe failed.\n");
		_cputs("Press any key to finish.\n");
		_getch();
		return GetLastError();
	}
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	wsprintf(lpszComLine, "Figure.exe %d %d", (int)hWritePipe, (int)hReadPipe);
	if (!CreateProcess(NULL, lpszComLine, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi))
	{
		_cputs("Create process failed.\n");
		_cputs("Press any key to finish.\n");
		_getch();
		return GetLastError();
	}
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	DWORD dwBytesWritten;
	if (!WriteFile(hWritePipe, &n, sizeof(n), &dwBytesWritten, NULL))
	{
		_cputs("Write to file failed.\n");
		_cputs("Press any key to finish.\n");
		_getch();
		return GetLastError();
	}
	DWORD dwBytesRead;
	WaitForSingleObject(hRead2, INFINITE);
	cout << "n = " << n << endl;
	cout << "Received information:" << endl;
	for (int i = 0; i < n; i++) {
		if (!ReadFile(hReadPipe, &arr[i], sizeof(char), &dwBytesRead, NULL))
		{
			_cputs("Read from the pipe failed.\n");
			_cputs("Press any key to finish.\n");
			_getch();
			return GetLastError();
		}
		cout << arr[i] << " ";
	}
	cout << endl;
	CloseHandle(hReadPipe);
	CloseHandle(hWritePipe);
	CloseHandle(hRead2);
	return 0;
}