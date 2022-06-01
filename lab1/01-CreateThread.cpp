#include <windows.h>
#include <iostream>
#include <cmath>

using namespace std;

bool isFractionalPartLessThanHalf(double number) {
	float temp = 5.1;
	float* tempPtr = &temp;
	float frPart = modf(number, tempPtr);

	if (frPart < 0) {
		frPart = -frPart;
	}

	return frPart < 0.5;
}

DWORD WINAPI ManipulateArray(LPVOID arr)
{
	cout << "Thread is started." << endl;
	int dimension = ((double*)arr)[0];
	double *mas = (double* )arr;
	unsigned long suspendTime = ((unsigned long*)arr)[dimension + 1];
	unsigned long resumeTime = ((unsigned long*)arr)[dimension + 2];
	mas = mas + 1;
	for (int i = 0; i < dimension; i++) {
		if (isFractionalPartLessThanHalf(mas[i])) {
			cout << mas[i] << " ";
		}
		Sleep(25);
	}

	cout << endl << "Thread is finished." << endl;

	return 0;
}

int main()
{
	int	dimension;
	cout << "Enter dimension of array: ";
	cin >> dimension;
	
	double* arr = new double[dimension + 3];
	arr[0] = dimension;

	cout << "Enter array: ";
	for (int i = 1; i < dimension + 1; i++) {
		cin >> arr[i];
	}

	cout << "Enter time (in ms) for suspending and resuming thread: ";
	cin >> arr[dimension + 1];
	cin >> arr[dimension + 2];


	HANDLE worker;
	DWORD IDThread;

	worker = CreateThread(NULL, 0, ManipulateArray, (void*)arr, 0, &IDThread);
	if (worker == NULL)
		return GetLastError();

	Sleep(arr[dimension + 1]);
	SuspendThread(worker);
	Sleep(arr[dimension + 2]);
	ResumeThread(worker);

	WaitForSingleObject(worker, INFINITE);
	CloseHandle(worker);

	delete arr;
	return 0;
}