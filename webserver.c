#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_LEN 1024     

int main()
{
    int server_fd, client_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_LEN] = {0};
    const char* html_body = 
    "<html>
        <head>
            <body><h1>WebServer em C!</h1>
            <p>Este é um exemplo de um servidor web simples em C.</p>
            </body>    
        </head>
    </html>";

    const char* html_response[BUFFER_LEN];
    snprintf(html_response, sizeof(html_response), 
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: %d\r\n"
        "\r\n"
        "%s", strlen(html_body), html_body);

        server_fd = socket(AF_INET, SOCK_STREAM, 0)

return 0;
}

// Criar as variáveis
// Inicializar
// Abris o socket
// Bind do Socket com interface de rede
// Escutar o socket
// Loop para conexões

