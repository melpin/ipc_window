//client

#include<strsafe.h>
#include<stdint.h>
#include<windows.h>
#include<crtdbg.h>
#include<wchar.h>
#include<stdio.h>
#include<stdlib.h>

DWORD WINAPI read_thread(void *param){
	char input_buff[1024] = { 0 };
	char last_buff[1024] = { 0 };
	int last_buff_size = 0;
	
	while (1){

		memset(input_buff, 0, 1024);
		CopyMemory(input_buff, (char*)param, 1024);
		if (input_buff[0] == 'u'){
			Sleep(500);
			continue;
		}

		if (strncmp(input_buff, last_buff, 1024) != 0 &&	
			!(input_buff[0] == 0) &&
			input_buff[1] != 'c'){
			fprintf(stdout, "server : %s\n", input_buff + 3);
			strncpy_s(last_buff, input_buff, 1024);

		}

	}
}

DWORD WINAPI write_thread(void *param){
	char input_buf[1024] = { 0 };
	int last_input_byte = 0;
	int i = 1;
	printf("chat start\n");

	while (1){

		input_buf[0] = 'u'; // using or un using 1 or 0
		input_buf[1] = 'c'; // server side check 
		input_buf[2] = i++;

		scanf_s("%[^\n]s ", input_buf + 3, 1021); // ignore token space
		getchar();

		memset((char*)param, 0, last_input_byte); // memory clean
		last_input_byte = strnlen(input_buf, 1024);
		StringCbPrintfA((char*)param, 1024, "%s\n", input_buf); // using memory
		memset((char*)param, 'f', 1); // use free
		memset(input_buf, 0, last_input_byte); // buffer clean

	}
}


int main(){
	printf("client!\n");

	DWORD ThreadId[2];
	HANDLE ThreadHandle[2];

	HANDLE map = CreateFileMapping(
		INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,
		0,
		1024,
		L"mil"
		);

	if (map == NULL){
		fwprintf(stderr, L"createfilemapping() failed.\n");
		return -1;
	}

	char *ptr = (char*)MapViewOfFile(
		map,
		FILE_MAP_READ | FILE_MAP_WRITE,
		0,
		0,
		1024);

	if (ptr == NULL){
		fwprintf(stderr, L"mapviewoffile() failed.\n");
		CloseHandle(map);
		return -1;
	}

	ThreadHandle[0] = CreateThread(
		NULL,
		0,
		read_thread,
		ptr,
		0,
		&ThreadId[0]);

	ThreadHandle[1] = CreateThread(
		NULL,
		0,
		write_thread,
		ptr,
		0,
		&ThreadId[1]);

	if (ThreadHandle != NULL && ThreadHandle != NULL) {
		WaitForMultipleObjects(2, ThreadHandle, TRUE, INFINITE);
	}

}
