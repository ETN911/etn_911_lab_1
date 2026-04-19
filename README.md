# 0 Requisitos:

1\. Computador laptop con alguna de estas opciones:

1\. SO Linux Ubuntu

2\. Windows con WSL instalado

3\. Windows con Vbox instalado y sobre este maquina virtual con Ubuntu

2\. librerias de programación instaladas:

*sudo apt install build-essential*

# 1. Código: servidor TCP simple en C

Este servidor:

-   Escucha en un puerto (ej: 8080)
-   Acepta una conexión
-   Envía un mensaje
-   Cierra

[servidor.c](https://github.com/habasto/etn_911_lab_1/blob/main/servidor.c)

``` c
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
```

# 2. Compilar

En Linux:

```console
gcc servidor.c -o servidor
```

# 3. Ejecutar

```console
./servidor
```

Verás:

```console
Servidor escuchando en puerto 8080\...
```

# 4. Probar el servidor

Desde otra terminal:

```console
telnet localhost 8080
```

o:

```console
nc localhost 8080
```

Deberías recibir:

```console
Hola cliente, soy el servidor
```

# 5. Explicación paso a paso

## 1. *socket()*

``` c
socket(AF_INET, SOCK_STREAM, 0);
```


-   *AF_INET* → IPv4
-   *SOCK_STREAM* → TCP
-   Devuelve un descriptor (como un archivo)

Aquí nace el "endpoint" de red.

## 2. *sockaddr_in*

``` c
struct sockaddr_in server_addr;
```

Define:

-   IP
-   Puerto
-   Tipo de red

``` c
server_addr.sin_addr.s_addr = INADDR_ANY;
```

Escucha en todas las interfaces (localhost, IP privada, etc.)

``` c
server_addr.sin_port = htons(PORT);
```

Convierte a formato de red (big endian)

## 3. *bind()*

``` c
bind(server_fd, \...);
```

Une el socket a un puerto del sistema operativo.

Sin esto, el SO no sabe dónde escuchar.

## 4. *listen()*

``` c
listen(server_fd, 3);
```

Pone el socket en modo servidor.

-   *3* = tamaño de la cola de conexiones pendientes

## 5. *accept()*

``` c
client_socket = accept(server_fd, NULL, NULL);
```

Aquí pasa algo clave:

-   Se bloquea esperando cliente

-   Cuando alguien se conecta:

    -   crea un NUEVO socket (*client_socket*)
    -   el original sigue escuchando

## 6. *send()*

``` c
send(client_socket, message, strlen(message), 0);
```

Envía datos al cliente.

Es como escribir en un archivo, pero hacia la red.

## 7. *close()*

``` c
close(client_socket);

close(server_fd);
```

Libera recursos del sistema.

# 7. Trabajo práctico:

1.  Cambiar el mensaje
2.  Explica las estructuras del ejemplo
3.  Dale solución al problema de bloqueo del servidor
4.  Modifica la aplicación para que el servidor responda con la hora del
    servidor
5.  Modifica la aplicación para que el servidor responda con la memoria
    RAM libre del servidor
6.  Modifica la aplicación para que el servidor responda con la
    geolocalización del servidor basados en el IP (se necesita Internet)
