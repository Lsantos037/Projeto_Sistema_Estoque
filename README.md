# Sistema de Gerenciamento de Estoque (C++)

Projeto desenvolvido para a atividade avaliativa de Programação Estruturada em C/C++.

## Funcionalidades

1. Cadastrar produtos com código automático, nome, quantidade e preço.
2. Consultar um produto pelo código.
3. Exportar os produtos ativos para `produtos.txt`.
4. Excluir produtos logicamente, alterando o campo `ativo` de 1 para 0.
5. Contabilizar produtos ativos e registros armazenados.
6. Listar os produtos ativos.

## Requisitos técnicos utilizados

- Linguagem C++.
- `struct Produto` para representar cada produto.
- Funções separadas por responsabilidade.
- Arquivo binário `produtos.dat` para persistência dos registros.
- Arquivo texto `produtos.txt` para exportação.
- Menu interativo no terminal.

## Como compilar e executar

É necessário ter um compilador C++ instalado, como o g++.

### Windows (MinGW)

Abra o terminal na pasta do projeto e execute:

```bash
g++ estoque.cpp -o estoque.exe
.\estoque.exe
```

### Linux

```bash
g++ estoque.cpp -o estoque
./estoque
```

### macOS

```bash
clang++ estoque.cpp -o estoque
./estoque
```

O arquivo `produtos.dat` será criado automaticamente na mesma pasta quando o primeiro produto for cadastrado. A exportação cria o arquivo `produtos.txt`.

## Como demonstrar ao professor

1. Escolha a opção 1 e cadastre dois produtos.
2. Escolha a opção 2 e consulte um produto usando o código gerado.
3. Escolha a opção 6 para listar os produtos ativos.
4. Escolha a opção 5 para mostrar a contagem.
5. Escolha a opção 3 e abra o arquivo `produtos.txt`.
6. Escolha a opção 4 e exclua um dos produtos.
7. Consulte novamente o produto excluído para mostrar que ele não aparece como ativo.
8. Escolha a opção 5 para comparar a quantidade de produtos ativos com a quantidade de registros armazenados.

## Observação sobre a exclusão lógica

O programa não apaga fisicamente o produto do arquivo binário. Ele altera o campo `ativo` para 0. Assim, o registro continua armazenado, mas deixa de aparecer nas consultas, listagens e exportações de produtos ativos.

## Estrutura do projeto

- `estoque.cpp`: código-fonte completo.
- `README.md`: instruções de compilação, execução e demonstração.
