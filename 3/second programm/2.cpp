#include <iostream>
#include <fstream>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <cstring>

const int BUFFER_SIZE = 10;

const char* SOCKET_FILE = "../socket/file_socket";

int main(int argc, char *argv[]) {
    printf("%s\n", "Programm-2: launched");

    // Create a socket
    int sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock == -1) {
        printf("Programm-2: ERROR! Can`t create a socket\n");
        perror("");
        return 1;
    }

    printf("%s\n", "Programm-2: Succesfully created a socket"); // logs

    // Parse socket address from file
    sockaddr_un addr;
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, SOCKET_FILE);

    printf("%s\n", "Programm-2: parsed socket address"); // logs

    unlink(SOCKET_FILE); // open up socket port if it`s locked

    // Bind to the socket
    if (bind(sock, (sockaddr*)&addr, sizeof(addr)) == -1) {
        printf("Programm-2: ERROR! Failed binding to socket\n");
        perror("");
        return 1;
    }

    printf("%s\n", "Programm-2: Succesfully binded to the socket"); //logs

    // Listen to the socket
    if (listen(sock, 1) == -1) {
        printf("Programm-2: ERROR! Failed listening to socket\n");
        perror("");
        return 1;
    }

    printf("%s\n", "Programm-2: Succesfully listened to the socket"); // logs

    // Create first child process
    int pid1 = fork();
    if (pid1 == 0) {
        execl(argv[1], argv[1], argv[2], (char*)NULL);
    }

    printf("%s\n", "Programm-2: Succesfully created first child");

    // Create second child process
    int pid2 = fork();
    if (pid2 == 0) {
        execl(argv[1], argv[1], argv[3], (char*)NULL);
    }

    printf("%s\n", "Programm-2: Succesfully created second child");

    // Open output file
    std::ofstream output("output.txt");

    printf("%s\n", "Programm-2: Succesfully opened output file");

    // Accept connections from the child processes
    // Примечание: accept() вытаскивает первый запрос
    // из очереди на подключение. Таким образом в переменных
    // conn1 и conn2 получим два разных клиента, из них
    // можно читать информацию без проверки на пересечения - 
    // пересечений не будет.
    int conn1 = accept(sock, NULL, NULL);
    int conn2 = accept(sock, NULL, NULL);

    printf("%s\n", "Programm-2: Succesfully made connections");

    // Read data from the child processes and perform XOR operation
    char buffer1[BUFFER_SIZE];
    char buffer2[BUFFER_SIZE];
    while (true) {
        printf("%s\n", "Programm-2: reading chunk...");
        int bytes1 = recv(conn1, buffer1, BUFFER_SIZE, 0);
        int bytes2 = recv(conn2, buffer2, BUFFER_SIZE, 0);

        if(bytes1 <= 0 && bytes2 <= 0){
            printf("%s\n", "Programm-2: leaving...");
            break;
        }

        for (int i = 0; i < bytes1 || i < bytes2; ++i) {
            if(i < bytes1 && i < bytes2){
                output << char(buffer1[i] ^ buffer2[i]);
                continue;
            } 
            if(i < bytes1){
                output << char(buffer1[i]);
                continue;
            } 
            output << char(buffer2[i]);
        }
    }

    printf("%s\n", "Programm-2: Succes");

    // Close the connections and the socket
    close(conn1);
    close(conn2);
    close(sock);

    return 0;
}
