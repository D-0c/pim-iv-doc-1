#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <time.h>
#include <inttypes.h>
#if defined __unix__
# include <unistd.h>
# define CLEAR_SCREEN() system("clear")
#elif defined _WIN32
# include <windows.h>
# define sleep(x) Sleep(1000 * (x))
# define CLEAR_SCREEN() system("cls")
#endif

struct usuario { char *nome; char *senha; };
struct empresa { char *responsavel; char *cpf; char *nomeEmpresa; char *cnpj; char *razaoSocial; 
								 char *nomeFantasia; char* endereco; char *email; char *abertura; };

struct relatorio { char *cnpj; unsigned int totalInsumosSemestre; unsigned int totalGastosMensais; };
int empresaExiste(char *cnpj);
struct relatorio carregarRelatorio(char *cnpj);
void telaPerguntarCNPJRelatorio();

void telaInicial(), telaCadastroFuncionario(), telaLoginFuncionario(), telaSistema();
struct empresa carregarEmpresa(char *cnpj);
int conectarFuncionario(char *nome, char *senha), salvarFuncionario(struct usuario u), salvarEmpresa(struct empresa e);
struct usuario fabricarUsuario(char *nome, char *senha);
struct empresa fabricarEmpresa(char *responsavel, char *cpf, char *nomeEmpresa, char *cnpj, char *razaoSocial, char *nomeFantasia, char *endereco, char *email, char *abertura);

int salvarRelatorio(struct relatorio r) {
	char nomeArquivo[64];
	char linha[128];
	char *sigla = "relatorio";
	char *extensao = "txt";
	FILE *f;

	sprintf(nomeArquivo, "%s-%s.%s", sigla, r.cnpj, extensao);
	f = fopen(nomeArquivo, "w");

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
	char *linha = malloc(256);
	FILE *f;

	f = fopen("empresas-cadastradas.txt", "a");
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
	FILE *f;
	char linha[128];

	f = fopen("funcionarios.txt", "r");
	if (f == 0) { return 0; }

	while (fscanf(f, "%s\n", linha) > 0) {
		char copia[128];
		strcpy(copia, linha);

		if (!funcionarioExiste(nome, copia)) { continue; }

		if (verificarSenhaFuncionario(nome, senha, linha)) { return 1; }
	}
	fclose(f);

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
	int i;
	CLEAR_SCREEN();

	escreverFrasePadrao("(Eco Client, versão: 1.0)");

	for (i = 0; i < tamanho; i++) {
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
			puts("Opção inválida!");
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
	char responsavel[64], cpf[32], nomeEmpresa[64], cnpj[32], razaoSocial[64], nomeFantasia[64], endereco[256], email[64], abertura[64];
	struct empresa e;
	struct relatorio r;
	char *roteiro[] = {"Você está registrando uma nova empresa no sistema.", "Insira os dados de forma sequencial", "Siga todas as instruções do sistema."};
	size_t tamanhoRoteiro = sizeof(roteiro) / sizeof(roteiro[0]);

	escreverRoteiro(roteiro, tamanhoRoteiro);
	
	escreverFrasePadrao("Responsavel (Nome e Sobrenome): ");
	getchar();
  scanf("%63[0-9a-zA-Z ]", responsavel);

	escreverFrasePadrao("CPF do Responsável (apenas numeros): ");
	getchar();
	scanf("%31[0-9a-zA-Z ]", cpf);

	escreverFrasePadrao("Nome da Empresa:");
	getchar();
	scanf("%63[0-9a-zA-Z ]", nomeEmpresa);

	escreverFrasePadrao("CNPJ da Empresa (apenas numeros):");
	getchar();
	scanf("%31[0-9a-zA-Z ]", cnpj);

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
	scanf("%63s", abertura);

	e = fabricarEmpresa(responsavel, cpf, nomeEmpresa, cnpj, razaoSocial, nomeFantasia, endereco, email, abertura);
	r = fabricarRelatorio(e.cnpj, 0, 0);

	salvarEmpresa(e);
	salvarRelatorio(r);
	telaRegistroEmpresaSucesso();
}

int empresaExiste(char *cnpj) {
	char nomeArquivo[64];
	char linha[128];
	FILE *f;
	char *byte;
	char *sigla = "relatorio";
	char *extensao = "txt";

	sprintf(nomeArquivo, "%s-%s.%s", sigla, cnpj, extensao);

	f = fopen(nomeArquivo, "r");

	if (f == 0) { return 0; }

	fscanf(f, "%s\n", linha);
	byte = strtok(linha, ";");

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

char** cortarLinha(char *linha, unsigned int tamanho) {
	char **linhas = malloc(sizeof(char*) * tamanho);
	int index = 0;
	char *token = strtok(linha, ";");

	while (token != NULL) {
		linhas[index++] = token;
		token = strtok(NULL, ";");
	}

	return linhas;
}

struct relatorio carregarRelatorio(char *cnpj) {
	struct relatorio r;
	char caminho[64];
	char linha[128];
	FILE *f;
	char **linhas;
	char *insumosSemestre;
	char *gastosMensal;
	char *sigla = "relatorio";
	char *extensao = "txt";

	if (!empresaExiste(cnpj)) { telaEmpresaNaoExiste(); };

	sprintf(caminho, "%s-%s.%s", sigla, cnpj, extensao);
	f = fopen(caminho, "r");

	fscanf(f, "%s\n", linha);

	linhas = cortarLinha(linha, 3);

	insumosSemestre = linhas[1];
	gastosMensal = linhas[2];

	r.cnpj = cnpj;
	r.totalInsumosSemestre = atoi(insumosSemestre);
	r.totalGastosMensais = atoi(gastosMensal);

	free(linhas);

	return r;
}

char* capturarLinha(FILE *f, char *cnpj) {
	char linha[256];
	while (fgets(linha, sizeof(linha), f)) {
		char *copia = malloc(sizeof(linha));
		char **tokens;
		strcpy(copia, linha);

		tokens = cortarLinha(linha, 9);
		if (strcmp(tokens[0], cnpj) == 0) { return copia; } else { free(copia); }
	}
	return "";
}

struct empresa carregarEmpresa(char *cnpj) {
	struct empresa e;
	char *caminho = "empresas-cadastradas.txt";
	FILE *f;
	char *linha;
	char **tokens;

	if (!empresaExiste(cnpj)) { telaEmpresaNaoExiste(); }

	f = fopen(caminho, "r");

	linha = capturarLinha(f, cnpj);

	tokens = cortarLinha(linha, 9);

	e.cnpj = tokens[0];
	e.cpf = tokens[1];
	e.responsavel = tokens[2];
	e.nomeEmpresa = tokens[3];
	e.abertura = tokens[4];
	e.email = tokens[5];
	e.endereco = tokens[6];
	e.nomeFantasia = tokens[7];
	e.razaoSocial = tokens[8];

	return e;
}

void telaRelatorio(struct empresa e, struct relatorio r) {
	char *roteiro[] = {"Você está visualizando o relatório da empresa escolhida.", "Os dados estão dispostos de maneira a facilitar a leitura", "Tome o tempo que precisar lendo os dados do relatório"};
	size_t tamanhoRoteiro = sizeof(roteiro) / sizeof(roteiro[0]);
	char insumos[64];
	char gastos[64];
	char nomeEmpresa[64];
	char responsavel[64];
	char endereco[128];
	const char* insumosPrefix = "Insumos produzidos no Semestre (toneladas)";
	const char* gastoMensalPrefix = "Gasto mensal da Empresa (real)";
	const char* responsavelPrefix = "Nome do reponsável pela Empresa";
	const char* nomeEmpresaPrefix = "Nome da empresa";
	const char* enderecoPrefix = "Endereço da empresa";
	escreverRoteiro(roteiro, tamanhoRoteiro);

	sprintf(insumos, "%s: %i", insumosPrefix, r.totalInsumosSemestre);
	sprintf(gastos, "%s: %i", gastoMensalPrefix, r.totalGastosMensais);
	sprintf(responsavel, "%s: %s", responsavelPrefix, e.responsavel);
	sprintf(nomeEmpresa, "%s: %s", nomeEmpresaPrefix, e.nomeEmpresa);
	sprintf(endereco, "%s: %s", enderecoPrefix, e.endereco);

	escreverFrasePadrao(nomeEmpresa);
	escreverFrasePadrao(responsavel);
	escreverFrasePadrao(endereco);
	escreverFrasePadrao(insumos);
	escreverFrasePadrao(gastos);

	escreverFrasePadrao("Presisone a tecla (ENTER), para continuar...");
	getchar();
	getchar();
}

void telaPerguntarCNPJRelatorio() {
	char *roteiro[] = {"Você está inserindo um CNPJ para visualizar o relatorio da empresa em tela", "Por favor, siga as instruções do sistema", "Insira os dados de forma sequencial como pede o sistema"};
	size_t tamanhoRoteiro = sizeof(roteiro) / sizeof(roteiro[0]);
	char cnpj[32];
	struct empresa e;
	struct relatorio r;
	escreverRoteiro(roteiro, tamanhoRoteiro);

	escreverFrasePadrao("Digite o CNPJ da empresa qual deseja visualizar o relatorio (apenas números):");
	scanf("%s", cnpj);

	if (!empresaExiste(cnpj)) { telaEmpresaNaoExiste(); }

	e = carregarEmpresa(cnpj);
	r = carregarRelatorio(cnpj);

	telaRelatorio(e, r);
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
	char cnpj[24];
	unsigned int insumosSemestre, gastosMensais;
	struct relatorio r;
	escreverRoteiro(roteiro, tamanhoRoteiro);

	escreverFrasePadrao("Digite o CNPJ da empresa desejada (somente números):");
	scanf("%23s", cnpj);

	if (!empresaExiste(cnpj)) { telaEmpresaNaoExiste(); }

	escreverFrasePadrao("Digite a quantidade de insumos produzidos no semestre (cada unidade representa uma tonelada):");
	scanf("%i", &insumosSemestre);

	escreverFrasePadrao("Digite a quantidade de gastos mensais (apenas números):");
	scanf("%i", &gastosMensais);

	r = fabricarRelatorio(cnpj, insumosSemestre, gastosMensais);

	salvarRelatorio(r);

	telaRelatorioEditadoComSucesso();
}

void telaSistema() {
	char *roteiro[] = {"Você está autenticado e no menu principal do sistema.", "Olá, seja bem-vindo", "Nesse menu você consegue acessar todas as funcionalidades de gerenciamento do sistema", "Digite o número da funcionalidade que deseja acessar", "(1) Registrar uma nova empresa no sistema", "(2) Editar e exportar o relatório de alguma empresa", "(3) Exibir relátorio de empresa em tela", "(4) Para sair do sistema"};
	size_t tamanhoRoteiro = sizeof(roteiro) / sizeof(roteiro[0]);
	uint8_t escolha;
	escreverRoteiro(roteiro, tamanhoRoteiro);

	getchar();
	scanf("%hhu", &escolha);

	switch (escolha) {
		case 1: telaRegistroEmpresa(); break;
		case 2: telaEditarRelatorio(); break;
		case 3: telaPerguntarCNPJRelatorio(); break;
		case 4: exit(0); break;
		default: puts("Opção invalida...");
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
	char nome[48], senha[64];
	struct usuario u;
	char *roteiro[] = {"Olá, você precisa realizar o cadastro de um novo usuário, para posteriormente realizar o login no sistema.", "O sistema precisa que você escolha um nome de usuário e uma senha"};
	size_t tamanhoRoteiro = sizeof(roteiro) / sizeof(roteiro[0]);
	escreverRoteiro(roteiro, tamanhoRoteiro);

	escreverFrasePadrao("Digite algum nome de usuário (apenas letras, não utilize espaços em brancos ou outros caracteres)");
	scanf("%s", nome);

	escreverFrasePadrao("Crie uma senha para acessar o sistema (apenas letras e numeros):");
	scanf("%s", senha);

	u = fabricarUsuario(nome, senha);
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
	char nome[48], senha[64];
	char *roteiro[] = {"Você deve realizar o seu login para acessar o sistema.", "Seja bem vindo novamente funcionário", "Siga as instruções do sistema para realizar o login", "Digite todos os dados como o sistema pede, de forma sequencial"};
	size_t tamanhoRoteiro = sizeof(roteiro) / sizeof(roteiro[0]);
	escreverRoteiro(roteiro, tamanhoRoteiro);

	escreverFrasePadrao("Digite o seu nome de usuário");
	scanf("%s", nome);

	escreverFrasePadrao("Digite a sua senha");
	scanf("%s", senha);

	if (!conectarFuncionario(nome, senha)) { telaLoginIncorreto(); }

	telaLoginSucesso();
}
