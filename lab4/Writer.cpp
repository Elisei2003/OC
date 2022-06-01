#pragma warning(disable : 4996)
#include <iostream>
#include <windows.h>
#include <conio.h>
HANDLE A, B, endWriter;
HANDLE hMutex;
using namespace std;
int main(int argc, char* argv[]) {
	A = CreateEvent(NULL, TRUE, FALSE, "A");
	if (A == NULL) {
		return GetLastError();
	}
	B = CreateEvent(NULL, TRUE, FALSE, "B");
	if (B == NULL) {
		return GetLastError();
	}
	endWriter = CreateEvent(NULL, TRUE, FALSE, "endWriter");
	if (endWriter == NULL) {
		return GetLastError();
	}
	hMutex = CreateMutex(NULL, FALSE, "MutexWriter");
	if (hMutex == NULL) {
		return GetLastError();
	}
	char message;
	WaitForSingleObject(hMutex, INFINITE);
	int num = atoi(argv[1]);
	for (int i = 0; i < num; i++) {
		cout << "Input message for process Writer:" << endl;
		cin >> message;
		if (message == 'A') {
			SetEvent(A);
		}
		else if (message == 'B') {
			SetEvent(B);
		}
	}
	ReleaseMutex(hMutex);
	CloseHandle(A);
	CloseHandle(B);
	SetEvent(endWriter);
	system("pause");
	return 0;
}