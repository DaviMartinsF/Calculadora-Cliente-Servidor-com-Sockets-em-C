# 🖥️ Aplicação Cliente-Servidor com Sockets TCP (C)

Este projeto implementa uma aplicação **cliente-servidor** em C, utilizando **sockets TCP (IPv4)**.  
O cliente envia operações matemáticas e o servidor processa e retorna os resultados seguindo um protocolo simples baseado em texto.  

> ⚠️ Este programa foi desenvolvido e testado em **Linux (Ubuntu na VirtualBox, versão mais atualizada)**.  

## 📂 Estrutura do Projeto

```
/Socket
 ├── client.c        # Código do cliente
 ├── server.c        # Código do servidor
 ├── Makefile        # Automação de compilação
 └── README.md       # Documentação do projeto
```

## ⚙️ Compilação e Execução

Dentro da pasta do projeto, use o `make`:

```bash
make
```

Isso irá gerar os executáveis:

- `server` → servidor TCP  
- `client` → cliente TCP  

### ▶️ Executando o servidor

Servidor na porta **padrão (5050):**

```bash
./server
```

Servidor em outra porta (ex.: 6000):

```bash
./server 6000
```

### ▶️ Executando o cliente

O cliente precisa do **IP do servidor** e da **porta**:

```bash
./client 127.0.0.1 5050
```

Após conectar, digite operações diretamente no terminal.

## 📡 Protocolo de Comunicação

### Requisição (cliente → servidor)

```
OP A B\n
```

- `OP` ∈ {`ADD`, `SUB`, `MUL`, `DIV`}  
- `A` e `B` são números reais (ex.: `2.5`, `-3.0`, `10.0`)  

### Resposta (servidor → cliente)

- **Sucesso:**
```
OK R\n
```
`R` é o resultado da operação.

- **Erro:**
```
ERR <COD> <mensagem>\n
```

Códigos possíveis:
- `EINV` → entrada inválida  
- `EZDV` → divisão por zero  
- `ESRV` → erro interno  

## ✅ Exemplos de Uso

### Operações válidas
```
Cliente → ADD 10 2
Servidor → OK 12
```

```
Cliente → SUB 7 9
Servidor → OK -2
```

```
Cliente → MUL -3 3.5
Servidor → OK -10.5
```

### Divisão por zero
```
Cliente → DIV 5 0
Servidor → ERR EZDV divisao_por_zero
```

### Encerrar conexão
```
Cliente → QUIT
```

## 🛠️ Decisões de Projeto

- Implementação simples (single-thread): o servidor atende **um cliente por vez**.  
- Parsing robusto: valida número de tokens, operação e números de entrada.  
- Uso de mensagens padronizadas para facilitar testes.  
- Porta configurável via argumento de linha de comando.  
- Desenvolvido e testado especificamente em **Ubuntu/Linux dentro de uma VM VirtualBox**.  

## 🚧 Limitações Conhecidas

- Não suporta múltiplos clientes simultâneos.  
- Apenas operações básicas (`ADD`, `SUB`, `MUL`, `DIV`).  
- Não implementa a forma infixa (`A + B`) — pode ser adicionada como extensão.  
- Portabilidade limitada: não foi testado em Windows ou MacOS.  

## 🧪 Como Rodar Testes

1. Inicie o servidor:
```bash
./server
```

2. Em outro terminal, inicie o cliente:
```bash
./client 127.0.0.1 5050
```

3. Digite operações e verifique as respostas:  
   - Operações válidas → deve retornar `OK R`.  
   - Divisão por zero → deve retornar `ERR EZDV divisao_por_zero`.  
   - Entrada inválida → deve retornar `ERR EINV entrada_invalida`.  

## 📌 Exemplo Completo de Sessão

### Terminal 1 (servidor):
```bash
$ ./server
Servidor ouvindo na porta 5050...
```

### Terminal 2 (cliente):
```bash
$ ./client 127.0.0.1 5050
ADD 10 5
OK 15
DIV 8 2
OK 4
DIV 3 0
ERR EZDV divisao_por_zero
QUIT
```

## 🔧 Compilação manual (sem Makefile)

Se preferir, você pode compilar manualmente:

```bash
gcc -Wall -g server.c -o server
gcc -Wall -g client.c -o client
```
