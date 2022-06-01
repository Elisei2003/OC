#include <conio.h>
#include <iostream>

bool isSeparator(char c) {
	const char separators[6] = "_.,!?";

	for (int i = 0; i < strlen(separators); i++) {
		if (separators[i] == c) {
			return true;
		}
	}
	
	return false;
}

void shift(char* a, int size) {
	int temp = a[0];
	for (int i = 0; i < size - 1; i++) {
		a[i] = a[i + 1];
	}
	a[size - 1] = temp;
}

char* findTokens(char* arr, int size) {
	char* newArr = new char[size * 2 + 1];
	bool isPrevToken = false;
	int newArrIndex = 0;
	int sepsIndex = -1;
	int shiftIndex = 0;

	for (int i = 0; i < size; i++) {
		if (!isSeparator(arr[i])) {
			newArr[newArrIndex] = arr[i];
			isPrevToken = true;
			newArrIndex++;
		}
		else if (isSeparator(arr[i]) && isPrevToken) {
			newArr[newArrIndex] = '_';
			newArrIndex++;
			isPrevToken = false;
			newArr[size + sepsIndex] = arr[i];
			sepsIndex++;
		}
		else {
			newArr[size + sepsIndex] = arr[i];
			sepsIndex++;
			shiftIndex++;
		}
	}
	newArr[newArrIndex + shiftIndex + sepsIndex] = '\0';

	char* rimToShift = newArr + newArrIndex;
	for (int i = 1; i < shiftIndex; i++) {
		shift(rimToShift, shiftIndex + sepsIndex + 1);
	}

	return newArr;
}

int main(int argc, char *argv[])
{
	_cputs("I am created.\n");

	int size = strlen(argv[0]);
	char* arr = argv[0];
	_cputs(arr);
	_cputs("\n");

	char* answ = new char[size * 2 + 1];
	answ = findTokens(arr, size);
	_cputs(answ);

	_cputs("\nPress any key to finish.\n");
	_getch();

	return 0;
}