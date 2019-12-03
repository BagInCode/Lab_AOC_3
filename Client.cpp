#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include <iostream>
#include <fstream>
#include <chrono>

#pragma warning(disable: 4996)

using namespace std;

bool connectServer(SOCKADDR_IN & addr, SOCKET & Connection, WSAData &wsaData, WORD& DllVersion)
{
	DllVersion = MAKEWORD(2, 1);

	if (WSAStartup(DllVersion, &wsaData))
	{
		return 1;
	}
	
	int sizeofaddr = sizeof(addr);
	
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(1111);
	addr.sin_family = AF_INET;

	Connection = socket(AF_INET, SOCK_STREAM, NULL);
	
	if (connect(Connection, (SOCKADDR*)&addr, sizeof(addr)))
	{
		return 0;
	}

	return 1;
}

double getPing(SOCKET& Connection, int messageLength = 1)
{
	char message[256];
	char type = 0;

	if (messageLength == 16)
	{
		type = 1;
	}
	if (messageLength == 64)
	{
		type = 2;
	}
	if (messageLength == 256)
	{
		type = 3;
	}

	send(Connection, &type, sizeof(type), NULL);

	chrono::duration < double > elapsed;
	auto start = chrono::system_clock::now();

	send(Connection, message, sizeof(type)*messageLength, NULL);
	recv(Connection, message, sizeof(type)*messageLength, NULL);

	elapsed = chrono::system_clock::now() - start;

	return round(elapsed.count() * 1e6);
}

void process()
{
	SOCKADDR_IN addr;
	SOCKET Connection;
	WSAData wsaData;
	WORD DllVersion;

	if (!connectServer(addr, Connection, wsaData, DllVersion))
	{
		cerr << "Error!\n";
		exit(1);
	}
	
	ofstream out;
	out.open("E:\\pingFile.txt");

	int countRepites[5] = { 10, 50, 100, 1000, 10000 };
	int Length[4] = { 4, 16, 64, 256 };

	for (int i = 0; i < 5; i++)
	{
		cerr << "!!\n";

		double averange[4] = { 0.0, 0.0, 0.0, 0.0 };

		out << "\nStart testing ping with " << countRepites[i] << " repites:\n\n";

		for (int j = 0; j < 4; j++)
		{
			out << "\tStart testing with length " << Length[j] << " bites:\n\n";

			for (int k = 0; k < countRepites[i]; k++)
			{
				double thisPing = getPing(Connection, Length[j]);
				averange[j] += thisPing;

				out << "\t\tping " << round(thisPing) << " ms\n";
			}
		}

		out << "\n";
		out << "---------------------------------------\n";
		out << "Result:\n";
		out << "\tLength 4,   averange ping " << round(averange[0] / countRepites[i]) << " ms\n";
		out << "\tLength 16,  averange ping " << round(averange[1] / countRepites[i]) << " ms\n";
		out << "\tLength 64,  averange ping " << round(averange[2] / countRepites[i]) << " ms\n";
		out << "\tLength 256, averange ping " << round(averange[3] / countRepites[i]) << " ms\n";
		out << "---------------------------------------\n";
	}

	return;
}

int main()
{
	process();

	return 0;
}