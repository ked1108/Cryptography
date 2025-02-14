// Client code
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

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

	int client_fd;
	struct sockaddr_in server_addr;
	char buffer[BUFFER_SIZE];

	// Create TCP socket
	client_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (client_fd == -1) {
		perror("Socket creation failed");
		return 1;
	}

	// Set up server address
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(PORT);

	// Connect to server
	if (connect(client_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
		perror("Connect failed");
		close(client_fd);
		return 1;
	}

	std::cout << "Connected to server." << std::endl;

	// Communicate with server
	while (true) {
		std::cout << "Enter message: ";
		std::cin >> buffer;
		std::string encrypted_message = encrypt(buffer, encryption);
		strcpy(buffer, encrypted_message.c_str());


		if (std::strcmp(buffer, "exit") == 0) {
			break;
		}

		// Send message to server
		if (write(client_fd, buffer, strlen(buffer)) == -1) {
			perror("Write error");
			break;
		}

		// Read response from server
		memset(buffer, 0, BUFFER_SIZE);
		ssize_t bytes_read = read(client_fd, buffer, BUFFER_SIZE - 1);
		if (bytes_read <= 0) {
			if (bytes_read == 0)
				std::cout << "Server disconnected." << std::endl;
			else
				perror("Read error");
			break;
		}

		std::cout << "Server response: " << buffer << std::endl;
		std::cout << "Decrypted response: " << decrypt(buffer, encryption) << std::endl;
	}

	close(client_fd);
	return 0;
}


