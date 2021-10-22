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
            int** _adc = klibObject->read();

            if (_adc == NULL) {
                // Communication stop
                // 1. If ForceLAB2 or Snowforce3 interrupts API communication;
                // 2. cpp program's TCP Client Error;
                break;
            }
            for (int i = 0; i < klibObject->row; ++i)
            {
                for (int j = 0; j < klibObject->col; ++j)
                {
                    printf("%d ", _adc[i][j]);
                }
                printf("\n");
            }
            printf("\n");
            //Sleep(1);
            system("cls");

			for (int i = 0; i < klibObject->row; ++i)
			{
				delete[] _adc[i];
			}
			delete[]_adc;
        }

        klibObject->stop();
    }
}