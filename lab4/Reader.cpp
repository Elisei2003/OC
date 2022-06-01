#pragma warning(disable : 4996)
#include <iostream>
#include <windows.h>
#include <conio.h>
using namespace std;
HANDLE A, B, endReader;
HANDLE hSemaphore;
int main(int argc, char* argv[])
{
	A = CreateEvent(NULL, TRUE, FALSE, "A");
	if (A == NULL) {
		return GetLastError();
	}
	B = CreateEvent(NULL, TRUE, FALSE, "B");
	if (B == NULL) {
		return GetLastError();
	}
	endReader = CreateEvent(NULL, TRUE, FALSE, "endReader");
	if (endReader == NULL) {
		return GetLastError();
	}
	hSemaphore = CreateSemaphore(NULL, 2, 2, "SemaphoreReader");
	if (hSemaphore == NULL) {
		return GetLastError();
	}
	WaitForSingleObject(hSemaphore, INFINITE);
	int num = atoi(argv[1]);
	HANDLE mass[] = { A, B };
	for (int i = 0; i < num; i++)
	{
		int ind = WaitForMultipleObjects(2, mass, FALSE, INFINITE) - WAIT_OBJECT_0;
		if (ind == 0)
		{
			cout << "Get message A from Writer" << endl;
			ResetEvent(A);
		}
		else if (ind == 1)
		{
			cout << "Get message B from Writer" << endl;
			ResetEvent(B);
		}
	}
	ReleaseSemaphore(hSemaphore, 1, NULL);
	CloseHandle(A);
	CloseHandle(B);
	SetEvent(endReader);
	system("pause");
	return 0;
}