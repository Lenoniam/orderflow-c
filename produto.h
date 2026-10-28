#ifndef PRODUTO_H
#define PRODUTO_H

#include <stddef.h>

#define ARQUIVO_PRODUTOS "produtos_orderflow.csv"
#define MAX_PRODUTOS 100

typedef struct {
    int id;
    char nome[50];
    char tamanho[10];
    float preco;
    int estoque;
} Produto;

void limparBuffer();
void sanitizarCampo(char *str);
int lerInteiroMin(const char *prompt, int minimo);
float lerFloatMin(const char *prompt, float minimo);
int igualIgnorandoCaixa(const char *a, const char *b);
int tamanhoValido(const char *texto);
void lerTamanho(char *destino, size_t tamanhoBuffer, const char *prompt);
int idJaExiste(int id);
void cadastrarProduto();
void listarProdutos();
void editarProduto();
Produto* carregarProdutosMemoria(int *totalLidos);
void salvarProdutosMemoria(Produto *produtos, int total);

#endif