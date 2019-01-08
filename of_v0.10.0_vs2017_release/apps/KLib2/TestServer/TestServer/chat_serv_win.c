#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <process.h> 

#define BUF_SIZE 6000
#define MAX_CLNT 256
#define PORT 3800

unsigned WINAPI HandleClnt(void * arg);
void SendMsg(char * msg, int len);
void ErrorHandling(char * msg);

int clntCnt = 0;
SOCKET clntSocks[MAX_CLNT];
HANDLE hMutex;

struct COM_PACKET {
	CHAR Header[4];
	INT Count;
	CHAR Deviece_name[24];
	CHAR sensor1_name[24];
	CHAR sensor2_name[24];
	INT NofDevice;
	INT row;
	INT col;
	CHAR temp1[8];
	BYTE adc[4800];
	CHAR temp2[96];
	CHAR Tail[4];
}packet;

void init()
{
	memset(packet.Header, 0x7E7E7E7E, sizeof(packet.Header));
	packet.Count = 0;
	strcpy_s(packet.Deviece_name, sizeof(packet.Deviece_name), "Snowboard2Wifi");
	strcpy_s(packet.sensor1_name, sizeof(packet.sensor1_name), "MS9723");
	//strcpy_s(packet.sensor2_name, sizeof(packet.sensor1_name), "MS9724");
	packet.NofDevice = 1;
	packet.row = 16;
	packet.col = 10;
	/*
	for (int i = 0; i < 8; ++i) {
		packet.temp1[i] = 0;
	}
	*/
	for (int i = 0; i < 4800; ++i)
	{
		packet.adc[i] = 0.0;
	}
	/*
	for (int i = 0; i < 96; ++i)
	{
		packet.temp2[i] = 0;
	}*/
	memset(packet.Tail, 0x81818181, sizeof(packet.Tail));
}

int main(int argc, char *argv[])
{
	WSADATA wsaData;
	SOCKET hServSock, hClntSock;
	SOCKADDR_IN servAdr, clntAdr;
	int clntAdrSz;
	HANDLE  hThread;
	/*
	if(argc!=2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}*/
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	hMutex = CreateMutex(NULL, FALSE, NULL);
	hServSock = socket(PF_INET, SOCK_STREAM, 0);

	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAdr.sin_port = htons(PORT);

	if (bind(hServSock, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
		ErrorHandling("bind() error");
	if (listen(hServSock, 5) == SOCKET_ERROR)
		ErrorHandling("listen() error");

	init();

	while (1)
	{
		clntAdrSz = sizeof(clntAdr);
		hClntSock = accept(hServSock, (SOCKADDR*)&clntAdr, &clntAdrSz);

		WaitForSingleObject(hMutex, INFINITE);
		clntSocks[clntCnt++] = hClntSock;
		ReleaseMutex(hMutex);

		hThread =
			(HANDLE)_beginthreadex(NULL, 0, HandleClnt, (void*)&hClntSock, 0, NULL);
		printf("Connected client IP: %s \n", inet_ntoa(clntAdr.sin_addr));
	}
	closesocket(hServSock);
	WSACleanup();
	return 0;
}

unsigned WINAPI HandleClnt(void * arg)
{
	SOCKET hClntSock = *((SOCKET*)arg);
	int strLen = 0, i;
	char msg[sizeof(packet)] = "";

	while (1)
	{
		SendMsg((char *)&packet, sizeof(packet));
		packet.Count++;
		if (packet.Count > 1000000000)
			packet.Count = 0;
		for (int i = 0; i < packet.row*packet.col; ++i)
		{
			packet.adc[i] = (int)((float)(i+(packet.Count%50)));
		}
	}
	WaitForSingleObject(hMutex, INFINITE);
	for (i = 0; i < clntCnt; i++)   // remove disconnected client
	{
		if (hClntSock == clntSocks[i])
		{
			while (i++ < clntCnt - 1)
				clntSocks[i] = clntSocks[i + 1];
			break;
		}
	}
	clntCnt--;
	ReleaseMutex(hMutex);
	closesocket(hClntSock);
	return 0;
}
void SendMsg(char * msg, int len)   // send to all
{
	int i;
	WaitForSingleObject(hMutex, INFINITE);
	for (i = 0; i < clntCnt; i++)
		send(clntSocks[i], msg, len, 0);

	ReleaseMutex(hMutex);
}
void ErrorHandling(char * msg)
{
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}