#include <windows.h>
#include <conio.h>
#include <iostream>
using namespace std;
int main(int argc, char* argv[])
{
	HANDLE hWritePipe, hReadPipe;
	HANDLE hRead2;
	hRead2 = OpenEvent(EVENT_ALL_ACCESS, FALSE, "Read2");
	hWritePipe = (HANDLE)atoi(argv[1]);
	hReadPipe = (HANDLE)atoi(argv[2]);
	int n;
	DWORD dwBytesRead;
	if (!ReadFile(hReadPipe, &n, sizeof(n), &dwBytesRead, NULL))
	{
		_cputs("Read from the pipe failed.\n");
		_cputs("Press any key to finish.\n");
		_getch();
		return GetLastError();
	}
	DWORD dwBytesWritten;
	char* arr = new char[n];
	srand(time(0));
	for (int i = 0; i < n; i++) {
		arr[i] = '0' + rand() % ('9' - '0');
	}
	for (int i = 0; i < n; i++) {
		if (!WriteFile(hWritePipe, &arr[i], sizeof(char), &dwBytesWritten, NULL))
		{
			_cputs("Write to file failed.\n");
			_cputs("Press any key to finish.\n");
			_getch();
			return GetLastError();
		}
	}
	cout << endl;
	SetEvent(hRead2);
	CloseHandle(hWritePipe);
	CloseHandle(hReadPipe);
	CloseHandle(hRead2);
	return 0;
}