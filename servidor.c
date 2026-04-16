#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>         // close()
#include <arpa/inet.h>      // sockaddr_in, inet_addr

#define PORT 8080

int main() {
    int server_fd, client_socket;
    struct sockaddr_in server_addr;
    char *message = "Hola cliente, soy el servidor\n";

    // 1. Crear socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("Error al crear socket");
        exit(EXIT_FAILURE);
    }

    // 2. Configurar dirección del servidor
    server_addr.sin_family = AF_INET;          // IPv4
    server_addr.sin_addr.s_addr = INADDR_ANY;  // Acepta cualquier IP
    server_addr.sin_port = htons(PORT);        // Puerto

    // 3. Asociar socket al puerto (bind)
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error en bind");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // 4. Escuchar conexiones
    if (listen(server_fd, 3) < 0) {
        perror("Error en listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Servidor escuchando en puerto %d...\n", PORT);

    // 5. Aceptar cliente
    client_socket = accept(server_fd, NULL, NULL);
    if (client_socket < 0) {
        perror("Error en accept");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Cliente conectado\n");

    // 6. Enviar mensaje
    send(client_socket, message, strlen(message), 0);

    // 7. Cerrar sockets
    close(client_socket);
    close(server_fd);

    printf("Conexión cerrada\n");

    return 0;
}
