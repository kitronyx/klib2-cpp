#-*- coding: utf-8 -*-
import os
import sys
import time
import numpy as np
import csv
import datetime
import time
import binascii
import base64

from socket import *
from select import select

reload(sys)
sys.setdefaultencoding('utf-8')

import codecs

class KLib():
    def __init__(self,_server_ip = "127.0.0.1", _port = 3800):
        self.nrow = 0
        self.ncol = 0
        self.datasize = 0
        self.adc = None
        self.port = _port
        self.server_ip = _server_ip
        self.device = ""
        self.sensor = ""
        self.adc = []
        
        self.buf = None
        self.result = None
        self.BUFSIZE = 5000
        self.addr = None
        self.client_socket = None

        self.client_socket_connection = False

        self.header = "7e7e7e7e".decode("hex")
    #TcpIP 연결 시도
    def init(self):
        try:
            self.addr = (self.server_ip, self.port) #server address 정보
            self.client_socket = socket(AF_INET, SOCK_STREAM) #소켓 정보
            
            self.client_socket.connect(self.addr) # tcpip연결
        except Exception as e:
            print('Failed to connect TCP/IP!')
            self.client_socket_connection = False
            return False
        self.client_socket_connection = True

        resp = self.client_socket.recv(self.BUFSIZE) #버퍼 받기

        self.buf = resp

        #header가 2개 이상이 아닌경우 패킷이 다안들어왔을 가능성이 있음
        while(1):
            if(self.buf.count(self.header) > 1): 
                break
            resp = self.client_socket.recv(self.BUFSIZE)
            self.buf = self.buf + resp
        
        #header 위치 찾기
        sp = self.buf.find(self.header)
        #header, tail을 뺀 버퍼를 result에 집어넣음
        self.result = self.buf[sp+4:sp+4996]

        self.device = self.result[4:28]
        self.sensor = self.result[28:52]
        self.nrow = int(binascii.hexlify(self.result[80]),16)
        print(self.nrow)
        self.ncol = int(binascii.hexlify(self.result[84]),16)
        print(self.ncol)
        self.datasize = self.nrow * self.ncol

        # rawdata array 생성
        for i in range(96,self.datasize+96):
             self.adc.append(int(binascii.hexlify(self.result[i]),16))
               

    def check_tcp_connection(self):
        if(self.client_socket_connection == True):
            return True
        else:
            return False
    #서버와 tcp 연결 시도
    def start(self):
        self.init()
    #서봐의 tcp 연결 끊기
    def stop(self):
        self.client_socket.close()
        self.client_socket_connection = False

    #패킷읽기
    def read(self):
        resp = self.client_socket.recv(self.BUFSIZE)

        #패킷이 잘려져서 올수도 있기 때문에 버퍼를 뒤에 추가함
        self.buf = self.buf + resp
        #header가 2개 이상이 아닌경우 패킷이 다안들어왔을 가능성이 있음
        while(1):
            if(self.buf.count(self.header) > 1):
                break
            resp = self.client_socket.recv(self.BUFSIZE)
            self.buf = self.buf + resp
        #헤더 위치 찾기
        sp = self.buf.find(self.header)

        self.result = self.buf[sp+4:sp+4996]

        self.buf = self.buf[sp+4996:]

        for i in range(96,self.datasize+96):
             self.adc[i-96] = int(binascii.hexlify(self.result[i]),16)

    def printadc(self):
        for i in range(self.nrow):
            write_str = ""
            for j in range(self.ncol):
                write_str = write_str + " " + str(self.adc[i*self.ncol + j])
            print write_str
        print ""

if __name__ == "__main__":
    klib = KLib("127.0.0.1", 3800)
    
    klib.start()
    while(1):
        klib.read()
        klib.printadc()
