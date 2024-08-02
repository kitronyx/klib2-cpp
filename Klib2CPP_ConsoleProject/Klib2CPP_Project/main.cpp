// ConsoleApplication6.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include "KLib2Cpp.h"

int main()
{
    while (1) {
        KLib2Cpp* klibObject = new KLib2Cpp();
    
        klibObject->start();

        while (1) {
            if (klibObject->dataType == "Raw") {
                int** _adc = klibObject->read();

                klibObject->printadc();

                printf("\n");
                //Sleep(1);
                system("cls");

                for (int i = 0; i < klibObject->row; ++i)
                {
                    delete[] _adc[i];
                }
                delete[]_adc;
            }
            else {
                double** forceData = klibObject->forceRead();

                klibObject->printForceData();

                printf("\n");
                //Sleep(1);
                system("cls");

                for (int i = 0; i < klibObject->row; ++i)
                {
                    delete[] forceData[i];
                }
                delete[]forceData;
            }
            
        }

        klibObject->stop();
    }
}