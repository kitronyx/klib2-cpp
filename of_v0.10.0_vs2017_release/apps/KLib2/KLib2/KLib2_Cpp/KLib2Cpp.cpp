#include "KLib2Cpp.h"

KLib2Cpp::KLib2Cpp()
{
	tcp_client = new ofxTCPClient();
	server_ip = "127.0.0.1";
	port = 3800;
	count = 0;

	memset(&header, 0x7e7e7e7e, sizeof(header));
	memset(&tail, 0x81818181, sizeof(tail));
}

KLib2Cpp::KLib2Cpp(const char* _server_ip, int _port) : server_ip(_server_ip), port(_port)
{
	tcp_client = new ofxTCPClient();
	memset(&header, 0x7e7e7e7e, sizeof(header));
	memset(&tail, 0x81818181, sizeof(tail));
	count = 0;
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
	
	int length = 0;

	// After reading with as large a buffer as possible at the time of the first transmission, 
	// receive the row and col sizes, and later decide the buffer size and send & receive.
	unsigned int startBufLength = 2<<24;
	buf = new char[startBufLength];

	while (1)
	{
		int npacket = tcp_client->receiveRawBytes(buf, startBufLength);

		int bufheader = 0;
		if (0 == memcmp(buf, header, sizeof(header)))
		{
			memcpy(&length, &buf[4], sizeof(length));
			if (0 == memcmp(&buf[length + 4], tail, sizeof(tail)))
				break;
		}
	}

	memcpy(&row,  &buf[88], sizeof(row));
	memcpy(&col, &buf[92], sizeof(col));
	memcpy(&count, &buf[8], sizeof(count));

	// row*col + etc
	bufLength = row * col + 200;

	delete[](buf);
	buf = new char[bufLength];
	creatarray();

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
	
	int length = 0;

	while(1)
	{
		int npacket = tcp_client->receiveRawBytes(buf, bufLength);
		
		int bufheader = 0;
		if (0 == memcmp(buf, header, sizeof(header)))
		{
			memcpy(&length, &buf[4], sizeof(length));
			if(0 == memcmp(&buf[length + 4], tail, sizeof(tail)))
				break;
		}
	}

	memcpy(&count, &buf[8], sizeof(count));

	deleteAdc();
	adc = new int*[row];
	for (int i = 0; i < row; ++i)
	{
		adc[i] = new int[col];
		for (int j = 0; j < col; ++j)
		{
			adc[i][j] = (int)buf[i*col+j + 96];
		}
	}	
	return true;
}

void KLib2Cpp::deleteAdc() {

	for (int i = 0; i < row; ++i)
	{
		delete[](adc[i]);
	}
	delete[](adc);
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