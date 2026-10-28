#include <stdio.h>
#include <stdlib.h>
#ifdef _WIN32
#include <windows.h>
#endif
#include "pedido.h"
#include "produto.h"

int main() {
#ifdef _WIN32
    // Faz o console do Windows aceitar/exibir acentos em UTF-8.
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
#endif
    int opcao;

    do {
        printf("\n===== OrderFlow =====\n");
        printf("1 - Cadastrar Produto\n");
        printf("2 - Listar Produtos\n");
        printf("3 - Editar Produto\n");
        printf("4 - Criar Pedido\n");
        printf("5 - Listar Pedidos\n");
        printf("0 - Sair\n");
        printf("Escolha: ");

        if (scanf("%d", &opcao) != 1) {
            // usuário digitou algo que não é número
            limparBuffer();
            printf("Entrada inválida! Digite um número.\n");
            continue;
        }
        limparBuffer();

        switch (opcao) {
            case 1:
                cadastrarProduto();
                break;
            case 2:
                listarProdutos();
                break;
            case 3:
                editarProduto();
                break;
            case 4:
                criarPedido();
                break;
            case 5:
                listarPedidos();
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
    } while (opcao != 0);

    return 0;
}
