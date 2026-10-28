# OrderFlow — C

Versão em C do sistema OrderFlow, para cadastro de produtos e registro de pedidos.

## Funcionalidades

- Cadastro de produtos
- Listagem do catálogo
- Edição de produtos
- Criação de pedidos
- Controle de estoque
- Histórico de pedidos
- Validação de entradas

## Compilar com GCC

```bash
gcc -std=c11 -Wall -Wextra -Wpedantic main.c produto.c Untitled2.c -o OrderFlow
./OrderFlow
```

Também é possível abrir `OrderFlow.dev` no Embarcadero Dev-C++.

## Dados locais

- `produtos_orderflow.csv` contém dados de exemplo do catálogo.
- `pedidos.txt` é mantido vazio no repositório para não expor dados pessoais. O programa grava os pedidos localmente durante a execução.
- Executáveis, objetos e arquivos gerados pelo Dev-C++ são ignorados pelo Git.
