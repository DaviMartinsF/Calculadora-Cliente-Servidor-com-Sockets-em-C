// Davi Martins Figueiredo - 10374878
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUF_SIZE 1024

int main(int argc, char *argv[]) {
    // verifica se IP e porta foram passados como argumentos
    if (argc < 3) {
        printf("Uso: %s <ip> <porta>\n", argv[0]);
        return 1;
    }

    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUF_SIZE];

    // cria socket TCP
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Erro no socket");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[2])); // porta do servidor

    // converte IP string para binário
    if (inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <= 0) {
        perror("Endereço inválido");
        return -1;
    }

    // conecta ao servidor
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Erro na conexão");
        return -1;
    }

    printf("Conectado ao servidor %s:%s\n", argv[1], argv[2]);
    printf("Digite operações (ex: ADD 2 3) ou QUIT para sair.\n");

    while (1) {
        printf("> ");
        fgets(buffer, BUF_SIZE, stdin); // lê comando do usuário

        // envia comando QUIT e encerra
        if (strncmp(buffer, "QUIT", 4) == 0) {
            send(sock, buffer, strlen(buffer), 0);
            break;
        }

        // envia comando ao servidor
        send(sock, buffer, strlen(buffer), 0);

        // recebe resposta do servidor
        memset(buffer, 0, BUF_SIZE);
        int valread = read(sock, buffer, BUF_SIZE - 1);
        if (valread <= 0) break;

        buffer[valread] = '\0';
        printf("Servidor: %s", buffer);
    }

    close(sock); // fecha socket
    return 0;
}
