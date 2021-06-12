#include <iostream>
#include "Server.hpp"

int main() {
	const int SERVER_PORT = 5000;
	IEngine::Net::Server server(SERVER_PORT);
	server.Run();
	std::cout << "Application Terminanted Normally." << std::endl;
	return 0;
}          