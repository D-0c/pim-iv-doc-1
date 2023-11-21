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

void telaInicial(), telaCadastroFuncionario(), telaLoginFuncionario();
int conectarFuncionario(char *nome, char *senha), salvarFuncionario(struct usuario u);
struct usuario fabricarUsuario(char *nome, char *senha);


int main(void) {
	setlocale(LC_ALL, "Portuguese_Brazil");
	telaInicial();
	return 0;
}

void escreverFrasePadrao(char *frase) {
	printf("%s\n\n", frase);
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

void telaSistema() {
	char *roteiro [] = {"Você está autenticado e no menu principal do sistema.", "Olá, seja bem-vindo", "Nesse menu você consegue acessar todas as funcionalidades de gerenciamento do sistema", "Digite o número da funcionalidade que deseja acessar", "(1) para registrar uma nova empresa no sistema.", "(2) para imprimir o relatório de alguma empresa na tela", "(3) para salvar os relátorios"};
	size_t tamanhoRoteiro = sizeof(roteiro) / sizeof(roteiro[0]);
	escreverRoteiro(roteiro, tamanhoRoteiro);
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
