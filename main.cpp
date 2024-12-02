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
		//closesocket(listening);
		//WSACleanup();
		return 1;
	}

	std::cout << "Server is listening on port 8000...\n";

	//wait for connection
	sockaddr_in client;
	int clientSize = sizeof(client);

	SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientSize);
	if (clientSocket == INVALID_SOCKET) {
		std::cerr << "Can't accept client!" << std::endl;
		closesocket(listening);
		WSACleanup();
		return 1;
	}

	char host[NI_MAXHOST];        // Client's remote name
	char service[NI_MAXSERV];     // Service (i.e.port) the client is connected on

	ZeroMemory(host, NI_MAXHOST);
	ZeroMemory(service, NI_MAXSERV);

	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0) {
		std::cout << "Client connected on port" << service << std::endl;
	}

	// buffer for receiving client data
	char buf[4096];
	ZeroMemory(buf, 4096);

	//recieve client's HTTP request
	int bytesReceived = recv(clientSocket, buf, 4096, 0);
	if (bytesReceived == SOCKET_ERROR) {
		std::cerr << "Error in recv()" << std::endl;
	}
	else {

		//Print the HTTP request
		std::cout << "\nIncoming HTTP Request:\n" << std::string(buf, 0, bytesReceived) << std::endl;


		// Create a basic HTTP response
		std::string response = "HTTP/1.1 200 OK\r\n";
		response += "Content-Type: text/html\r\n";
		response += "Connection: close\r\n";
		response += "\r\n";
		response += "<html><body><h1>Welcome to your HTTP server!</h1></body></html>";

		//Send the response
		send(clientSocket, response.c_str(), response.size(), 0);

	}

	

	//cleanup
	closesocket(clientSocket);
	closesocket(listening);
	WSACleanup();
	return 0;



}