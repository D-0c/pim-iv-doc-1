#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <time.h>
#include <inttypes.h>

struct usuario { char *senha; signed long id; char *nome; };

void Tela1(), Tela2(), Tela3(), Tela4(), Tela5(), Tela6(), Tela7(), telaRegistroSucesso();
char usu[50];
char senhaString[10];
float GeralResiduo, GeralValor, GeralCusto;
int norte, nordeste, sul, sudeste, centroOeste;

int main(void){
    setlocale(LC_ALL, "Portuguese");
    Tela1();
    return 0;
}

void escreverFrasePadrao(char *frase) {
		printf("%s\n\n", frase);
}

struct usuario fabricarUsuario(char *nome, char *senha)
{
	struct usuario fabricado;

	srand(time(0));

	signed long id = rand() % 100000000000000000;

	fabricado.nome = nome;
	fabricado.senha = senha;
	fabricado.id = id;

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

void Tela1()
{
		char *roteiro[] = {"Olá funcionário, você já possui um login?", "Digite (1) para sim", "Digite (2) para não", "Digite (3) para fechar o programa"};

		size_t tamanhoRoteiro = sizeof(roteiro) / sizeof(roteiro[0]);
    uint8_t escolha;

		escreverRoteiro(roteiro, tamanhoRoteiro);
    scanf("%hhu", &escolha);

    switch(escolha){
        case 1:
            Tela3();
            break;
        case 2:
            Tela2();
            break;
        case 3:
            printf("\nFechando o programa...\n");
            exit(0);
            break;
        default:
            printf("Resposta inv�lida! tente novamente");
            Tela1();
            break;
    }
}

void telaRegistroSucesso()
{
	char *roteiro[] = {"Seu novo usuário foi registrado com sucesso.", "Pressione qualquer tecla para continuar..."};

	size_t tamanhoRoteiro = sizeof(roteiro) / sizeof(roteiro[0]);

	escreverRoteiro(roteiro, tamanhoRoteiro);

	getchar();
	getchar();
}

// Cadastro funcionário.
void Tela2(){
	char *roteiro[] = {"Crie o seu usuário para o login no sistema."};

	size_t tamanhoRoteiro = sizeof(roteiro) / sizeof(roteiro[0]);

	escreverRoteiro(roteiro, tamanhoRoteiro);

	char *nome;
	char *senha;

	escreverFrasePadrao("Digite algum nome de usuário.");
	scanf("%s", nome);

	escreverFrasePadrao("Crie uma senha para acessar o sistema.");
	scanf("%s", senha);

	fabricarUsuario(nome, senha);

	telaRegistroSucesso();
}
void Tela3()
{
    char usuConferir[100];
    char senhaConferir[10];
    system("cls");
    printf("----------------------------\n");
    printf("|     Realize seu login     |\n");
    printf("----------------------------\n\n");
    printf("Digite o seu usu�rio\n");
    scanf("%s",&usuConferir);
    printf("\nDigite a o sua senha\n");
    scanf("%s",&senhaConferir);
    if(strcmp(usuConferir,usu)==0||strcmp(senhaConferir,senhaString)==0)
    {
        printf("\nUsu�rio correto! Seja muito bem vindo %s!",usuConferir);
        Tela4();
    }
    else
    {
        printf("\nDados incorretos... Tente novamente\n");
        system("pause");
        Tela1();
    }
}
void Tela4(){
    system("cls");
    int resp;
    printf("--------------------------------------\n");
    printf("|       O que deseja fazer,%s        |\n",usu);
    printf("--------------------------------------\n\n");
    printf("Op��es:\n");
    printf("[1] Realizar cadastro de uma nova empresa\n");
    printf("[2] Realizar cadastro de uma nova um novo servi�o\n");
    printf("[3] Conferir relat�rios\n");
    scanf("%d",&resp);
    switch(resp){
        case 1:
            Tela6();
            break;
        case 2:
            Tela5();
            break;
        case 3:
            Tela7();
            break;

        default:
            printf("\nResposta inv�lida! digite novamente");
            Tela4();
    }
}
void Tela6(){

    char cpf[20];
    char nomeDo[50];
    char telefoneD[50];
    char cnpj[20];
    char nome[50];
    char razaoSocial[50];
    char nomeFantasia[50];
    char cep[50];
    char rua[50];
    char bairro[50];
    char cidade[50];
    char numeroCep[10];
    char telefone[25];
    char email[50];
    char funcionarios[20];
    system("cls");
    printf("-------------------------------\n");
    printf("|     Cadastre um cliente!     |\n");
    printf("-------------------------------\n\n");

    printf("Insira o CNPJ da empresa\n");
    scanf("%s",&cnpj);
    printf("\nInsira o nome da empresa\n");
    scanf("%s",&nome);
    printf("\nInsira o nome fantasia da empresa\n");
    scanf("%s",&nomeFantasia);
    printf("\nInsira a raz�o social da empresa\n");
    scanf("%s",&razaoSocial);
    printf("\nInsira o n�mero de funcion�rios da empresa\n");
    scanf("%s",&funcionarios);
    printf("\nInsira o CEP da empresa\n");
    scanf("%s",&cep);
    printf("\nInsira a rua da empresa\n");
    scanf("%s",&rua);
    printf("\nInsira o bairro da empresa\n");
    scanf("%s",&bairro);
    printf("\nInsira a cidade da empresa\n");
    scanf("%s",&cidade);
    printf("\nInsira o n�mero de endere�o empresa\n");
    scanf("%s",&numeroCep);
    printf("\nInsira o telefone da empresa\n");
    scanf("%s",&telefone);
    printf("\nInsira o e-mail da empresa\n");
    scanf("%s",&email);
    printf("\nInsira o nome do dono da empresa\n");
    scanf("%s",&nomeDo);
    printf("\nInsira o CPF do dono da empresa\n");
    scanf("%s",&cpf);
    printf("\nInsira o telefone do dono da empresa\n");
    scanf("%s",&telefoneD);

    FILE *file =fopen("empresas_cadastradas.txt","a");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }
    printf("\nInforma��es salvas em empresa.txt!\n");
    fprintf(file,"CNPJ da empresa: %s \n",cnpj);
    fprintf(file,"Nome da empresa: %s \n",nome);
    fprintf(file,"Nome fantasia da empresa: %s \n",nomeFantasia);
    fprintf(file,"Raz�o social da empresa: %s \n",razaoSocial);
    fprintf(file,"Quantidade de funcion�rios: %s \n",funcionarios);
    fprintf(file,"CEP da empresa: %s \n",cep);
    fprintf(file,"Rua da empresa: %s \n",rua);
    fprintf(file,"Bairro da empresa: %s \n",bairro);
    fprintf(file,"Cidade da empresa: %s \n",cidade);
    fprintf(file,"N�mero de endere�o empresa: %s \n",numeroCep);
    fprintf(file,"Telefone da empresa: %s \n",telefone);
    fprintf(file,"Email da empresa: %s \n",email);
    fprintf(file,"CPF do dono da empresa: %s \n",cpf);
    fprintf(file,"Nome do dono da empresa: %s \n",nomeDo);
    fprintf(file,"Telefone do dono da empresa: %s \n",telefoneD);
    fclose(file);
    system("pause");
    Tela4();
}
void Tela5(){
    system("cls");
    char nome[20];
    char regiao[15];
    float residuo, custo, custoRec, valor;
    printf("-------------------------------\n");
    printf("|         Cadastre um         |\n");
    printf("|         novo servi�o        |\n");
    printf("-------------------------------\n\n");

    printf("Insira o nome da empresa: ");
    scanf("%s",&nome);
    printf("\nQuantos kilos de residuo ser�o tratados?:  ");
    scanf("%f",&residuo);
    GeralResiduo+=residuo;

    printf("\nQual ser� o custo operacional desse servi�o?: ");
    scanf("%f",&custo);
    GeralCusto+=custo;

    printf("\nQual ser� o valor cobrado por esse servi�o?: ");
    scanf("%f",&valor);
    GeralValor+=valor;

    printf("\nQual � a regi�o da empresa?");
    scanf("%f",&regiao);

    FILE *file =fopen("servicos.txt","a");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }
    printf("Informa��es registradas no arquivo sericos.txt!\n");
    fprintf(file,"\nNome da empresa: %s, Qtd de res�duos: %.2fKG, Despesas:R$%.2f, Valor do servi�o: R$%.2f, Regi�o: %s\n",nome,residuo,custo,valor, regiao);
    fclose(file);
    system("pause");
    Tela4();
}



void Tela7(){
    int resp;
    FILE *file =fopen("relatorio.txt","w");
        if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
        }
    system("cls");
    printf("--------------------------------\n");
    printf("|       Confira relat�rios      |\n");
    printf("--------------------------------\n\n");
    printf("\nTotal de res�duos tratados (em kg): %.2f\n",GeralResiduo);
    printf("\nDespesas totais: R$%.2f \n",GeralCusto);
    printf("\nFaturamento total: R$%.2f\n",GeralValor);
    printf("\nTemos a op��o de salvar as informa��es em uma arquivo txt, gostaria de salva-las?\nDigite 1 para sim\nDigite 2 para n�o\n");
    scanf("%d",&resp);
    switch(resp){
    case 1:
        fprintf(file,"Res�duos tratados: %.2fKG\nDespesas totais:R$%.2f\nFaturamento total:R$%.2f\n",GeralResiduo,GeralCusto,GeralValor);
        fclose(file);
        printf("\nAs informa��es foram salvas no arquivo 'relatorio.txt'");
    case 2:
       fclose(file);
       system("pause");
       Tela4();
       break;
    }
}
