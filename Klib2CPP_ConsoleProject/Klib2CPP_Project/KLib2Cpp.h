#ifndef KLIB2CPP_H
#define KLIB2CPP_H

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#pragma comment(lib,"ws2_32.lib")

#include <string>
#include <vcruntime_string.h>
#include <stdio.h>
#include <cstring>
#include <string>

#include <iostream>
#include <conio.h>

#include <WinSock2.h>

struct COM_PACKET {
	CHAR Header[4];
	INT Count;
	INT NofDevice;
	INT row;
	INT col;
	CHAR Tail[4];
};

class KLib2Cpp
{
private:
	//ofxTCPClient* tcp_client;
	WSADATA wsdata;
	int iRes;
	SOCKET hSocket;
	SOCKADDR_IN  servAddr;

	COM_PACKET* packet;
	std::string server_ip;
	int port;
	char* buf;

	char header[4];
	char tail[4];

	void init();
	void creatarray();
	void setupTCPIP();

public:
	KLib2Cpp();
	KLib2Cpp(const char*,int);
	
	~KLib2Cpp();

	bool start();
	bool stop();
	int** read();

	int** adc;
	int row;
	int col;
	int count;
	int bufLength;

	int getport() { return port; };
	void setport(int _port) { port = _port; }
	std::string getserver_ip() { return server_ip; }
	void setserver_ip(std::string _server_ip) { server_ip = _server_ip; }
	int getcount() { return count; }

	void printadc();
	void printbyte();
};

#endif