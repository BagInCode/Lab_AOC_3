#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include <iostream>

#pragma warning(disable: 4996)

using namespace std;

int main()
{
	WSAData wsaData;
	WORD DllVersion = MAKEWORD(2, 1);

	if (WSAStartup(DllVersion, &wsaData))
	{
		cout << "Error\n";
		exit(1);
	}

	SOCKADDR_IN addr;
	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(1111);
	addr.sin_family = AF_INET;

	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);
	bind(sListen, (SOCKADDR*)&addr, sizeof(addr));
	listen(sListen, SOMAXCONN);

	SOCKET newConnection;
	newConnection = accept(sListen, (SOCKADDR*)&addr, &sizeofaddr);

	if (newConnection == 0)
	{
		cout << "Error #2\n";
	}else 
	{
		cout << "Client connected...\n";

		while (1)
		{
			char type;
			recv(newConnection, &type, sizeof(type), NULL);

			char message[256];

			int Length = 0;

			if (type == 0)
			{
				Length = 4;
			}
			if (type == 1)
			{
				Length = 16;
			}
			if (type == 2)
			{
				Length = 64;
			}
			if (type == 3)
			{
				Length = 256;
			}

			recv(newConnection, message, sizeof(type)*Length, NULL);
			send(newConnection, message, sizeof(type)*Length, NULL);
		}
	}

	system("pause");

	return 0;
}