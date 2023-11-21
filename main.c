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
	conectarFuncionario("Maria", "matematica180");
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

int verificarSenhaFuncionario(char *senha, char *linha) {
	if (strstr(linha, senha)) {
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

		if (verificarSenhaFuncionario(senha, linha)) { return 1; }
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

void telaRegistroSucesso()
{
	char *roteiro[] = {"Seu novo usuário foi registrado com sucesso.", "Você sera redirecionado para o menu inicial em alguns segundos."};

	size_t tamanhoRoteiro = sizeof(roteiro) / sizeof(roteiro[0]);

	escreverRoteiro(roteiro, tamanhoRoteiro);

	sleep(3);

	telaInicial();
}

void telaCadastroFuncionario(){
	char *roteiro[] = {"Crie o seu usuário para o login no sistema."};
	size_t tamanhoRoteiro = sizeof(roteiro) / sizeof(roteiro[0]);
	escreverRoteiro(roteiro, tamanhoRoteiro);

	char nome[32], senha[64];

	escreverFrasePadrao("Digite algum nome de usuário.");
	scanf("%s", nome);

	escreverFrasePadrao("Crie uma senha para acessar o sistema.");
	scanf("%s", senha);

	fabricarUsuario(nome, senha);

	telaRegistroSucesso();
}

void telaLoginFuncionario()
{
}
