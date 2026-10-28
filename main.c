#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pedido.h"
#include "produto.h"

void criarPedido() {
    int totalProdutos = 0;
    Produto *produtos = carregarProdutosMemoria(&totalProdutos);

    if (!produtos || totalProdutos == 0) {
        printf("\nNenhum produto disponível para venda.\n");
        if (produtos) free(produtos);
        return;
    }

    Cliente cli;
    int idBusca, quantidade, indiceEncontrado = -1;

    printf("\n--- Criar Novo Pedido ---\n");
    printf("Nome do Cliente: ");
    fgets(cli.nome, sizeof(cli.nome), stdin);
    cli.nome[strcspn(cli.nome, "\n")] = 0;
    sanitizarCampo(cli.nome);

    printf("CPF do Cliente: ");
    fgets(cli.cpf, sizeof(cli.cpf), stdin);
    cli.cpf[strcspn(cli.cpf, "\n")] = 0;
    sanitizarCampo(cli.cpf);

    printf("Telefone do Cliente: ");
    fgets(cli.telefone, sizeof(cli.telefone), stdin);
    cli.telefone[strcspn(cli.telefone, "\n")] = 0;
    sanitizarCampo(cli.telefone);

    idBusca = lerInteiroMin("Informe o ID do Produto desejado: ", 0);
    quantidade = lerInteiroMin("Quantidade desejada: ", 1);

    for (int i = 0; i < totalProdutos; i++) {
        if (produtos[i].id == idBusca) {
            indiceEncontrado = i;
            break;
        }
    }

    if (indiceEncontrado == -1) {
        printf("Produto com ID %d não foi encontrado.\n", idBusca);
        free(produtos);
        return;
    }

    if (produtos[indiceEncontrado].estoque < quantidade) {
        printf("Estoque insuficiente! Disponível em estoque: %d\n", produtos[indiceEncontrado].estoque);
        free(produtos);
        return;
    }

    // Atualiza o estoque em memória
    produtos[indiceEncontrado].estoque -= quantidade;

    // Salva a alteração do estoque de volta no CSV
    salvarProdutosMemoria(produtos, totalProdutos);

    // Registra o pedido no arquivo TXT
    FILE *arqPedidos = fopen(ARQUIVO_PEDIDOS, "a");
    if (arqPedidos) {
        float total = produtos[indiceEncontrado].preco * quantidade;
        fprintf(arqPedidos, "Cliente: %s | CPF: %s | Tel: %s | Produto: %s (Tam: %s) | Qtd: %d | Total: R$ %.2f\n", 
                cli.nome, cli.cpf, cli.telefone, 
                produtos[indiceEncontrado].nome, produtos[indiceEncontrado].tamanho, 
                quantidade, total);
        fclose(arqPedidos);
        printf("\nPedido registrado com sucesso!\nTotal da Compra: R$ %.2f\n", total);
    }

    free(produtos);
}

void listarPedidos() {
    FILE *arq = fopen(ARQUIVO_PEDIDOS, "r");
    if (!arq) {
        printf("\nNenhum pedido registrado ainda.\n");
        return;
    }

    char linha[256];
    printf("\n--- Histórico de Pedidos ---\n");
    while (fgets(linha, sizeof(linha), arq)) {
        printf("%s", linha);
    }

    fclose(arq);
}