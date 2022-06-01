#pragma warning(disable : 4996)
#include <Windows.h>
#include <iostream>
#include <ctime>
#include <algorithm>
using namespace std;
CRITICAL_SECTION cs;
HANDLE event;
struct Array {
	char* array;
	int n;	
	int sum = 0;
};
DWORD WINAPI SumElement(LPVOID _array)
{
	EnterCriticalSection(&cs);
	WaitForSingleObject(event, INFINITE);
	Array* array = (Array*)_array;
	for (int i = 0; i < array->n; i++) {
		if (array->array[i] == '_') {
			break;
		}
		array->sum += 1;
	}	
	LeaveCriticalSection(&cs);	
	return 0;
}
DWORD WINAPI work(LPVOID _array)
{
	cout << "Input sleep time" << endl;
	int s = 0;
	cin >> s;
	Array* array = (Array*)_array;
	char* arr = new char[array->n];
	int j = 0;
	for (int i = 0; i < array->n-1; i++) {
		if (i == 0 && (array->array[i] == '1' || array->array[i] == '2' || array->array[i] == '3' || array->array[i] == '4' || array->array[i] == '5' || array->array[i] == '6' || array->array[i] == '7' || array->array[i] == '8' || array->array[i] == '9' || array->array[i] == '0')) {
			for (; i < array->n; i++) {
				if (array->array[i] == ' ' || array->array[i] == '-') {
					break;
				}
				arr[j] = array->array[i];
				j++;
			}
			Sleep(s);
		}
		if ((array->array[i] == ' ' || array->array[i] == '-') && (array->array[i+1] == '1' || array->array[i+1] == '2' || array->array[i+1] == '3' || array->array[i+1] == '4' || array->array[i+1] == '5' || array->array[i+1] == '6' || array->array[i+1] == '7' || array->array[i+1] == '8' || array->array[i+1] == '9' || array->array[i+1] == '0')) {
			i++;
			for (; i < array->n; i++) {
				if (array->array[i] == ' ' || array->array[i] == '-') {
					break;
				}
				arr[j] = array->array[i];
				j++;
			}
			Sleep(s);
		}
	}
	for (; j < array->n; j++) {
		arr[j] = '_';
	}
	arr[array->n] = '\0';
	for (int i = 0; i < array->n; i++) {
		array->array[i] = arr[i];
	}
	PulseEvent(event);	
	return 0;
}
int main() {
	event = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (event == NULL) {
		return GetLastError();
	}
	InitializeCriticalSection(&cs);
	HANDLE hThread;
	DWORD IDThread;
	HANDLE hThread2;
	DWORD IDThread2;
	cout << "Input n:" << endl;
	int n;
	cin >> n;
	cout << "Input array:" << endl;
	char* arr = new char[n+1];
	cin.get();
	cin.getline(arr, n+1);
	cout << "Size of the source array: " << n << endl;
	cout << "Elements of the source array: ";
	for (int i = 0; i < n; i++) {
		cout << arr[i];
	}
	cout << endl;	
	Array* array = new Array();
	array->array = arr;
	array->n = n;	
	hThread = CreateThread(NULL, 0, work, (void*)array, 0, &IDThread);
	if (hThread == NULL) {
		return GetLastError();
	}
	hThread2 = CreateThread(NULL, 0, SumElement, (void*)array, 0, &IDThread2);
	if (hThread2 == NULL) {
		return GetLastError();
	}
	WaitForSingleObject(event, INFINITE);
	cout << "New array:" << endl;
	for (int i = 0; i < array->n; i++) {
		cout << array->array[i];
	}
	cout << endl;
	EnterCriticalSection(&cs);
	cout << "Count of elements after thread work and CountElements: ";
	cout << array->sum << endl;
	LeaveCriticalSection(&cs);
	WaitForSingleObject(hThread2, INFINITE);
	DeleteCriticalSection(&cs);
	CloseHandle(hThread);
	CloseHandle(hThread2);
}