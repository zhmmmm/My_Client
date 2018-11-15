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
		WSACleanup();//释放套接字资源
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
		closesocket(sHost); //关闭套接字
		WSACleanup(); //释放套接字资源
		return -1;
	}
	return 1;
}
int SendServerData(SOCKET &sHost, int &retVal, char *Buf, int BufSize)//向服务端发送数据
{
	retVal = send(sHost, Buf, strlen(Buf), 0);
	if (SOCKET_ERROR == retVal)
	{
		closesocket(sHost); //关闭套接字
		WSACleanup(); //释放套接字资源
		return -1;
	}
	return 1;
}
int ReceiveServerData(SOCKET &sHost, char *BufRec, int BufRecSize)//接受服务端数据
{
	recv(sHost, BufRec, BufRecSize, 0); // 接收服务器端的数据， 只接收5个字符
	return 1;
}
int ExitsHost(SOCKET &sHost)
{
	closesocket(sHost); //关闭套接字
	WSACleanup(); //释放套接字资源
	return 1;
}
int main(int argc, char* argv[])
{
	//const int BUF_SIZE = 64;
	//WSADATA wsd; //WSADATA变量
	//SOCKET sHost; //服务器套接字
	//SOCKADDR_IN servAddr; //服务器地址
	//char buf[BUF_SIZE]; //接收数据缓冲区
	//char bufRecv[BUF_SIZE];
	//int retVal; //返回值
	////初始化套结字动态库
	//InitSocket();
	//CreateSocket(sHost);
	//u_short Port = 4999;
	//SetServerOrPort(servAddr,Port);
	//ConnectServer(sHost,servAddr,retVal);
	//while (true)
	//{
	//	//向服务器发送数据
	//	ZeroMemory(buf, BUF_SIZE);
	//	cout << " 向服务器发送数据:  ";
	//	cin >> buf;
	//	SendServerData(sHost, retVal, buf, 64);
	//	ZeroMemory(bufRecv, BUF_SIZE);
	//	ReceiveServerData(sHost, bufRecv, 64);
	//	cout << endl << "从服务器接收数据：" << bufRecv<<endl;
	//}
	////退出
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
	SOCKADDR_IN servAddr; //服务器地址
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
			std::cout << "初始化套字节成功！" << std::endl;
		}
		_CreateSocket CreateSocket = (_CreateSocket)GetProcAddress(hDll, "CreateSocket");
		int CreateSocketReturn = CreateSocket(sHost);
		if (CreateSocketReturn == 1)
		{
			std::cout << "创建套字节成功！" << std::endl;
		}
		_SetServerOrPort SetServerOrPort = (_SetServerOrPort)GetProcAddress(hDll, "SetServerOrPort");
		u_short Port = 4999;
		int SetServerOrPortReturn = SetServerOrPort(servAddr, Port);
		if (SetServerOrPortReturn == 1)
		{
			std::cout << "设置服务地址和端口！" << std::endl;
		}
		_ConnectServer ConnectServer = (_ConnectServer)GetProcAddress(hDll, "ConnectServer");
		int ConnectServerReturn = ConnectServer(sHost, servAddr, retVal);
		if (ConnectServerReturn == 1)
		{
			std::cout << "连接服务器成功！" << std::endl;
		}
		while (true)
		{
			//向服务器发送数据
			ZeroMemory(buf, 64);
			cout << "向服务器发送数据:  ";
			cin >> buf;
			_SendServerData SendServerData = (_SendServerData)GetProcAddress(hDll, "SendServerData");
			int SendServerDataReturn = SendServerData(sHost, retVal, buf, 64);
			if (SendServerDataReturn == 1)
			{
				std::cout << "向服务器发送消息成功！" << std::endl;
			}

			ZeroMemory(bufRecv, 64);
			_ReceiveServerData ReceiveServerData = (_ReceiveServerData)GetProcAddress(hDll, "ReceiveServerData");
			int ReceiveServerDataReturn = ReceiveServerData(sHost, bufRecv, 64);
			if (ReceiveServerDataReturn == 1)
			{
				std::cout << "接受服务器消息成功！" << std::endl;
			}
			cout << "从服务器接收数据：" << bufRecv << endl;
		}
		//退出
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