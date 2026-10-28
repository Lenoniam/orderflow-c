#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "produto.h"

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Remove ';' e '|' de um campo de texto, pois são os separadores usados
// no produtos_orderflow.csv e no pedidos.txt; deixá-los passar quebraria
// o parsing das linhas desses arquivos.
void sanitizarCampo(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == ';' || str[i] == '|') {
            str[i] = ' ';
        }
    }
}

// Lê um inteiro validando a entrada: repete o prompt enquanto o usuário
// digitar algo que não seja número, ou um número menor que 'minimo'.
int lerInteiroMin(const char *prompt, int minimo) {
    int valor;
    while (1) {
        printf("%s", prompt);
        if (scanf("%d", &valor) != 1) {
            limparBuffer();
            printf("Entrada inválida! Digite um número inteiro.\n");
            continue;
        }
        limparBuffer();
        if (valor < minimo) {
            printf("Valor deve ser maior ou igual a %d.\n", minimo);
            continue;
        }
        return valor;
    }
}

// Mesma ideia, mas para números decimais (preço).
float lerFloatMin(const char *prompt, float minimo) {
    float valor;
    while (1) {
        printf("%s", prompt);
        if (scanf("%f", &valor) != 1) {
            limparBuffer();
            printf("Entrada inválida! Digite um número válido.\n");
            continue;
        }
        limparBuffer();
        if (valor < minimo) {
            printf("Valor deve ser maior ou igual a %.2f.\n", minimo);
            continue;
        }
        return valor;
    }
}

// Compara duas strings ignorando maiúsculas/minúsculas (ex: "m" == "M").
int igualIgnorandoCaixa(const char *a, const char *b) {
    while (*a && *b) {
        if (tolower((unsigned char)*a) != tolower((unsigned char)*b)) {
            return 0;
        }
        a++;
        b++;
    }
    return *a == '\0' && *b == '\0';
}

// Verifica se o texto é um tamanho válido: uma das letras de roupa
// (P, M, G, GG) ou um número de calçado entre 30 e 48.
int tamanhoValido(const char *texto) {
    const char *tamanhosRoupa[] = {"P", "M", "G", "GG"};
    int totalRoupa = 4;

    for (int i = 0; i < totalRoupa; i++) {
        if (igualIgnorandoCaixa(texto, tamanhosRoupa[i])) {
            return 1;
        }
    }

    if (strlen(texto) == 0) {
        return 0;
    }

    int todosDigitos = 1;
    for (int i = 0; texto[i] != '\0'; i++) {
        if (!isdigit((unsigned char)texto[i])) {
            todosDigitos = 0;
            break;
        }
    }

    if (todosDigitos) {
        int numero = atoi(texto);
        if (numero >= 30 && numero <= 48) {
            return 1;
        }
    }

    return 0;
}

// Lê o tamanho do usuário, repetindo o prompt enquanto o valor não for
// uma letra de roupa reconhecida (P, M, G, GG) ou um número de calçado (30 a 48).
void lerTamanho(char *destino, size_t tamanhoBuffer, const char *prompt) {
    while (1) {
        printf("%s", prompt);
        fgets(destino, (int) tamanhoBuffer, stdin);
        destino[strcspn(destino, "\n")] = 0;
        sanitizarCampo(destino);

        if (tamanhoValido(destino)) {
            return;
        }

        printf("Tamanho inválido! Use P, M, G, GG ou um número de calçado entre 30 e 48.\n");
    }
}

// Verifica se já existe um produto com o ID informado.
// Retorna 1 se encontrar (ID duplicado), 0 se o ID estiver livre.
int idJaExiste(int id) {
    int total = 0;
    Produto *produtos = carregarProdutosMemoria(&total);
    int existe = 0;

    for (int i = 0; i < total; i++) {
        if (produtos[i].id == id) {
            existe = 1;
            break;
        }
    }

    if (produtos) free(produtos);
    return existe;
}

void cadastrarProduto() {
    Produto p;
    printf("\n--- Cadastrar Produto ---\n");

    do {
        p.id = lerInteiroMin("ID: ", 0);
        if (idJaExiste(p.id)) {
            printf("Já existe um produto com o ID %d. Escolha outro ID.\n", p.id);
        } else {
            break;
        }
    } while (1);

    FILE *arq = fopen(ARQUIVO_PRODUTOS, "a");
    if (!arq) {
        printf("Erro ao abrir banco de dados de produtos!\n");
        return;
    }

    printf("Nome: ");
    fgets(p.nome, sizeof(p.nome), stdin);
    p.nome[strcspn(p.nome, "\n")] = 0;
    sanitizarCampo(p.nome);

    lerTamanho(p.tamanho, sizeof(p.tamanho), "Tamanho (P, M, G, GG ou número de calçado): ");

    p.preco = lerFloatMin("Preço: ", 0.0f);
    p.estoque = lerInteiroMin("Estoque: ", 0);

    fprintf(arq, "%d;%s;%s;%.2f;%d\n", p.id, p.nome, p.tamanho, p.preco, p.estoque);
    fclose(arq);

    printf("Produto cadastrado com sucesso!\n");
}

void listarProdutos() {
    FILE *arq = fopen(ARQUIVO_PRODUTOS, "r");
    if (!arq) {
        printf("\nNenhum produto cadastrado ainda.\n");
        return;
    }

    Produto p;
    int contador = 0;

    printf("\n--- Catálogo de Produtos ---\n");
    printf("ID\tNome\t\tTam\tPreço\t\tEstoque\n");
    printf("-------------------------------------------------------\n");

    while (fscanf(arq, " %d;%49[^;];%9[^;];%f;%d\n", &p.id, p.nome, p.tamanho, &p.preco, &p.estoque) == 5) {
        printf("%d\t%-15s %-7s R$ %-8.2f %d\n", p.id, p.nome, p.tamanho, p.preco, p.estoque);
        contador++;
    }

    fclose(arq);

    if (contador == 0) {
        printf("(Incompatibilidade de dados ou nenhum produto gravado).\n");
    }
}

Produto* carregarProdutosMemoria(int *totalLidos) {
    FILE *arq = fopen(ARQUIVO_PRODUTOS, "r");
    if (!arq) {
        *totalLidos = 0;
        return NULL;
    }

    int capacidade = 5;
    int qtd = 0;
    Produto *produtos = (Produto*) malloc(capacidade * sizeof(Produto));

    if (!produtos) {
        fclose(arq);
        *totalLidos = 0;
        return NULL;
    }

    Produto temp;
    while (fscanf(arq, " %d;%49[^;];%9[^;];%f;%d\n", 
                  &temp.id, temp.nome, temp.tamanho, &temp.preco, &temp.estoque) == 5) {
        if (qtd >= capacidade) {
            capacidade *= 2;
            Produto *tempPtr = (Produto*) realloc(produtos, capacidade * sizeof(Produto));
            if (!tempPtr) break;
            produtos = tempPtr;
        }
        produtos[qtd++] = temp;
    }

    fclose(arq);
    *totalLidos = qtd;
    return produtos;
}

void salvarProdutosMemoria(Produto *produtos, int total) {
    FILE *arq = fopen(ARQUIVO_PRODUTOS, "w");
    if (!arq) {
        printf("Erro ao atualizar arquivo de produtos!\n");
        return;
    }

    for (int i = 0; i < total; i++) {
        fprintf(arq, "%d;%s;%s;%.2f;%d\n", 
                produtos[i].id, produtos[i].nome, produtos[i].tamanho, produtos[i].preco, produtos[i].estoque);
    }

    fclose(arq);
}

void editarProduto() {
    int total = 0;
    Produto *produtos = carregarProdutosMemoria(&total);

    if (!produtos || total == 0) {
        printf("\nNenhum produto cadastrado para editar.\n");
        if (produtos) free(produtos);
        return;
    }

    int encontrado = 0;
    printf("\n--- Editar Produto ---\n");
    int idBusca = lerInteiroMin("Informe o ID do produto que deseja editar: ", 0);

    for (int i = 0; i < total; i++) {
        if (produtos[i].id == idBusca) {
            encontrado = 1;
            printf("\nProduto Atual: %s (Tam: %s | R$ %.2f | Estoque: %d)\n", 
                   produtos[i].nome, produtos[i].tamanho, produtos[i].preco, produtos[i].estoque);
            
            printf("Novo Nome: ");
            fgets(produtos[i].nome, sizeof(produtos[i].nome), stdin);
            produtos[i].nome[strcspn(produtos[i].nome, "\n")] = 0;
            sanitizarCampo(produtos[i].nome);

            lerTamanho(produtos[i].tamanho, sizeof(produtos[i].tamanho), "Novo Tamanho (P, M, G, GG ou número de calçado): ");
            
            produtos[i].preco = lerFloatMin("Novo Preço: ", 0.0f);
            produtos[i].estoque = lerInteiroMin("Novo Estoque: ", 0);
            break;
        }
    }

    if (encontrado) {
        salvarProdutosMemoria(produtos, total);
        printf("Produto atualizado com sucesso!\n");
    } else {
        printf("Produto com ID %d não foi encontrado.\n", idBusca);
    }

    free(produtos);
}
