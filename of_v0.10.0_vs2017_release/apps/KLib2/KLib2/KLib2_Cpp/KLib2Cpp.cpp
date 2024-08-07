#include "KLib2Cpp.h"

KLib2Cpp::KLib2Cpp() : tcp_client(new ofxTCPClient()),
						server_ip("127.0.0.1"),
						port(3800),
						count(0),
						buf(nullptr),
						adc(nullptr),
						forceData(nullptr) {
	memset(&header, 0x7e7e7e7e, sizeof(header));
	memset(&tail, 0x81818181, sizeof(tail));
}

KLib2Cpp::KLib2Cpp(const char* _server_ip, int _port)
	: server_ip(_server_ip), port(_port), count(0), buf(nullptr), adc(nullptr), forceData(nullptr) {
	tcp_client = new ofxTCPClient();
	memset(&header, 0x7e7e7e7e, sizeof(header));
	memset(&tail, 0x81818181, sizeof(tail));
}

KLib2Cpp::~KLib2Cpp()
{
	stop();
	delete tcp_client;
	delete[] buf;
	deleteAdc();
	deleteForce();
}

void KLib2Cpp::init()
{
	row = packet->row;
	col = packet->col;
	creatarray();
}

void KLib2Cpp::creatarray()
{
	adc = new int* [row];
	forceData = new double* [row];
	for (int i = 0; i < row; i++)
	{
		adc[i] = new int[col];
		forceData[i] = new double[col];
	}
}

bool KLib2Cpp::start()
{
	if (!tcp_client->setup(server_ip, port)) {
		return false;
	}
	
	int length = 0;
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
	bufLength = row * col + 100;

	delete[] buf;
	buf = new char[bufLength];
	if (bufLength < length) {
		dataType = "Force";
		bufLength = row * col * 8 + 100;
	}
	else {
		dataType = "Raw";
	}

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

		if (npacket <= 0) {
			continue;
		}

		if (0 == memcmp(buf, header, sizeof(header)))
		{
			memcpy(&length, &buf[4], sizeof(length));
			if (0 == memcmp(&buf[length + 4], tail, sizeof(tail))) 
			{
				break;
			}
		}
	}

	memcpy(&count, &buf[8], sizeof(count));

	if (dataType == "Raw") {
		deleteAdc();
		adc = new int* [row];
		for (int i = 0; i < row; ++i)
		{
			adc[i] = new int[col];
			for (int j = 0; j < col; ++j)
			{
				adc[i][j] = (int)(unsigned char)buf[i * col + j + 96];
			}
		}
	}
	else {
		deleteForce();
		forceData = new double* [row];
		for (int i = 0; i < row; ++i)
		{
			forceData[i] = new double[col];
			for (int j = 0; j < col; ++j)
			{
				std::memcpy(&forceData[i][j], &buf[(i * col + j) * 8 + 96], 8);
			}
		}
	}
	return true;
}

void KLib2Cpp::deleteAdc() {
	for (int i = 0; i < row; ++i) 
	{
		delete[] adc[i];
	}
	delete[] adc;
}

void KLib2Cpp::deleteForce() {
	for (int i = 0; i < row; ++i) 
	{
		delete[] forceData[i];
	}
	delete[] forceData;
}


void KLib2Cpp::printbyte()
{

}