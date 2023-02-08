#define TABLE_MAX 50000000
#define TABLE_INIT 50
#define STRING_MAX 128

/* Estrutura de uma entrada na tabela hash
 * Assim como especificado cada entrada contém dois itens:
 *  - Uma chave do tipo double
 *  - Uma cadeia de caracteres de tamanho maximo 128
 *    - A fim de simplificação foi utilizada uma estrutura de array ao invés de
 *      ponteiros para strings alocadas dinamicamente
 */
typedef struct {
  double chave;
  char dados_satelite[STRING_MAX];
} EntradaHash;

/* A tabela hash será implementada como um vetor de EntradaHash
 * O tamanho inicial será de 50 entradas e a medida que for necessário o vetor
 * será realocado para um vetor de comprimento maior
 *  - Isso foi feito porque como é uma implementação com fins didaticos
 *    dificilmente serão utilizadas milhões de posições
 *  - A realocação será feita caso uma tentativa de inserção resulte em mais de
 * 3 colisões
 */
typedef struct {
  EntradaHash* entradas;
  int tamanho;      // tamanho maximo da tabela
  int n_elementos;  // quantidade de elementos atualmente presentes na tabela
} TabelaHash;

/* Cria uma tabela hash fazendo as alocações de memoria necessarias
 * Em caso de erro de alocação uma mensagem de erro será impressa na saida
 * padrão
 *
 * Retorno: ponteiro para a memoria alocada para a tabela em caso de sucesso
 *          ou NULL caso ocorra erro na alocação
 */
TabelaHash* MakeTable(void);

/* Imprime os elementos da tabela na saida padrão
 */
void ShowTable(TabelaHash* t);

void DestroyTable(TabelaHash* t);

int SizeTable(TabelaHash* t);

void InsertTable(TabelaHash* t, double x, char dados[STRING_MAX]);

char* SearchTable(TabelaHash* t, double x);

int RemoveTable(TabelaHash* t, double x);
