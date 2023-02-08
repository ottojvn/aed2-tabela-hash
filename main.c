#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "tabela_hash.h"

void print_menu(TabelaHash *t);
void processa_input(TabelaHash *t, short input);
void LerArquivo(TabelaHash *t, const char *a);

enum comandos {
  INSERIR = 1,
  REMOVER,
  CONSULTAR,
  IMPRIMIR,
  QUANTIDADE,
  ARQUIVO,
  SAIR
};

int main(void) {
  TabelaHash *tabela = MakeTable();
  if (tabela == NULL) {
    return 1;
  }
  short input = 0;
  while (input != SAIR) {
    print_menu(tabela);
    printf("\n");
    printf("Digite o numero da função que deseja executar: ");
    int s = scanf("%hd", &input);
    if (s == 0) {
      continue;
    }
    printf("\n");
    processa_input(tabela, input);
    printf("\n");
  }

  return 0;
}

void print_menu(TabelaHash *t) {
  printf("%d - Inserir novo elemento\n", INSERIR);
  if (t->n_elementos != 0) {
    printf("%d - Remover elemento\n", REMOVER);
    printf("%d - Consultar chave\n", CONSULTAR);
    printf("%d - Imprimir elementos\n", IMPRIMIR);
    printf("%d - Consultar quantidade de elementos\n", QUANTIDADE);
  }
  printf("%d - Ler arquivo com entradas\n", ARQUIVO);
  printf("%d - Sair\n", SAIR);
}

void processa_input(TabelaHash *t, short input) {
  double chave;
  char dados[STRING_MAX] = "\0";
  int len;
  switch (input) {
    case INSERIR:
      do {
        printf("Qual numero deseja inserir na tabela?\n");
      } while (scanf("%lf", &chave) == 0);

      fgets(dados, STRING_MAX, stdin);

      do {
        printf("Dados adicionais (string no maximo 127 caracteres)\n");
      } while (fgets(dados, STRING_MAX, stdin) == NULL);

      // remover o '\n' do final da string
      len = strlen(dados);
      if (dados[len - 1] == '\n') {
        dados[len - 1] = '\0';
      }

      printf("\n");
      InsertTable(t, chave, dados);
      break;

    case REMOVER:
      do {
        printf("Qual chave deseja remover da tabela?\n");
      } while (scanf("%lf", &chave) == 0);
      printf("\n");
      RemoveTable(t, chave);
      break;

    case CONSULTAR:
      do {
        printf("Qual chave deseja consultar?\n");
      } while (scanf("%lf", &chave) == 0);
      printf("\n");
      strcpy(dados, SearchTable(t, chave));
      if (strcmp(dados, "\0")) {
        printf("%s\n", dados);
      } else {
        printf("A chave não foi encontrada na tabela\n");
      }
      break;

    case IMPRIMIR:
      ShowTable(t);
      break;

    case QUANTIDADE:
      printf("A tabela possui %d elementos\n", SizeTable(t));
      break;

    case ARQUIVO:
      fgets(dados, STRING_MAX, stdin);
      printf("Digite o nome do arquivo\n");
      fgets(dados, STRING_MAX, stdin);
      // remover o '\n' do final da string
      len = strlen(dados);
      if (dados[len - 1] == '\n') {
        dados[len - 1] = '\0';
      }
      LerArquivo(t, dados);
      break;

    case SAIR:
      DestroyTable(t);
      break;

    default:
      printf("\nEntrada invalida\n");
      break;
  }
}

void LerArquivo(TabelaHash *t, const char *a) {
  FILE *file = fopen(a, "r");
  if (file == NULL) {
    fprintf(stderr, "Erro ao abrir o arquivo\n");
    return;
  }

  char buffer[1024];
  while (fgets(buffer, 1024, file) != NULL) {
    double chave;
    char dados[STRING_MAX] = "\0";
    sscanf(buffer, "%lf %127[^\n]", &chave, dados);
    dados[strlen(dados)] = '\0';
    InsertTable(t, chave, dados);
  }
}
