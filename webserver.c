#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdbool.h>

#define PORT 8080
#define BUFFER_LEN 1024

bool running = true; // Variável para controlar o loop do servidor

void serve_file(int client_fd, const char *filename);
void shutdown_server(int client_fd);

int main() {
    int server_fd, client_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_LEN] = {0};

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Erro ao criar socket");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Erro ao fazer bind");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("Erro ao escutar");
        exit(EXIT_FAILURE);
    }

    printf("Servidor rodando na porta %d\n", PORT);

    while (running) {
        client_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        if (client_fd < 0) {
            perror("Erro ao aceitar conexão");
            continue;
        }

        int byteCount = read(client_fd, buffer, sizeof(buffer));
        if (byteCount > 0) {
            printf("Requisição recebida:\n%s\n", buffer);
        }

        // Verifica se a requisição é para desligar o servidor
        if (strncmp(buffer, "POST /shutdown", 14) == 0) {
            shutdown_server(client_fd);
            break; // Sai do loop principal
        } else {
            serve_file(client_fd, "index.html");
        }

        close(client_fd);
    }

    close(server_fd);
    printf("Servidor desligado.\n");

    return 0;
}

void serve_file(int client_fd, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        char not_found_response[] = "HTTP/1.1 404 Not Found\r\n"
                                    "Content-Type: text/plain\r\n"
                                    "Content-Length: 13\r\n"
                                    "\r\n"
                                    "404 Not Found";
        write(client_fd, not_found_response, strlen(not_found_response));
        return;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    char *file_content = malloc(file_size + 1);
    fread(file_content, 1, file_size, file);
    fclose(file);
    file_content[file_size] = '\0';

    char response_header[BUFFER_LEN];
    snprintf(response_header, sizeof(response_header), 
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: %ld\r\n"
        "\r\n", file_size);

    write(client_fd, response_header, strlen(response_header));
    write(client_fd, file_content, file_size);
    free(file_content);
}

void shutdown_server(int client_fd) {
    char shutdown_message[] = "HTTP/1.1 200 OK\r\n"
                              "Content-Type: text/plain\r\n"
                              "Content-Length: 18\r\n"
                              "\r\n"
                              "Servidor desligando";
    write(client_fd, shutdown_message, strlen(shutdown_message));
    running = false; // Sai do loop principal
}
