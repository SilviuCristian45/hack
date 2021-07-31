#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>

//global vars
HANDLE hProc = NULL; //a reference to the procces we will attach this program
DWORD pid; //the id of the procces we will attach

bool attachProc(std::string procName) {
	PROCESSENTRY32 procEntry32;
	//set the size of the structure 
	procEntry32.dwSize = sizeof(PROCESSENTRY32);
	//getting the snapshot of all processes (a list) we will loop through it to find our process
	auto hProcSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hProcSnap == INVALID_HANDLE_VALUE) {
		std::cout << "failed to take snapshot of the processes";
		return false;
	}

	const char* procNameCstr = procName.c_str();

	while (Process32Next(hProcSnap, &procEntry32)) {
		std::cout << procEntry32.szExeFile << "\n";
		
		if (!strcmp(procNameCstr, procEntry32.szExeFile)) {
			std::cout << "we found our process with pid " << procEntry32.th32ProcessID << "\n";
			hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procEntry32.th32ProcessID);

			if (hProc == NULL){
				std::cout << "failed opening the process" << "\n";
			}

			CloseHandle(hProcSnap); //close the hProcSnap (we found the procces so we can stop)
			return true;
		}
	}

	//if we got here 
	std::cout << "Couldnt find the target procces" << "\n";
	return false;

}

template <class dataType> //write any type of data 

void wpm(dataType valToWrite, DWORD addresToWrite) {
	WriteProcessMemory(hProc, (PVOID)addresToWrite, &valToWrite, sizeof(dataType), 0); //scriem la adresa respectiva
}


int main() {

	DWORD hackableAddres = 0x0053FE0C;
	attachProc("HackableProgram.exe");
	wpm <int>(150, hackableAddres);
	return 0;
}