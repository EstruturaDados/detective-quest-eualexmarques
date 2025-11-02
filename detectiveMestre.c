#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAMANHO_HASH 10

//Eestrutura das pistas
typedef struct PistaNode {
    char descricao[100];
    struct PistaNode* esquerda;
    struct PistaNode* direita;
} PistaNode;

// Estrutura que representa uma sala da mansão
typedef struct Sala {
    char nome[100];
    char pista[100]; // nova linha
    struct Sala* esquerda;
    struct Sala* direita;
} Sala;
//Estrutura para os suspeitos
typedef struct EntradaHash {
    char pista[100];
    char suspeito[50];
    struct EntradaHash* proximo;
} EntradaHash;

EntradaHash* tabela_hash[TAMANHO_HASH];
PistaNode* arvore_pistas = NULL;

// Função que cria uma sala com nome e aloca dinamicamente
Sala* criarSala(const char* nome, const char* pista) {
    Sala* nova = (Sala*)malloc(sizeof(Sala));
    strcpy(nova->nome, nome);
    if (pista != NULL)
        strcpy(nova->pista, pista);
    else
        nova->pista[0] = '\0'; // sem pista
    nova->esquerda = nova->direita = NULL;
    return nova;
}

//Inserindo a pista na avore binaria BST 
void inserirPista(PistaNode** raiz, const char* descricao) {
    if (*raiz == NULL) {
        *raiz = (PistaNode*)malloc(sizeof(PistaNode));
        strcpy((*raiz)->descricao, descricao);
        (*raiz)->esquerda = (*raiz)->direita = NULL;
    } else if (strcmp(descricao, (*raiz)->descricao) < 0) {
        inserirPista(&((*raiz)->esquerda), descricao);
    } else {
        inserirPista(&((*raiz)->direita), descricao);
    }
}
//Função hash
int funcao_hash(const char* chave) {
    int soma = 0;
    for (int i = 0; chave[i] != '\0'; i++) soma += chave[i];
    return soma % TAMANHO_HASH;
}
//Inserindo as pistas aso suspeitos
void inserirNaHash(const char* pista, const char* suspeito) {
    int indice = funcao_hash(pista);
    EntradaHash* nova = (EntradaHash*)malloc(sizeof(EntradaHash));
    strcpy(nova->pista, pista);
    strcpy(nova->suspeito, suspeito);
    nova->proximo = tabela_hash[indice];
    tabela_hash[indice] = nova;
}
//Associando as pistas aos suspeitos
char* encontrarSuspeito(const char* pista) {
    int indice = funcao_hash(pista);
    EntradaHash* atual = tabela_hash[indice];
    while (atual != NULL) {
        if (strcmp(atual->pista, pista) == 0) return atual->suspeito;
        atual = atual->proximo;
    }
    return NULL;
}
//As pistas coletadas que serão usadas para associar aos suspeitos
int contarPistas(PistaNode* raiz, const char* suspeito) {
    if (raiz == NULL) return 0;
    int cont = 0;
    char* s = encontrarSuspeito(raiz->descricao);
    if (s != NULL && strcmp(s, suspeito) == 0) cont++;
    return cont + contarPistas(raiz->esquerda, suspeito) + contarPistas(raiz->direita, suspeito);
}
//Função para jugar quem é o suspeito
void verificarSuspeitoFinal() {
    char nome[50];
    printf("Quem você acusa?\n");
    printf("Joana ou Marcos: ");
    scanf(" %[^\n]", nome);

    int total = contarPistas(arvore_pistas, nome);
    if (total >= 2)
        printf("Acusação válida! %d pistas apontam para %s.\n", total, nome);
    else
        printf("Acusação fraca. Apenas %d pista(s) apontam para %s.\n", total, nome);
}


//Função procuro se tem pista na sala atual
int buscarPista(PistaNode* raiz, const char* chave) {
    if (raiz == NULL) return 0;
    if (strcmp(chave, raiz->descricao) == 0) return 1;
    if (strcmp(chave, raiz->descricao) < 0)
        return buscarPista(raiz->esquerda, chave);
    else
        return buscarPista(raiz->direita, chave);
}
//Função para exibir pistas encontradas
void exibirPistas(PistaNode* raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esquerda);
        printf("- %s\n", raiz->descricao);
        exibirPistas(raiz->direita);
    }
}

// Função que permite ao jogador explorar a mansão interativamente
void explorarSalas(Sala* atual) {
    if (atual == NULL) return;

    printf("\nVocê entrou na sala: %s\n", atual->nome);

    if (strlen(atual->pista) > 0 && !buscarPista(arvore_pistas, atual->pista)) {
        printf("Pista encontrada: %s\n", atual->pista);
        inserirPista(&arvore_pistas, atual->pista);
    }

    // Verifica se não há mais caminhos
    if (atual->esquerda == NULL && atual->direita == NULL) {
        printf("Você chegou ao fim do caminho. Não há mais salas para explorar.\n");
        return;
    }

    char escolha;
    printf("Escolha que ira seguir\n");
    printf("(e) esquerda\n");
    printf("(d) direita\n");
    printf(": ");
    scanf(" %c", &escolha);

    if (escolha == 'e') explorarSalas(atual->esquerda);
    else if (escolha == 'd') explorarSalas(atual->direita);
    else return;
}


//Função para liberar memoria da sala
void liberarSala(Sala* sala) {
    if (sala != NULL) {
        liberarSala(sala->esquerda);
        liberarSala(sala->direita);
        free(sala);
    }
}
//Função para liberar memoria da pista
void liberarPistas(PistaNode* raiz) {
    if (raiz != NULL) {
        liberarPistas(raiz->esquerda);
        liberarPistas(raiz->direita);
        free(raiz);
    }
}
// Função principal que monta o mapa da mansão e inicia a exploração
int main() {

    for (int i = 0; i < TAMANHO_HASH; i++){
        tabela_hash[i] = NULL;
    }

    // Monta a mansão
    Sala* hall = criarSala("Hall de Entrada", NULL);
    Sala* salaEstar = criarSala("Sala de Estar", "Faca ensanguentada");
    Sala* cozinha = criarSala("Cozinha", "Luvas sujas");
    Sala* biblioteca = criarSala("Biblioteca", "Faltando um livro");
    Sala* jardim = criarSala("Jardim", "Pegadas na lama");
    Sala* armario = criarSala("Armario", NULL);

    //Oscaminhas que seram percorridos
    hall->esquerda = salaEstar;
    hall->direita = armario;
    salaEstar->esquerda = biblioteca;
    salaEstar->direita = cozinha;
    cozinha->direita = jardim;

    // Associa pistas a suspeitos
    inserirNaHash("Luvas sujas", "Marcos");
    inserirNaHash("Faca ensanguentada", "Joana");
    inserirNaHash("Pegadas na lama", "Marcos");
    inserirNaHash("Faltando um livro", "Joana");

    // Exploração
    explorarSalas(hall);

    //Mostrar pistas coletadas antes do suspeito final
    printf("\nPistas coletadas:\n");
    exibirPistas(arvore_pistas);

    // Julgamento
    verificarSuspeitoFinal();

    liberarSala(hall);
    liberarPistas(arvore_pistas);

    return 0;
}