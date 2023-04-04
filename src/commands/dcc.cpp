#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <errno.h>
#include <time.h>

#define MAX_PENDING 5
#define BUFFER_SIZE 1024

void handle_dcc_transfer(int sockfd, struct sockaddr_in client_addr, char* filename, int filesize, int dcc_port) {
	// Create a new socket for the DCC transfer
	int dcc_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (dcc_sockfd < 0) {
		printf("Error creating DCC socket\n");
		return;
	}

	// Set up the DCC server address
	struct sockaddr_in dcc_server_addr;
	dcc_server_addr.sin_family = AF_INET;
	dcc_server_addr.sin_port = htons(dcc_port);
	dcc_server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	// Bind the DCC socket to the server address
	if (bind(dcc_sockfd, (struct sockaddr*)&dcc_server_addr, sizeof(dcc_server_addr)) < 0) {
		printf("Error binding DCC socket\n");
		return;
	}

	// Listen for a connection from the client
	if (listen(dcc_sockfd, MAX_PENDING) < 0) {
		printf("Error listening on DCC socket\n");
		return;
	}

	// Send the DCC transfer request to the client
	char buffer[BUFFER_SIZE];
	snprintf(buffer, BUFFER_SIZE, "DCC SEND \"%s\" %d %d %d\r\n", filename, htonl(client_addr.sin_addr.s_addr), dcc_port, htonl(filesize));
	send(sockfd, buffer, strlen(buffer), 0);

	// Accept the client connection
	struct sockaddr_in dcc_client_addr;
	socklen_t dcc_client_len = sizeof(dcc_client_addr);
	int dcc_client_sockfd = accept(dcc_sockfd, (struct sockaddr*)&dcc_client_addr, &dcc_client_len);
	if (dcc_client_sockfd < 0) {
		printf("Error accepting DCC connection\n");
		return;
	}

	// Open the file for reading
	FILE* file = fopen(filename, "rb");
	if (!file) {
		printf("Error opening file for reading\n");
		return;
	}

	// Read and send the file data in chunks
	char file_buffer[BUFFER_SIZE];
	int bytes_sent = 0;
	while (bytes_sent < filesize) {
		int bytes_read = fread(file_buffer, 1, BUFFER_SIZE, file);
		int bytes_remaining = filesize - bytes_sent;
		int bytes_to_send = (bytes_read < bytes_remaining) ? bytes_read : bytes_remaining;
		if (send(dcc_client_sockfd, file_buffer, bytes_to_send, 0) < 0) {
			printf("Error sending file data\n");
			break;
		}
		bytes_sent += bytes_to_send;
	}

	// Close the file and sockets
	fclose(file);
	close(dcc_client_sockfd);
	close(dcc_sockfd);
}

void handle_message(int sockfd, char* buffer, struct sockaddr_in client_addr) {
	// Parse the message and extract the filename and filesize
	char command[BUFFER_SIZE];
	char filename[BUFFER_SIZE];
	int filesize;

	if (sscanf(buffer, "%s %s %d", command, filename, &filesize) != 3) {
		printf("Error parsing DCC message\n");
		return;
	}

	// Handle the DCC transfer request
	if (strcmp(command, "DCC") == 0) {
		handle_dcc_transfer(sockfd, client_addr, filename, filesize, 12345);
	}
}

int main(int argc, char** argv) {
	// Create the socket
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		printf("Error creating socket\n");
		return 1;
	}

	// Bind the socket to a port
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(1234);
	if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
		printf("Error binding socket\n");
		return 1;
	}

	// Listen for incoming connections
	if (listen(sockfd, MAX_PENDING) < 0) {
		printf("Error listening on socket\n");
		return 1;
	}

	// Loop to handle incoming messages
	while (1) {
		// Accept a new connection
		struct sockaddr_in client_addr;
		socklen_t client_len = sizeof(client_addr);
		int client_sockfd = accept(sockfd, (struct sockaddr*)&client_addr, &client_len);
		if (client_sockfd < 0) {
			printf("Error accepting connection\n");
			continue;
		}

		// Receive a message from the client
		char buffer[BUFFER_SIZE];
		int bytes_received = recv(client_sockfd, buffer, BUFFER_SIZE, 0);
		if (bytes_received < 0) {
			printf("Error receiving message\n");
			close(client_sockfd);
			continue;
		}

		// Handle the message
		handle_message(client_sockfd, buffer, client_addr);

		// Close the client socket
		close(client_sockfd);
	}

	// Close the server socket
	close(sockfd);

	return 0;
}
