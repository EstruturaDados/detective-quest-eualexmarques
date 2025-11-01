#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura que representa uma sala da mansão
typedef struct Sala {
    char nome[50];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// Função que cria uma sala com nome e aloca dinamicamente
Sala* criarSala(const char* nome) {
    Sala* novaSala = (Sala*)malloc(sizeof(Sala));
    if (novaSala == NULL) {
        printf("Erro ao alocar memória para a sala.\n");
        exit(1);
    }
    strcpy(novaSala->nome, nome);
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    return novaSala;
}

// Função que permite ao jogador explorar a mansão interativamente
void explorarSalas(Sala* atual) {
    char escolha;

    while (atual != NULL) {
        printf("\nVocê está na sala: %s\n", atual->nome);

        // Verifica se é uma sala sem caminhos (nó-folha)
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("Fim da exploração. Você chegou ao último cômodo.\n");
            break;
        }

        printf("\nEscolha um caminho que ira seguir\n");
        printf("(e) esquerda\n");
        printf("(d) direita\n");
        printf("(s) sair\n");
        printf(": ");
        scanf(" %c", &escolha);

        if (escolha == 'e') {
            if (atual->esquerda != NULL) {
                atual = atual->esquerda;
            } else {
                printf("Não há caminho à esquerda.\n");
            }
        } else if (escolha == 'd') {
            if (atual->direita != NULL) {
                atual = atual->direita;
            } else {
                printf("Não há caminho à direita.\n");
            }
        } else if (escolha == 's') {
            printf("O detetive desistiu.\n");
            break;
        } else {
            printf("Opção inválida. Tente novamente.\n");
        }
    }
}

// Função principal que monta o mapa da mansão e inicia a exploração
int main() {
    // Criação manual da árvore binária (mapa da mansão)
    Sala* hall = criarSala("Hall de Entrada");
    Sala* salaEstar = criarSala("Sala de Estar");
    Sala* cozinha = criarSala("Cozinha");
    Sala* biblioteca = criarSala("Biblioteca");
    Sala* jardim = criarSala("Jardim");

    // Montagem da árvore
    hall->esquerda = salaEstar;
    hall->direita = cozinha;
    salaEstar->esquerda = biblioteca;
    salaEstar->direita = jardim;

    // Início da exploração
    printf("Bem-vindo ao Detective Quest!\n");
    printf("Explore a mansão para encontrar pistas.\n");
    explorarSalas(hall);

    // Liberação de memória
    free(jardim);
    free(biblioteca);
    free(cozinha);
    free(salaEstar);
    free(hall);

    return 0;
}