#include <iostream>
#include <WS2tcpip.h> 
#pragma comment(lib, "ws2_32.lib")

int main() {

	//initialise winsock
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	int wsResult = WSAStartup(ver, &wsData);
	if (wsResult != 0) {
		std::cerr << "Can't initialize Winsock! Error: " << wsResult << std::endl;
		return 1;
	}

	// Create a socket
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET) {
		std::cerr << "Can't create socket! Error:" << WSAGetLastError() << std::endl;
		WSACleanup();
		return 1;
	}

	//binds the socket to an IP Address and port
	sockaddr_in hint;
	hint.sin_family = AF_INET;					// IPv4
	hint.sin_port = htons(8080);				// Port 8080
	hint.sin_addr.S_un.S_addr = INADDR_ANY;		//Binds to any local address

	if (bind(listening, (sockaddr*)&hint, sizeof(hint)) == SOCKET_ERROR) {
		std::cerr << "Can't bind socket! Error: " << WSAGetLastError() << std::endl;
		closesocket(listening);
		WSACleanup();
		return 1;
	}

	//Tell Winsock the socket is for listening
	if (listen(listening, SOMAXCONN) == SOCKET_ERROR) {
		std::cerr << "Can't listen! Error:" << WSAGetLastError() << std::endl;
		closesocket(listening);
		WSACleanup();
		return 1;
	}

	std::cout << "Server is listening on port 8000..." << std::endl;

	//cleanup
	closesocket(listening);
	WSACleanup();
	return 0;



}