#include "tabela_hash.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void InitTable(TabelaHash *t) {
  t->n_elementos = 0;
  for (int i = 0; i < t->tamanho; ++i) {
    t->entradas[i].chave = -1;
    strcpy(t->entradas[i].dados_satelite, "\0");
  }
}

/* Função de realocação
 * Primeiro ira transferir todos os elementos atuais da tabela para uma pilha
 * Depois irá realocar o vetor para um vetor 10x maior (levando em conta o
 * tamanho maximo de 50 000 000) Em seguida irá reinserir todos os elementos de
 * volta na tabela
 */
void ReallocTable(TabelaHash *t) {
  printf("Realocando vetor de %d elementos para %d elementos\n", t->tamanho,
         t->tamanho * 10);
  if (t->tamanho >= TABLE_MAX) {
    fprintf(stderr, "A tabela ja atingiu o tamanho maximo");
    return;
  }

  EntradaHash elementos[t->n_elementos];
  int p = 0;
  for (int i = 0; i < t->tamanho; ++i) {
    if (t->entradas[i].chave != -1) {
      elementos[p].chave = t->entradas[i].chave;
      strcpy(elementos[p++].dados_satelite, t->entradas[i].dados_satelite);
      if (p == t->n_elementos) {
        break;
      }
    }
  }

  free(t->entradas);
  t->tamanho = t->tamanho * 10;
  t->n_elementos = 0;
  t->entradas = (EntradaHash *)malloc(t->tamanho * sizeof(EntradaHash));
  if (t->entradas == NULL) {
    fprintf(stderr, "Não foi possível alocar memoria para a tabela.\n");
    free(t);
    return;
  }
  InitTable(t);

  for (int i = 0; i < p; ++i) {
    InsertTable(t, elementos[i].chave, elementos[i].dados_satelite);
  }
}

TabelaHash *MakeTable(void) {
  TabelaHash *t = (TabelaHash *)malloc(sizeof(TabelaHash));
  if (t == NULL) {
    fprintf(stderr, "Não foi possível alocar memoria para a tabela.\n");
    return NULL;
  }

  t->entradas = (EntradaHash *)malloc(TABLE_INIT * sizeof(EntradaHash));
  t->tamanho = TABLE_INIT;
  if (t->entradas == NULL) {
    fprintf(stderr, "Não foi possível alocar memoria para a tabela.\n");
    free(t);
    return NULL;
  }

  // Inicializa todos as posições da tabela para -1
  // indicando que a posição está vazia
  InitTable(t);

  return t;
}

void PrintEntry(EntradaHash e) {
  printf("Chave: %.6f // Dados: %s\n", e.chave, e.dados_satelite);
}

void ShowTable(TabelaHash *t) {
  for (int i = 0; i < t->tamanho; i++) {
    if (t->entradas[i].chave != -1) {
      PrintEntry(t->entradas[i]);
    }
  }
}

void DestroyTable(TabelaHash *t) {
  free(t->entradas);
  free(t);
}

int SizeTable(TabelaHash *t) { return t->n_elementos; }

// Calcula o hash de uma chave pelo metodo da reinserção quadratica com fator
// de aumento Caso ocorra colisão a função irá recalcular o valor do hash
// levando em conta quantas tentativas ocorreram Caso na terceira tentativa
// ainda ocorra colisão o vetor será realocado para um com mais posições
int Hash(TabelaHash *t, double chave, unsigned int tentativa) {
  unsigned int pos = (unsigned int)floor(fabs(chave)) % t->tamanho;
  if (tentativa == 1) {
    return pos;
  } else {
    unsigned int inc = pow(chave * (tentativa + 1), 2) / 2;
    return (unsigned int)((pos + inc) * inc) % t->tamanho;
  }
}

void InsertTable(TabelaHash *t, double x, char dados[STRING_MAX]) {
  unsigned int tentativa = 1;
  bool inserido = false;
  unsigned int tentativa_max = t->tamanho * 10;

  while (!inserido) {
    unsigned int pos = Hash(t, x, tentativa);
    tentativa++;
    if (t->entradas[pos].chave == -1) {  // posição vazia
      t->entradas[pos].chave = x;
      strcpy(t->entradas[pos].dados_satelite, dados);
      t->n_elementos++;
      inserido = true;
      printf("Elemento inserido após %d tentativas\n", tentativa);
    } else if (tentativa == tentativa_max + 1 && t->tamanho < TABLE_MAX) {
      // Caso o numero de tentativas seja maior que 10x o tamanho do vetor
      // o vetor será realocado para um com 10x mais posicoes
      ReallocTable(t);
      tentativa = 0;
    }
  }
}

char *SearchTable(TabelaHash *t, double x) {
  unsigned int tentativa = 1;
  unsigned int tentativa_max = t->tamanho * 10;
  while (tentativa <= tentativa_max + 1) {
    unsigned int pos = Hash(t, x, tentativa);
    tentativa++;
    if (t->entradas[pos].chave == -1) {
      break;
    } else if (t->entradas[pos].chave == x) {
      return (t->entradas[pos].dados_satelite);
    }
  }
  return "\0";
}

int RemoveTable(TabelaHash *t, double x) {
  unsigned int tentativa = 1;
  unsigned int tentativa_max = t->tamanho * 10;
  while (tentativa <= tentativa_max + 1) {
    unsigned int pos = Hash(t, x, tentativa);
    tentativa++;
    if (t->entradas[pos].chave == -1) {
      printf("Não há elemento com a chave %.6f na tabela\n", x);
      break;
    } else if (t->entradas[pos].chave == x) {
      t->entradas[pos].chave = -1;
      strcpy(t->entradas[pos].dados_satelite, "\0");
      return 0;
    }
  }
  fprintf(stderr, "Erro ao procurar o elemento com chave %.6lf\n", x);
  return 1;
}
