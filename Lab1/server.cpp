// Server code
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <string>

#include "encrypt.h"

#define PORT 8080
#define BUFFER_SIZE 256

int main() {
	int encryption = 0;
	std::cout << "Select Algorithm:\n";
	std::cout << "1) Caesar\n";
	std::cout << "2) Vigenere\n";
	std::cout << "3) Vernam\n";
	std::cout << "4) Playfair\n";
	std::cout << "5) Hill\n";
	std::cout << "6) Rail Fence\n";

	std::cin >> encryption;


	int server_fd, client_fd;
	struct sockaddr_in server_addr, client_addr;
	socklen_t client_len;
	char buffer[BUFFER_SIZE];

	// Create TCP socket
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd == -1) {
		perror("Socket creation failed");
		return 1;
	}

	const int enable = 1;
	if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
    perror("setsockopt(SO_REUSEADDR) failed");

	// Set up server address
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(PORT);

	// Bind the socket
	if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
		perror("Bind failed");
		close(server_fd);
		return 1;
	}

	// Listen for connections
	if (listen(server_fd, 5) == -1) {
		perror("Listen failed");
		close(server_fd);
		return 1;
	}

	std::cout << "Server listening on port " << PORT << std::endl;

	// Accept a client connection
	client_len = sizeof(client_addr);
	client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
	if (client_fd == -1) {
		perror("Accept failed");
		close(server_fd);
		return 1;
	}

	std::cout << "Client connected." << std::endl;

	// Communicate with client
	while (true) {
		memset(buffer, 0, BUFFER_SIZE);
		ssize_t bytes_read = read(client_fd, buffer, BUFFER_SIZE - 1);
		if (bytes_read <= 0) {
			if (bytes_read == 0)
				std::cout << "Client disconnected." << std::endl;
			else
				perror("Read error");
			break;
		}

		std::cout << "Received:\t" << buffer << std::endl;
		std::cout << "Decrypted:\t" << decrypt(buffer, encryption) << std::endl;

		std::string message = "";
		std::cout << "Enter Message:\t" ;
		std::cin >> message;
		std::string encrypted_message = encrypt(message, encryption);
		std::cout << "Encrypted message:\t" << encrypted_message << std::endl;

		if (write(client_fd, encrypted_message.c_str(), encrypted_message.length()) == -1) {
			perror("Write error");
			break;
		}
	}

	close(client_fd);
	close(server_fd);
	return 0;
}

