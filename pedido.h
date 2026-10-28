#ifndef PEDIDO_H
#define PEDIDO_H

#define ARQUIVO_PEDIDOS "pedidos.txt"

typedef struct {
    char nome[100];
    char cpf[15];
    char telefone[20];
} Cliente;

void criarPedido();
void listarPedidos();

#endif