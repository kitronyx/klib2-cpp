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
            if (klibObject->read() == FALSE) {
                // Communication stop
                // 1. If ForceLAB2 or Snowforce3 interrupts API communication;
                // 2. cpp program's TCP Client Error;
                break;
            }
            klibObject->printadc();
            //Sleep(1);
            system("cls");
        }

        klibObject->stop();
    }
}