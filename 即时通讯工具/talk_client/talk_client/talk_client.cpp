#include "stdafx.h"

#include<WINSOCK2.H>
#include<STDIO.H>
#include<iostream>
#include<cstring>
#include <string>
using namespace std;
#pragma comment(lib, "ws2_32.lib")

#define BUF_SIZE 1024

typedef struct test{//����һ���ṹ�壬�������̴߳�����
	SOCKET sclient;
}Test;
DWORD WINAPI FunSend(LPVOID lpParamter)//�������ݵ��߳�
{
		Test *pt=(Test*)lpParamter;
		string sLine;
		getline(cin,sLine);
		string data=sLine;
		const char * sendData;
		sendData = data.c_str();   //�ַ���ת��Ϊָ�� 
		

		send(pt->sclient, sendData, strlen(sendData), 0);
		Sleep(5000);
		//send()������������ָ����socket�����Է�����
    return 0;
}
//
DWORD WINAPI FunRev(LPVOID lpParamter){//�������ݵ��߳�
		char buffer[255];
		Test *pt=(Test*)lpParamter;
		int ret=recv(pt->sclient, buffer, BUF_SIZE, 0);  
		if(ret>0){
			buffer[ret]='\0';
			printf("server:%s\n",buffer);
		}
		Sleep(5000);
		return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
	WORD sockVersion = MAKEWORD(2, 2);//ʹ��socket�Ĺ̶�д��
	WSADATA data;
	if(WSAStartup(sockVersion, &data)!=0)
	{
		return 0;
	}
	//while(true){
		SOCKET sclient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//�½�һ��SOCKET����������õĲ�����������ʲôЭ���
		if(sclient == INVALID_SOCKET)
		{
			printf("invalid socket!");
			return 0;
		}
		
		sockaddr_in serAddr;//����һ���׽��֣��׽����������SOCKET�����ԣ�����IP���˿ڵ�
		serAddr.sin_family = AF_INET;
		serAddr.sin_port = htons(8888);
		//serAddr.sin_port = htons(2049);
		serAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
		//serAddr.sin_addr.S_un.S_addr = inet_addr("192.32.7.1");
		if(connect(sclient, (sockaddr *)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
		{  //����ʧ�� 
			printf("connect error !");
			closesocket(sclient);
			return 0;
		}
		else{
			printf("Hellow\n");
			
		}


		//��sclient���sock�����߳�
		Test t;
		t.sclient=sclient;
		while(true){
		HANDLE hThreadSend = CreateThread(NULL, 0, FunSend, (LPVOID)&t, 0, NULL);//���������շ��߳�
		HANDLE hThreadRev = CreateThread(NULL, 0, FunRev, (LPVOID)&t, 0, NULL);
		


		CloseHandle(hThreadSend);//�ر��߳�
		CloseHandle(hThreadRev);
		}	
	closesocket(sclient);//�ر�socket
	
	WSACleanup();
	
	return 0;
}

