#include "KLib2Cpp.h"


#define _WINSOCK_DEPRECATED_NO_WARNINGS

//#pragma comment( lib, "ws2_32.lib")

using namespace std;



KLib2Cpp::KLib2Cpp()
{
	count = 0;	
	memset(&header, 0x7e7e7e7e, sizeof(header));
	memset(&tail, 0x81818181, sizeof(tail));
}

void KLib2Cpp::setupTCPIP()
{
	iRes = ::WSAStartup(MAKEWORD(0x02, 0x02), &wsdata);
	if (ERROR_SUCCESS != iRes) return; // 소켓 만들기
	hSocket = ::socket(PF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == hSocket) return; // 서버에 연결
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servAddr.sin_port = htons(3800);
	iRes = ::connect(hSocket, (LPSOCKADDR)&servAddr, sizeof(servAddr));
}

KLib2Cpp::KLib2Cpp(const char* _server_ip, int _port) : server_ip(_server_ip), port(_port)
{
	//tcp_client = new ofxTCPClient();
	memset(&header, 0x7e7e7e7e, sizeof(header));
	memset(&tail, 0x81818181, sizeof(tail));
	count = 0;
}

KLib2Cpp::~KLib2Cpp()
{	 

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
	forceData = new double* [row];
	for (int i = 0; i < row; i++)
	{
		adc[i] = new int[col];
		forceData[i] = new double[col];
	}
}

bool KLib2Cpp::start()
{
	buf = new char[2 << 24];
	int length = 0;
	while (1)
	{
		setupTCPIP();

		delete(buf);
		buf = new char[2 << 24];
		int npacket = ::recv(hSocket, buf, 2<<24, 0);

		//int npacket = tcp_client->receiveRawBytes(buf, MAX_PACKET);

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

	bufLength = row * col + 200;

	delete(buf);
	if (bufLength < length) {
		dataType = "Force";
		bufLength = row * col * 8 + 100;
	}
	else {
		dataType = "Raw";
	}
	buf = new char[bufLength];		

	return true;
}

bool KLib2Cpp::stop()
{
	::closesocket(hSocket);
	return true;
}

int** KLib2Cpp::read()
{
	char x;
	//// 데이터를 받는다. 에러가 발생하면 멈춘다.
	if (recv(hSocket, &x, sizeof(char), 0) == SOCKET_ERROR)
	{
		// 에러 콘솔 출력
		cout << "error" << endl;
		stop();
		return NULL;
	}

	int length = 0;

	int zeroCount = 0;

	while(1)
	{
		int npacket = recv(hSocket, buf, bufLength,0);

		if (npacket == 0) {
			memset(buf, 0, bufLength);
			zeroCount++;
			if (zeroCount > 100000) {
				return NULL;
			}
		}
		
		int bufheader = 0;
		if (0 == memcmp(buf, header, sizeof(header)))
		{
			memcpy(&length, &buf[4], sizeof(length));
			if(0 == memcmp(&buf[length + 4], tail, sizeof(tail)))
				break;
		}
	}

	memcpy(&count, &buf[8], sizeof(count));

	adc = new int*[row];
	for (int i = 0; i < row; ++i)
	{
		adc[i] = new int[col];
		for (int j = 0; j < col; ++j)
		{
			adc[i][j] = (int)(unsigned char)buf[i*col+j + 96];
		}
	}

	return adc;
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

double** KLib2Cpp::forceRead()
{
	char x;
	//// 데이터를 받는다. 에러가 발생하면 멈춘다.
	if (recv(hSocket, &x, sizeof(char), 0) == SOCKET_ERROR)
	{
		// 에러 콘솔 출력
		cout << "error" << endl;
		stop();
		return NULL;
	}

	int length = 0;

	int zeroCount = 0;

	while (1)
	{
		int npacket = recv(hSocket, buf, bufLength, 0);

		if (npacket == 0) {
			memset(buf, 0, bufLength);
			zeroCount++;
			if (zeroCount > 100000) {
				return NULL;
			}
		}

		int bufheader = 0;
		if (0 == memcmp(buf, header, sizeof(header)))
		{
			memcpy(&length, &buf[4], sizeof(length));
			if (0 == memcmp(&buf[length + 4], tail, sizeof(tail)))
				break;
		}
	}

	memcpy(&count, &buf[8], sizeof(count));

	forceData = new double* [row];
	for (int i = 0; i < row; ++i)
	{
		forceData[i] = new double[col];
		for (int j = 0; j < col; ++j)
		{
			std::memcpy(&forceData[i][j], &buf[(i * col + j) * 8 + 96], 8);
		}
	}

	return forceData;
}

void KLib2Cpp::printForceData()
{
	for (int i = 0; i < row; ++i)
	{
		for (int j = 0; j < col; ++j)
		{
			printf("%.3f ", forceData[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}