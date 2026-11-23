#include <iostream>
#include <fstream>
#include <iomanip>
#include <limits>
#include <cstring>
#include <string>

using namespace std;

// Arquivo binario: cada registro possui tamanho fixo.
const string ARQUIVO_BINARIO = "produtos.dat";
const string ARQUIVO_TEXTO = "produtos.txt";

struct Produto {
    int codigo;
    char nome[50];
    int quantidade;
    float preco;
    int ativo; // 1 = ativo; 0 = excluido logicamente
};

// Limpa dados restantes da entrada.
void limparEntrada() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Procura o maior codigo ja utilizado, inclusive registros excluidos.
int gerarProximoCodigo() {
    ifstream arquivo(ARQUIVO_BINARIO, ios::binary);
    Produto produto;
    int maiorCodigo = 0;

    if (!arquivo) {
        return 1;
    }

    while (arquivo.read(reinterpret_cast<char*>(&produto), sizeof(Produto))) {
        if (produto.codigo > maiorCodigo) {
            maiorCodigo = produto.codigo;
        }
    }

    arquivo.close();
    return maiorCodigo + 1;
}

// Cadastra um novo produto no final do arquivo binario.
void cadastrarProduto() {
    Produto produto{};
    produto.codigo = gerarProximoCodigo();
    produto.ativo = 1;

    cout << "\n===== CADASTRO DE PRODUTO =====\n";
    cout << "Codigo gerado: " << produto.codigo << "\n";

    cout << "Nome do produto: ";
    cin.getline(produto.nome, 50);

    while (strlen(produto.nome) == 0) {
        cout << "O nome nao pode ficar vazio. Digite novamente: ";
        cin.getline(produto.nome, 50);
    }

    cout << "Quantidade em estoque: ";
    while (!(cin >> produto.quantidade) || produto.quantidade < 0) {
        cout << "Quantidade invalida. Digite um numero inteiro maior ou igual a zero: ";
        limparEntrada();
    }

    cout << "Preco unitario (ex.: 12.50): R$ ";
    while (!(cin >> produto.preco) || produto.preco < 0) {
        cout << "Preco invalido. Digite um valor maior ou igual a zero: R$ ";
        limparEntrada();
    }
    limparEntrada();

    ofstream arquivo(ARQUIVO_BINARIO, ios::binary | ios::app);
    if (!arquivo) {
        cout << "Erro ao abrir o arquivo de dados.\n";
        return;
    }

    arquivo.write(reinterpret_cast<char*>(&produto), sizeof(Produto));
    arquivo.close();

    cout << "\nProduto cadastrado com sucesso!\n";
}

// Consulta um produto pelo codigo. Exibe somente registros ativos.
void consultarProduto() {
    int codigo;
    Produto produto;
    bool encontrado = false;

    cout << "\n===== CONSULTA DE PRODUTO =====\n";
    cout << "Digite o codigo: ";
    if (!(cin >> codigo)) {
        cout << "Codigo invalido.\n";
        limparEntrada();
        return;
    }
    limparEntrada();

    ifstream arquivo(ARQUIVO_BINARIO, ios::binary);
    if (!arquivo) {
        cout << "Ainda nao existem produtos cadastrados.\n";
        return;
    }

    while (arquivo.read(reinterpret_cast<char*>(&produto), sizeof(Produto))) {
        if (produto.codigo == codigo && produto.ativo == 1) {
            cout << "\nProduto encontrado:\n";
            cout << "Codigo: " << produto.codigo << "\n";
            cout << "Nome: " << produto.nome << "\n";
            cout << "Quantidade: " << produto.quantidade << "\n";
            cout << fixed << setprecision(2);
            cout << "Preco unitario: R$ " << produto.preco << "\n";
            cout << "Valor total em estoque: R$ "
                 << produto.quantidade * produto.preco << "\n";
            encontrado = true;
            break;
        }
    }

    arquivo.close();

    if (!encontrado) {
        cout << "Produto nao encontrado ou ja excluido.\n";
    }
}

// Exporta todos os produtos ativos para um arquivo TXT legivel.
void exportarParaTXT() {
    ifstream entrada(ARQUIVO_BINARIO, ios::binary);
    if (!entrada) {
        cout << "\nNao ha dados para exportar.\n";
        return;
    }

    ofstream saida(ARQUIVO_TEXTO);
    if (!saida) {
        cout << "\nErro ao criar o arquivo TXT.\n";
        return;
    }

    Produto produto;
    int quantidadeExportada = 0;

    saida << "========== RELATORIO DE ESTOQUE ==========\n\n";
    saida << left << setw(8) << "Codigo"
          << setw(25) << "Nome"
          << setw(12) << "Quantidade"
          << setw(12) << "Preco (R$)"
          << "Total (R$)\n";
    saida << string(75, '-') << "\n";

    while (entrada.read(reinterpret_cast<char*>(&produto), sizeof(Produto))) {
        if (produto.ativo == 1) {
            saida << left << setw(8) << produto.codigo
                  << setw(25) << produto.nome
                  << setw(12) << produto.quantidade
                  << setw(12) << fixed << setprecision(2) << produto.preco
                  << fixed << setprecision(2)
                  << produto.quantidade * produto.preco << "\n";
            quantidadeExportada++;
        }
    }

    saida << "\nTotal de produtos ativos: " << quantidadeExportada << "\n";
    entrada.close();
    saida.close();

    cout << "\nExportacao concluida! " << quantidadeExportada
         << " produto(s) ativo(s) exportado(s) para " << ARQUIVO_TEXTO << ".\n";
}

// Realiza exclusao logica: altera o campo ativo sem remover o registro.
void excluirProduto() {
    int codigo;
    Produto produto;
    bool encontrado = false;

    cout << "\n===== EXCLUSAO DE PRODUTO =====\n";
    cout << "Digite o codigo do produto: ";
    if (!(cin >> codigo)) {
        cout << "Codigo invalido.\n";
        limparEntrada();
        return;
    }
    limparEntrada();

    fstream arquivo(ARQUIVO_BINARIO, ios::binary | ios::in | ios::out);
    if (!arquivo) {
        cout << "Ainda nao existem produtos cadastrados.\n";
        return;
    }

    while (arquivo.read(reinterpret_cast<char*>(&produto), sizeof(Produto))) {
        if (produto.codigo == codigo && produto.ativo == 1) {
            produto.ativo = 0;

            // Volta ao inicio do registro encontrado e grava a alteracao.
            streampos posicao = arquivo.tellg();
            arquivo.seekp(posicao - static_cast<streamoff>(sizeof(Produto)));
            arquivo.write(reinterpret_cast<char*>(&produto), sizeof(Produto));

            encontrado = true;
            break;
        }
    }

    arquivo.close();

    if (encontrado) {
        cout << "Produto excluido logicamente com sucesso.\n";
        cout << "O registro continua armazenado no arquivo binario.\n";
    } else {
        cout << "Produto nao encontrado ou ja excluido.\n";
    }
}

// Conta registros ativos e tambem informa quantos estao armazenados no arquivo.
void contabilizarProdutos() {
    ifstream arquivo(ARQUIVO_BINARIO, ios::binary);
    if (!arquivo) {
        cout << "\nNenhum produto cadastrado.\n";
        cout << "Produtos ativos: 0\nRegistros armazenados: 0\n";
        return;
    }

    Produto produto;
    int ativos = 0;
    int armazenados = 0;

    while (arquivo.read(reinterpret_cast<char*>(&produto), sizeof(Produto))) {
        armazenados++;
        if (produto.ativo == 1) {
            ativos++;
        }
    }

    arquivo.close();

    cout << "\n===== CONTAGEM DE REGISTROS =====\n";
    cout << "Produtos ativos: " << ativos << "\n";
    cout << "Registros armazenados no arquivo: " << armazenados << "\n";
}

// Lista os produtos ativos para facilitar a demonstracao do sistema.
void listarProdutos() {
    ifstream arquivo(ARQUIVO_BINARIO, ios::binary);
    if (!arquivo) {
        cout << "\nNenhum produto cadastrado.\n";
        return;
    }

    Produto produto;
    bool existeAtivo = false;

    cout << "\n==================== PRODUTOS EM ESTOQUE ====================\n";
    cout << left << setw(8) << "Codigo"
         << setw(25) << "Nome"
         << setw(12) << "Quantidade"
         << "Preco (R$)\n";
    cout << string(58, '-') << "\n";

    while (arquivo.read(reinterpret_cast<char*>(&produto), sizeof(Produto))) {
        if (produto.ativo == 1) {
            cout << left << setw(8) << produto.codigo
                 << setw(25) << produto.nome
                 << setw(12) << produto.quantidade
                 << fixed << setprecision(2) << produto.preco << "\n";
            existeAtivo = true;
        }
    }

    arquivo.close();

    if (!existeAtivo) {
        cout << "Nao ha produtos ativos no estoque.\n";
    }
}

int main() {
    int opcao;

    do {
        cout << "\n========================================\n";
        cout << "       SISTEMA DE GERENCIAMENTO\n";
        cout << "                 DE ESTOQUE\n";
        cout << "========================================\n";
        cout << "1 - Cadastrar produto\n";
        cout << "2 - Consultar produto por codigo\n";
        cout << "3 - Exportar produtos para TXT\n";
        cout << "4 - Excluir produto (exclusao logica)\n";
        cout << "5 - Contabilizar registros\n";
        cout << "6 - Listar produtos ativos\n";
        cout << "0 - Sair\n";
        cout << "Escolha uma opcao: ";

        if (!(cin >> opcao)) {
            cout << "Opcao invalida. Digite um numero.\n";
            limparEntrada();
            continue;
        }
        limparEntrada();

        switch (opcao) {
            case 1:
                cadastrarProduto();
                break;
            case 2:
                consultarProduto();
                break;
            case 3:
                exportarParaTXT();
                break;
            case 4:
                excluirProduto();
                break;
            case 5:
                contabilizarProdutos();
                break;
            case 6:
                listarProdutos();
                break;
            case 0:
                cout << "\nEncerrando o sistema. Ate mais!\n";
                break;
            default:
                cout << "\nOpcao invalida. Tente novamente.\n";
        }

    } while (opcao != 0);

    return 0;
}
