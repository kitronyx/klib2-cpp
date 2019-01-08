#ifndef KLIB2CPP_H
#define KLIB2CPP_H

#include "ofxTCPClient.h"

#define MAX_PACKET 5000

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
};

class KLib2Cpp
{
private:
	ofxTCPClient* tcp_client;
	COM_PACKET* packet;
	std::string server_ip;
	int port;
	char buf[MAX_PACKET];

	char header[4];
	char tail[4];

	void init();
	void creatarray();

public:
	KLib2Cpp();
	KLib2Cpp(const char*,int);
	
	~KLib2Cpp();

	bool start();
	bool stop();
	bool read();

	int** adc;
	int row;
	int col;

	int getport() { return port; };
	void setport(int _port) { port = _port; }
	std::string getserver_ip() { return server_ip; }
	void setserver_ip(std::string _server_ip) { server_ip = _server_ip; }
	int getcount() { return packet->Count; }

	void printadc();
	void printbyte();
};

#endif