#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <time.h>
#include <inttypes.h>
#ifdef __unix__
# include <unistd.h>
#elif defined _WIN32
# include <windows.h>
#define sleep(x) Sleep(1000 * (x))
#endif

struct usuario { char *nome; char *senha; };
struct empresa { char *responsavel; char *cpf; char *nomeEmpresa; char *cnpj; char *razaoSocial; 
								 char *nomeFantasia; char* endereco; char *email; char *abertura; };

struct relatorio { char *cnpj; unsigned int totalInsumosSemestre; unsigned int totalGastosMensais; };
int empresaExiste(char *cnpj);

void telaInicial(), telaCadastroFuncionario(), telaLoginFuncionario(), telaSistema();
int conectarFuncionario(char *nome, char *senha), salvarFuncionario(struct usuario u), salvarEmpresa(struct empresa e);
struct usuario fabricarUsuario(char *nome, char *senha);
struct empresa fabricarEmpresa(char *responsavel, char *cpf, char *nomeEmpresa, char *cnpj, char *razaoSocial, char *nomeFantasia, char *endereco, char *email, char *abertura);

int salvarRelatorio(struct relatorio r) {
	char nomeArquivo[64];

	sprintf(nomeArquivo, "relatorio-%s.txt", r.cnpj);
	FILE *f = fopen(nomeArquivo, "w");

	char linha[128];

	sprintf(linha, "%s;%i;%i\n", r.cnpj, r.totalInsumosSemestre, r.totalGastosMensais);
	fprintf(f, "%s", linha);

	fclose(f);

	return 1;
}

struct relatorio fabricarRelatorio(char *cnpj, unsigned int totalInsumosSemestre, unsigned int totalGastosMensais) {
	struct relatorio r;

	r.cnpj = cnpj;
	r.totalInsumosSemestre = totalInsumosSemestre;
	r.totalGastosMensais = totalGastosMensais;

	return r;
}

int main(void) {
	setlocale(LC_ALL, "Portuguese_Brazil");
	telaInicial();
	return 0;
}


void escreverFrasePadrao(char *frase) {
	printf("%s\n\n", frase);
}

int salvarEmpresa(struct empresa e) {
	FILE *f = fopen("empresas-cadastradas", "a");

	char *linha = malloc(256);

	sprintf(linha, "%s;%s;%s;%s;%s;%s;%s;%s;%s\n", e.cnpj, e.cpf, e.responsavel, e.nomeEmpresa, e.abertura, e.email, e.endereco, e.nomeFantasia, e.razaoSocial);

	fprintf(f, "%s", linha);

	fclose(f);
	free(linha);

	return 1;
}

struct empresa fabricarEmpresa(char *responsavel, char *cpf, char *nomeEmpresa, char *cnpj, char *razaoSocial, char *nomeFantasia, char *endereco, char *email, char *abertura) {
	struct empresa e;

	e.responsavel = responsavel;
	e.cpf = cpf;
	e.nomeEmpresa = nomeEmpresa;
	e.cnpj = cnpj;
	e.razaoSocial = razaoSocial;
	e.nomeFantasia = nomeFantasia;
	e.endereco = endereco;
	e.email = email;
	e.abertura = abertura;

	return e;
}

int funcionarioExiste(char *nome, char *linha) {
	char *token = strtok(linha, ";");
	if (strcmp(nome, token) == 0) { return 1; }

	return 0;
}

int verificarSenhaFuncionario(char *nome, char *senha, char *linha) {
	char entrada[128];
	sprintf(entrada, "%s;%s", nome, senha);

	if (strcmp(linha, entrada) == 0) {
		return 1;
	}

	return 0;
}

int conectarFuncionario(char *nome, char *senha) {
	FILE *arquivo = fopen("funcionarios.txt", "r");

	if (arquivo == 0) { return 0; }

	char linha[128];

	while (fscanf(arquivo, "%s\n", linha) > 0) {
		char copia[128];
		strcpy(copia, linha);

		if (!funcionarioExiste(nome, copia)) { continue; }

		if (verificarSenhaFuncionario(nome, senha, linha)) { return 1; }
	}
	fclose(arquivo);

	return 0;
}

int salvarFuncionario(struct usuario u) {
	char *linha = malloc(sizeof(u));
	FILE *arquivo = fopen("funcionarios.txt", "a");

	sprintf(linha, "%s;%s\n", u.nome, u.senha);
	fprintf(arquivo, "%s", linha);
	
	free(linha);
	fclose(arquivo);

	return 0;
}

struct usuario fabricarUsuario(char *nome, char *senha)
{
	struct usuario fabricado;

	fabricado.nome = nome;
	fabricado.senha = senha;

	return fabricado;
}

void escreverRoteiro(char *roteiro[], unsigned int tamanho)
{
	system("clear");

	escreverFrasePadrao("(Eco Client, versão: 1.0)");

	for (int i = 0; i < tamanho; i++) {
		if (i == 0) {
			printf("[!] %s\n\n", roteiro[i]);
			continue;
		}

		escreverFrasePadrao(roteiro[i]);
	}
}

void telaInicial()
{
	char *roteiro[] = {"Olá funcionário, você já possui um login?", "Digite (1) para sim", "Digite (2) para não", "Digite (3) para fechar o programa"};

	size_t tamanhoRoteiro = sizeof(roteiro) / sizeof(roteiro[0]);
	uint8_t escolha;

	escreverRoteiro(roteiro, tamanhoRoteiro);
	scanf("%hhu", &escolha);

	switch(escolha) {
		case 1:
			telaLoginFuncionario();
			break;
		case 2:
			telaCadastroFuncionario();
			break;
		case 3:
			printf("\nFechando o programa...\n");
			exit(0);
			break;
		default:
			printf("Resposta inválida, tente novamente!");
			break;
	}
}

void telaRegistroEmpresaSucesso() {
	char *roteiro[] = {"Você registrou uma nova empresa com sucesso.", "Agora você pode gerenciar os residuos dessa empresa.", "Você sera redirecionado em alguns segundos para o menu"};
	size_t tamanhoRoteiro = sizeof(roteiro) / sizeof(roteiro[0]);
	escreverRoteiro(roteiro, tamanhoRoteiro);

	sleep(4);
	telaSistema();
}

void telaRegistroEmpresa() {
	char *roteiro[] = {"Você está registrando uma nova empresa no sistema.", "Insira os dados de forma sequencial", "Siga todas as instruções do sistema."};
	size_t tamanhoRoteiro = sizeof(roteiro) / sizeof(roteiro[0]);
	escreverRoteiro(roteiro, tamanhoRoteiro);

	char responsavel[32], cpf[16], nomeEmpresa[64], cnpj[20], razaoSocial[64], nomeFantasia[64], endereco[256], email[64], abertura[32];
	
	escreverFrasePadrao("Responsavel (Nome e Sobrenome): ");
	getchar();
	scanf("%31[0-9a-zA-Z ]", responsavel);

	escreverFrasePadrao("CPF do Responsável (apenas numeros): ");
	getchar();
	scanf("%15[0-9a-zA-Z ]", cpf);

	escreverFrasePadrao("Nome da Empresa:");
	getchar();
	scanf("%63[0-9a-zA-Z ]", nomeEmpresa);

	escreverFrasePadrao("CNPJ da Empresa (apenas numeros):");
	getchar();
	scanf("%19[0-9a-zA-Z ]", cnpj);

	escreverFrasePadrao("Razão Social:");
	getchar();
	scanf("%63[0-9a-zA-Z ]", razaoSocial);

	escreverFrasePadrao("Nome de Fantasia:");
	getchar();
	scanf("%63[0-9a-zA-Z ]", nomeFantasia);

	escreverFrasePadrao("Endereço da Empresa (Estado, Cidade, Bairro, Rua e numero):");
	getchar();
	scanf("%[0-9a-zA-Z-, ][^,] ", endereco);

	escreverFrasePadrao("Email da Empresa (exemplo@gmail.com):");
	getchar();
	scanf("%63s", email);

	escreverFrasePadrao("Data de abertura (xx/xx/xxxx):");
	getchar();
	scanf("%31s", abertura);

	struct empresa e = fabricarEmpresa(responsavel, cpf, nomeEmpresa, cnpj, razaoSocial, nomeFantasia, endereco, email, abertura);
	struct relatorio r = fabricarRelatorio(e.cnpj, 0, 0);

	salvarEmpresa(e);
	salvarRelatorio(r);
	telaRegistroEmpresaSucesso();
}

int empresaExiste(char *cnpj) {
	char nomeArquivo[64];
	char linha[128];

	sprintf(nomeArquivo, "relatorio-%s.txt", cnpj);

	FILE *f = fopen(nomeArquivo, "r");
	// File does exists, guard clause
	if (f == 0) { return 0; }

	fscanf(f, "%s\n", linha);
	char *byte = strtok(linha, ";");

	if (strcmp(cnpj, byte) == 0) { fclose(f); return 1; }

	fclose(f);

	return 0;
}

void telaEmpresaNaoExiste() {
	char *roteiro[] = {"Oops... Parce que o CNPJ da empresa que você inseriu não existe", "Por favor, adicione uma nova empresa ao sistema e tente novamente", "Você sera redirecionado ao menu do sistema em alguns segundos"};
	size_t tamanhoRoteiro = sizeof(roteiro) / sizeof(roteiro[0]);
	escreverRoteiro(roteiro, tamanhoRoteiro);

	sleep(4);
	telaSistema();
}

void telaRelatorioEditadoComSucesso() {
	char *roteiro[] = {"O relátorio foi alterado e exportado com sucesso", "Os relátorios seguem o formato relatório-{CNPJ}.txt", "As informações estão dispostas de um modo que o arquivo seja portável", "Você sera redirecionado ao menu do sistema em alguns segundos"};
	size_t tamanhoRoteiro = sizeof(roteiro) / sizeof(roteiro[0]);
	escreverRoteiro(roteiro, tamanhoRoteiro);

	sleep(4);
	telaSistema();
}

void telaEditarRelatorio() {
	char *roteiro[] = {"Você está no menu de gerenciamento de relatórios.", "Olá funcionário, siga as instruções do sistema", "Você precisa digitar o CNPJ da empresa desejada", "Ao editar um relatório, ele automaticamente é exportado"};
	size_t tamanhoRoteiro = sizeof(roteiro) / sizeof(roteiro[0]);
	escreverRoteiro(roteiro, tamanhoRoteiro);

	char cnpj[24];

	escreverFrasePadrao("Digite o CNPJ da empresa desejada (somente números):");
	scanf("%23s", cnpj);

	if (!empresaExiste(cnpj)) { telaEmpresaNaoExiste(); }

	unsigned int insumosSemestre, gastosMensais;
	
	escreverFrasePadrao("Digite a quantidade de insumos produzidos no semestre (cada unidade representa uma tonelada):");
	scanf("%i", &insumosSemestre);

	escreverFrasePadrao("Digite a quantidade de gastos mensais (apenas números):");
	scanf("%i", &gastosMensais);

	struct relatorio r = fabricarRelatorio(cnpj, insumosSemestre, gastosMensais);

	salvarRelatorio(r);

	telaRelatorioEditadoComSucesso();
}

void telaSistema() {
	char *roteiro[] = {"Você está autenticado e no menu principal do sistema.", "Olá, seja bem-vindo", "Nesse menu você consegue acessar todas as funcionalidades de gerenciamento do sistema", "Digite o número da funcionalidade que deseja acessar", "(1) Registrar uma nova empresa no sistema", "(2) Editar e exportar o relatório de alguma empresa", "(3) Exibir relátorio de empresa em tela"};
	size_t tamanhoRoteiro = sizeof(roteiro) / sizeof(roteiro[0]);
	escreverRoteiro(roteiro, tamanhoRoteiro);

	uint8_t escolha;
	scanf("%hhu", &escolha);

	switch (escolha) {
		case 1: telaRegistroEmpresa(); break;
		case 2: telaEditarRelatorio(); break;
		default: puts("Essa funcionalidade não existe...");
	}
}

void telaRegistroSucesso()
{
	char *roteiro[] = {"Seu novo usuário foi registrado com sucesso.", "Você sera redirecionado para o menu inicial em alguns segundos."};
	size_t tamanhoRoteiro = sizeof(roteiro) / sizeof(roteiro[0]);

	escreverRoteiro(roteiro, tamanhoRoteiro);

	sleep(3);

	telaInicial();
}

void telaCadastroFuncionario(){
	char *roteiro[] = {"Olá, você precisa realizar o cadastro de um novo usuário, para posteriormente realizar o login no sistema.", "O sistema precisa que você escolha um nome de usuário e uma senha"};
	size_t tamanhoRoteiro = sizeof(roteiro) / sizeof(roteiro[0]);
	escreverRoteiro(roteiro, tamanhoRoteiro);

	char nome[48], senha[64];

	escreverFrasePadrao("Digite algum nome de usuário.");
	scanf("%s", nome);

	escreverFrasePadrao("Crie uma senha para acessar o sistema.");
	scanf("%s", senha);

	struct usuario u = fabricarUsuario(nome, senha);
	salvarFuncionario(u);

	telaRegistroSucesso();
}

void telaLoginSucesso() {
	char *roteiro[] = {"Você realizou o login com sucesso no sistema.", "Olá, seja bem-vindo novamente ao sistema", "Lembre-se que agora você está autenticado como funcionário", "Você sera redirecionado ao menu do sistema em alguns segundos."};
	size_t tamanhoRoteiro = sizeof(roteiro) / sizeof(roteiro[0]);
	escreverRoteiro(roteiro, tamanhoRoteiro);

	sleep(6);
	telaSistema();
}

void telaLoginIncorreto() {
	char *roteiro[] = {"Oops... Parece que o seu usuário não existe, ou a senha está incorreta.", "Por favor, tente realizar o login no sistema novamente", "Caso ainda não tenha um usuário, realize o cadastro no menu principal", "Você seria redirecionado ao menu principal em alguns segundos"};
	size_t tamanhoRoteiro = sizeof(roteiro) / sizeof(roteiro[0]);
	escreverRoteiro(roteiro, tamanhoRoteiro);

	sleep(6);
	telaInicial();
}

void telaLoginFuncionario() {
	char *roteiro[] = {"Você deve realizar o seu login para acessar o sistema.", "Seja bem vindo novamente funcionário", "Siga as instruções do sistema para realizar o login", "Digite todos os dados como o sistema pede, de forma sequencial"};
	size_t tamanhoRoteiro = sizeof(roteiro) / sizeof(roteiro[0]);
	escreverRoteiro(roteiro, tamanhoRoteiro);

	char nome[48], senha[64];

	escreverFrasePadrao("Digite o seu nome de usuário");
	scanf("%s", nome);

	escreverFrasePadrao("Digite a sua senha");
	scanf("%s", senha);

	if (!conectarFuncionario(nome, senha)) { telaLoginIncorreto(); }

	telaLoginSucesso();
}
