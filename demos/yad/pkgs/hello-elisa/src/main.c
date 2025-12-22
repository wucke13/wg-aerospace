#include <arpa/inet.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

static uint16_t const port = 5001;
static uintptr_t const buffer_size = 256;

void process_message(char *, int);

// the fds must be global so that the signhandler can close them
int server_fd, client_fd;

// conduct a clean shutdown
void clean_shutdown() {
  close(server_fd);
  close(client_fd);
  exit(EXIT_SUCCESS);
}

// Close the server socket upon SIGTERM
void handle_sigint(int sig) {
  printf("received signal %d, exiting...", sig);
  clean_shutdown();
}

int main() {
  signal(SIGINT, handle_sigint); // Register signal handler for SIGINT

  struct sockaddr_in server_address;
  char buffer[buffer_size];
  memset(buffer, 0, buffer_size);

  // Create a socket
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }

  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(port);
  server_address.sin_addr.s_addr = INADDR_ANY;

  if (bind(server_fd, (struct sockaddr *)&server_address,
           sizeof(server_address)) < 0) {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }

  if (listen(server_fd, 3) < 0) {
    perror("listen");
    exit(EXIT_FAILURE);
  }

  printf("Server listening on port %d...\n", port);

  while (1) {
    struct sockaddr_in client_address;
    socklen_t conn_len = sizeof(server_address);

    client_fd =
        accept(server_fd, (struct sockaddr *)&client_address, &conn_len);
    if (client_fd < 0) {
      perror("accept");
      continue;
    }

    printf("Connection accepted from IP address %s and port %d...\n",
           inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));

    // Read data from the client
    int bytes_received = read(client_fd, buffer, buffer_size);
    if (bytes_received < 0) {
      perror("read failed");
      continue;
    }

    // Process the message
    process_message(buffer, client_fd);

    memset(buffer, 0, buffer_size);

    close(client_fd);
    client_fd = -1;
  }

  clean_shutdown();
}

void process_message(char *message, int socket) {
  char *command = strtok(message, "\n");
  printf("Processing command: %s", command);
  if (command == NULL) {
    // Handle invalid input
    printf("Invalid input. Please enter a valid command.\n");
    return;
  }

  if (strcmp(command, "exit") == 0) {
    printf("Exiting...\n");
    clean_shutdown();
    return;
  } else if (strcmp(command, "ping") == 0) {
    char *pong = "Pong!";
    send(socket, pong, strlen(pong), 0);
  } else {
    printf("Invalid command: %s\n", command);
  }
}
