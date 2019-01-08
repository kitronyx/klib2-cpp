#include "KLib2Cpp.h"

KLib2Cpp::KLib2Cpp()
{
	tcp_client = new ofxTCPClient();
	server_ip = "127.0.0.1";
	port = 3800;

	memset(&header, 0x7e7e7e7e, sizeof(header));
	memset(&tail, 0x81818181, sizeof(tail));
}

KLib2Cpp::KLib2Cpp(const char* _server_ip, int _port) : server_ip(_server_ip), port(_port)
{
	tcp_client = new ofxTCPClient();
	memset(&header, 0x7e7e7e7e, sizeof(header));
	memset(&tail, 0x81818181, sizeof(tail));
}

KLib2Cpp::~KLib2Cpp()
{
	if(tcp_client->isConnected())
		tcp_client->close();
	tcp_client->~ofxTCPClient();
	delete tcp_client;
}

void KLib2Cpp::init()
{
	row = packet->row;
	col = packet->col;
	creatarray();
}

void KLib2Cpp::creatarray()
{
	adc = new int*[row];
	for (int i = 0; i < row; i++)
	{
		adc[i] = new int[col];
	}
}

bool KLib2Cpp::start()
{
	tcp_client->setup(server_ip, port);

	do 
	{
		int npacket = tcp_client->receiveRawBytes(buf, MAX_PACKET);
		packet = (COM_PACKET*)&buf;
		
	} while (memcmp(packet->Header, header, sizeof(packet->Header)) || memcmp(packet->Tail, tail, sizeof(packet->Tail)));
	
	init();

	return true;
}

bool KLib2Cpp::stop()
{
	if(tcp_client->isConnected())
		tcp_client->close();
	return true;
}

bool KLib2Cpp::read()
{
	if (!tcp_client->isConnected())
	{
		stop();
		return false;
	}
	do
	{
		int npacket = tcp_client->receiveRawBytes(buf, MAX_PACKET);
		packet = (COM_PACKET*)&buf;

	} while (memcmp(packet->Header, header, sizeof(packet->Header)) || memcmp(packet->Tail, tail, sizeof(packet->Tail)));
	
	
	for (int i = 0; i < row; ++i)
	{
		for (int j = 0; j < col; ++j)
		{
			adc[i][j] = packet->adc[i*col + j];
		}
	}

	return true;
}

void KLib2Cpp::printadc()
{
	for (int i = 0; i < row; ++i)
	{
		for (int j = 0; j < col; ++j)
		{
			printf("%d ", adc[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

void KLib2Cpp::printbyte()
{

}