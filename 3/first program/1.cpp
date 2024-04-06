#include <iostream>
#include <fstream>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

// IO buffer size:
const int BUFFER_SIZE = 10;

// Socket file:
const char* SOCKET_FILE = "../socket/file_socket";

int main(int argc, char* argv[]) {
    int pid = getpid(); // Programm ID for logs

    printf("Programm-1 (pid=%d): launched\n", pid); // logs

    // File path should be provided
    if (argc != 2) {      
        printf("Programm-1 (pid=%d)): ERROR! File path not provided\n", pid);
        return 1;
    }

    // Create a socket
    int sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock == -1) {
        printf("Programm-1 (pid=%d)): ERROR! Can`t create a socket\n", pid);
        perror("");
        return 1;
    }

    printf("Programm-1 (pid=%d): created a socket\n", pid); // logs

    // Parsing socket address from file
    sockaddr_un addr;
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, SOCKET_FILE);

    printf("Programm-1 (pid=%d): parsed socket address\n", pid); // logs

    // Connect to the socket
    if (connect(sock, (sockaddr*)&addr, sizeof(addr)) == -1) {
        printf("Programm-1 (pid=%d)): ERROR! Can`t connect to a socket\n", pid);
        perror("");
        return 1;
    }
     
    printf("Programm-1 (pid=%d): connected to a socket\n", pid); // logs

    // Open the input file
    std::ifstream file;
    file.open((argv[1]));
    if (!file.is_open()) {
        printf("Programm-1 (pid=%d)): ERROR! Can`t open input file\n", pid);
        perror("");
        return 1;
    }

    printf("Programm-1 (pid=%d): opened input file '%s'\n", pid, argv[1]);

    // Read the file and send its content to the socket
    char buffer[BUFFER_SIZE];
    while (file.read(buffer, BUFFER_SIZE)) {
        send(sock, buffer, file.gcount(), 0);
    }
    send(sock, buffer, file.gcount(), 0); // remaining part

    printf("Programm-1 (pid=%d): Success reading input file and sending to socket!\n", pid);

    // Clean up
    file.close();
    close(sock);

    return 0;
}
