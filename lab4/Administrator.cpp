#pragma warning(disable : 4996)
#include <Windows.h>
#include <process.h>
#include <conio.h>
#include <iostream>
#include <string>
using namespace std;
int main() {
	HANDLE A, B, endWriter, endReader;
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
	endReader = CreateEvent(NULL, TRUE, FALSE, "endReader");
	if (endReader == NULL) {
		return GetLastError();
	}
	string ReaderProcessName = "Reader.exe";
	string WriterProcessName = "Writer.exe";
	int numReaderProcess, numWriterProcess, numMessageReaderProcess, numMessageWriterProcess;
	cout << "Input num of the Writer processes: ";
	cin >> numWriterProcess;
	cout << "Input num of the Reader processes: ";
	cin >> numReaderProcess;
	cout << "Input num of the messages from Writer processes: ";
	cin >> numMessageWriterProcess;
	numMessageReaderProcess = (numMessageWriterProcess * numWriterProcess) / (numReaderProcess / 2 + numReaderProcess % 2);
	HANDLE* Writers = new HANDLE[numWriterProcess];
	HANDLE* Readers = new HANDLE[numReaderProcess];
	STARTUPINFO si;
	PROCESS_INFORMATION piApp;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	string connect;
	connect = WriterProcessName + " " + to_string(numMessageWriterProcess);
	char* con = new char[connect.size() + 1];
	con = strcpy(con, connect.c_str());
	for (int i = 0; i < numWriterProcess; i++) {
		if (!CreateProcess(NULL, con, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &piApp))
		{
			_cputs("The new process is not created.\n"); // Эти три штуки были и в методичках, поэтому решил не менять, но по-большому счету можно заменить к примеру на 
			_cputs("Check a name of the process.\n"); // cout << "Error"; или вообще убрать.
			_cputs("Press any key to finish.\n");
			return 0;
		}
		Writers[i] = piApp.hProcess;
	}
	string connect1;
	connect1 = ReaderProcessName + " " + to_string(numMessageReaderProcess);
	char* con1 = new char[connect1.size() + 1];
	con1 = strcpy(con, connect1.c_str());
	for (int i = 0; i < numReaderProcess; i++) {
		if (!CreateProcess(NULL, con1, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &piApp))
		{
			_cputs("The new process is not created.\n");
			_cputs("Check a name of the process.\n");
			_cputs("Press any key to finish.\n");
			return 0;
		}
		Readers[i] = piApp.hProcess;
	}
	int sum = 0;
	HANDLE mass[] = { endReader, endWriter };
	while (true) {
		int ind = WaitForMultipleObjects(2, mass, FALSE, INFINITE) - WAIT_OBJECT_0;
		if (ind == 0) {
			sum += 1;
			ResetEvent(endReader);
		}
		else if (ind == 1) {
			sum += 1;
			ResetEvent(endWriter);
		}
		if (sum == numWriterProcess + numReaderProcess / 2 + numReaderProcess % 2) {
			break;
		}
	}
	CloseHandle(A);
	CloseHandle(B);
	CloseHandle(endWriter);
	CloseHandle(endReader);
	for (int i = 0; i < numWriterProcess; i++)
	{
		CloseHandle(Writers[i]);
	}
	for (int i = 0; i < numReaderProcess; i++)
	{
		CloseHandle(Readers[i]);
	}
	return 0;
}