#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>


#define PORT 8080
#define BUFFER_LEN 1024     

int main()
{
    int server_fd, client_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_LEN] = {0};
    const char* html_body = 
    "<html>"
    "<head><title>Test</title></head>"
    "<body><h1>Welcome to my Web Server!</h1></body>"
    "</html>";

    char html_response[BUFFER_LEN];

    snprintf(html_response, sizeof(html_response), 
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: %d\r\n"
        "\r\n"
        "%s", (int)strlen(html_body), html_body);

        if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0){
            perror("socket failed");
            exit(EXIT_FAILURE);
        }

    int opt = 1;
    // Set socket options
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEADDR, &opt, sizeof(opt));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    // Bind the socket to the address
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd,3);
    
    printf("Listening on port %d\n", PORT);
    while(1)
    {
        client_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        int byteCount = read(client_fd, buffer, sizeof(buffer));
        if(byteCount > 0)
        {
            printf("Received request:\n%s\n", buffer);
        }
        write(client_fd, html_response, strlen(html_response));
        printf("Response sent to client.\n");
        close(client_fd);
    }

    close(server_fd);

return 0;
}

// Criar as variáveis
// Inicializar
// Abrir o socket
// Bind do Socket com interface de rede
// Escutar o socket
// Loop para conexões
//Concurrência (Usando threads ou select)

