#include <winsock2.h>
#include <iostream>
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib,"..\\Debug\\ClientDll.lib")
using namespace std;
int InitSocket()
{
	WSADATA wsd;
	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
	{
		return -1;
	}
	return 1;
}
int CreateSocket(SOCKET &sHost)
{
	sHost = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == sHost)
	{
		WSACleanup();//�ͷ��׽�����Դ
		return  -1;
	}
	return 1;
}
int SetServerOrPort(SOCKADDR_IN &servAddr, u_short Port)
{
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servAddr.sin_port = htons((short)Port);
	return 1;
}
int ConnectServer(SOCKET &sHost, SOCKADDR_IN &servAddr, int &retVal)
{
	retVal = connect(sHost, (LPSOCKADDR)&servAddr, sizeof(servAddr));
	if (SOCKET_ERROR == retVal)
	{
		closesocket(sHost); //�ر��׽���
		WSACleanup(); //�ͷ��׽�����Դ
		return -1;
	}
	return 1;
}
int SendServerData(SOCKET &sHost, int &retVal, char *Buf, int BufSize)//�����˷�������
{
	retVal = send(sHost, Buf, strlen(Buf), 0);
	if (SOCKET_ERROR == retVal)
	{
		closesocket(sHost); //�ر��׽���
		WSACleanup(); //�ͷ��׽�����Դ
		return -1;
	}
	return 1;
}
int ReceiveServerData(SOCKET &sHost, char *BufRec, int BufRecSize)//���ܷ��������
{
	recv(sHost, BufRec, BufRecSize, 0); // ���շ������˵����ݣ� ֻ����5���ַ�
	return 1;
}
int ExitsHost(SOCKET &sHost)
{
	closesocket(sHost); //�ر��׽���
	WSACleanup(); //�ͷ��׽�����Դ
	return 1;
}
int main(int argc, char* argv[])
{
	//const int BUF_SIZE = 64;
	//WSADATA wsd; //WSADATA����
	//SOCKET sHost; //�������׽���
	//SOCKADDR_IN servAddr; //��������ַ
	//char buf[BUF_SIZE]; //�������ݻ�����
	//char bufRecv[BUF_SIZE];
	//int retVal; //����ֵ
	////��ʼ���׽��ֶ�̬��
	//InitSocket();
	//CreateSocket(sHost);
	//u_short Port = 4999;
	//SetServerOrPort(servAddr,Port);
	//ConnectServer(sHost,servAddr,retVal);
	//while (true)
	//{
	//	//���������������
	//	ZeroMemory(buf, BUF_SIZE);
	//	cout << " ���������������:  ";
	//	cin >> buf;
	//	SendServerData(sHost, retVal, buf, 64);
	//	ZeroMemory(bufRecv, BUF_SIZE);
	//	ReceiveServerData(sHost, bufRecv, 64);
	//	cout << endl << "�ӷ������������ݣ�" << bufRecv<<endl;
	//}
	////�˳�
	//ExitsHost(sHost);


	HMODULE hDll = LoadLibraryA("..\\Debug\\ClientDll.dll");
	typedef int(*Function)(int&, int&);
	typedef int(*_InitSocket)();
	typedef int(*_CreateSocket)(SOCKET&);
	typedef int(*_SetServerOrPort)(SOCKADDR_IN&, u_short);
	typedef int(*_ConnectServer)(SOCKET&, SOCKADDR_IN&, int&);
	typedef int(*_SendServerData)(SOCKET &, int &, char *, int);
	typedef int(*_ReceiveServerData)(SOCKET &, char *, int);
	typedef int(*_ExitsHost)(SOCKET&);

	int num1 = 30;
	int num2 = 40;
	SOCKET sHost;
	SOCKADDR_IN servAddr; //��������ַ
	char buf[64] = { 0 };
	char bufRecv[64] = { 0 };
	int retVal = 0;

	if (hDll)
	{
		Function function = (Function)GetProcAddress(hDll, "Function");
		int num = function(num1, num2);
		std::cout << "Ok " << num << std::endl;
		std::cout << "num1 " << num1 << std::endl;
		std::cout << "num2 " << num2 << std::endl;

		_InitSocket InitSocket = (_InitSocket)GetProcAddress(hDll, "InitSocket");
		int InitSocketReturn = InitSocket();
		if (InitSocketReturn == 1)
		{
			std::cout << "��ʼ�����ֽڳɹ���" << std::endl;
		}
		_CreateSocket CreateSocket = (_CreateSocket)GetProcAddress(hDll, "CreateSocket");
		int CreateSocketReturn = CreateSocket(sHost);
		if (CreateSocketReturn == 1)
		{
			std::cout << "�������ֽڳɹ���" << std::endl;
		}
		_SetServerOrPort SetServerOrPort = (_SetServerOrPort)GetProcAddress(hDll, "SetServerOrPort");
		u_short Port = 4999;
		int SetServerOrPortReturn = SetServerOrPort(servAddr, Port);
		if (SetServerOrPortReturn == 1)
		{
			std::cout << "���÷����ַ�Ͷ˿ڣ�" << std::endl;
		}
		_ConnectServer ConnectServer = (_ConnectServer)GetProcAddress(hDll, "ConnectServer");
		int ConnectServerReturn = ConnectServer(sHost, servAddr, retVal);
		if (ConnectServerReturn == 1)
		{
			std::cout << "���ӷ������ɹ���" << std::endl;
		}
		while (true)
		{
			//���������������
			ZeroMemory(buf, 64);
			cout << "���������������:  ";
			cin >> buf;
			_SendServerData SendServerData = (_SendServerData)GetProcAddress(hDll, "SendServerData");
			int SendServerDataReturn = SendServerData(sHost, retVal, buf, 64);
			if (SendServerDataReturn == 1)
			{
				std::cout << "�������������Ϣ�ɹ���" << std::endl;
			}

			ZeroMemory(bufRecv, 64);
			_ReceiveServerData ReceiveServerData = (_ReceiveServerData)GetProcAddress(hDll, "ReceiveServerData");
			int ReceiveServerDataReturn = ReceiveServerData(sHost, bufRecv, 64);
			if (ReceiveServerDataReturn == 1)
			{
				std::cout << "���ܷ�������Ϣ�ɹ���" << std::endl;
			}
			cout << "�ӷ������������ݣ�" << bufRecv << endl;
		}
		//�˳�
		_ExitsHost ExitsHost = (_ExitsHost)GetProcAddress(hDll, "ExitsHost");
		ExitsHost(sHost);
	}
	else
	{
		return 0;
	}
	FreeLibrary(hDll);
	hDll = NULL;


	return 0;
}