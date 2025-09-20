// Davi Martins Figueiredo - 10374878
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 5050
#define BUF_SIZE 1024

int main(int argc, char *argv[]) {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUF_SIZE];

    // cria socket TCP
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Erro ao criar socket");
        exit(EXIT_FAILURE);
    }

    // configura endereço do servidor
    address.sin_family = AF_INET;           // IPv4
    address.sin_addr.s_addr = INADDR_ANY;   // aceita conexões de qualquer IP
    address.sin_port = htons((argc > 1) ? atoi(argv[1]) : PORT); // porta padrão 5050 ou passada como argumento

    // associa socket à porta
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Erro no bind");
        exit(EXIT_FAILURE);
    }

    // coloca o socket em modo de escuta
    if (listen(server_fd, 3) < 0) {
        perror("Erro no listen");
        exit(EXIT_FAILURE);
    }

    printf("Servidor ouvindo na porta %d...\n", ntohs(address.sin_port));

    // aceita conexão de um cliente
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                             (socklen_t *)&addrlen)) < 0) {
        perror("Erro no accept");
        exit(EXIT_FAILURE);
    }

    printf("Cliente conectado!\n");

    while (1) {
        memset(buffer, 0, BUF_SIZE);
        int valread = read(new_socket, buffer, BUF_SIZE - 1); // lê mensagem do cliente
        if (valread <= 0) break;

        buffer[valread] = '\0';

        // verifica se cliente pediu para sair
        if (strncmp(buffer, "QUIT", 4) == 0) {
            printf("Cliente pediu para encerrar.\n");
            break;
        }

        // parsing simples: "OP A B"
        char op[10];
        double a, b, result;
        if (sscanf(buffer, "%s %lf %lf", op, &a, &b) != 3) {
            sprintf(buffer, "ERR EINV entrada_invalida\n");
        } else {
            if (strcmp(op, "ADD") == 0) result = a + b;
            else if (strcmp(op, "SUB") == 0) result = a - b;
            else if (strcmp(op, "MUL") == 0) result = a * b;
            else if (strcmp(op, "DIV") == 0) {
                if (b == 0) { // tratamento de divisão por zero
                    sprintf(buffer, "ERR EZDV divisao_por_zero\n");
                    write(new_socket, buffer, strlen(buffer));
                    continue;
                }
                result = a / b;
            } else { // operação inválida
                sprintf(buffer, "ERR EINV operacao_invalida\n");
                write(new_socket, buffer, strlen(buffer));
                continue;
            }
            sprintf(buffer, "OK %.6f\n", result); // formata resultado
        }
        write(new_socket, buffer, strlen(buffer)); // envia resposta
    }

    // fecha sockets
    close(new_socket);
    close(server_fd);
    return 0;
}
