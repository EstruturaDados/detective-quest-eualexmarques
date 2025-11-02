#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Eestrutura das pistas
typedef struct PistaNode {
    char valor[100];
    struct PistaNode* esquerda;
    struct PistaNode* direita;
} PistaNode;

// Estrutura que representa uma sala da mansão
typedef struct Sala {
    char nome[100];
    char* pista; // Pista associada ao cômodo (opcional)
    struct Sala* esquerda;
    struct Sala* direita;
} Sala;


// Função que cria uma sala com nome e aloca dinamicamente
Sala* criarSala(const char* nome, const char* pista) {
    Sala* nova = (Sala*)malloc(sizeof(Sala));
    if (nova == NULL) {
        printf("Erro ao alocar memória para sala.\n");
        exit(1);
    }
    strcpy(nova->nome, nome);
    nova->esquerda = NULL;
    nova->direita = NULL;
    if (pista != NULL) {
        nova->pista = malloc(strlen(pista) + 1);
        strcpy(nova->pista, pista);
    } else {
        nova->pista = NULL;
    }
    return nova;
}
//Inserindo a pista na avore binaria BST 
PistaNode* inserirPista(PistaNode* raiz, const char* valor) {
    if (raiz == NULL) {
        PistaNode* nova = (PistaNode*)malloc(sizeof(PistaNode));
        strcpy(nova->valor, valor);
        nova->esquerda = NULL;
        nova->direita = NULL;
        return nova;
    }
    if (strcmp(valor, raiz->valor) < 0)
        raiz->esquerda = inserirPista(raiz->esquerda, valor);
    else if (strcmp(valor, raiz->valor) > 0)
        raiz->direita = inserirPista(raiz->direita, valor);
    return raiz;
}
//Função procuro se tem pista na sala atual
int buscarPista(PistaNode* raiz, const char* chave) {
    if (raiz == NULL) return 0;
    if (strcmp(chave, raiz->valor) == 0) return 1;
    if (strcmp(chave, raiz->valor) < 0)
        return buscarPista(raiz->esquerda, chave);
    else
        return buscarPista(raiz->direita, chave);
}
//Função para exibir pistas encontradas
void exibirPistas(PistaNode* raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esquerda);
        printf("- %s\n", raiz->valor);
        exibirPistas(raiz->direita);
    }
}

// Função que permite ao jogador explorar a mansão interativamente
void explorarSalasComPistas(Sala* atual, PistaNode** pistas) {
    char escolha;
    while (atual != NULL) {
        printf("\nVocê está na sala: %s\n", atual->nome);

        // Coleta automática de pista
        if (atual->pista != NULL && !buscarPista(*pistas, atual->pista)) {
            printf("Você encontrou uma pista: %s\n", atual->pista);
            *pistas = inserirPista(*pistas, atual->pista);
        }

        // Menu de navegação
        printf("\nEscolha um caminho:\n");
        if (atual->esquerda) printf("(e) Ir para a esquerda: %s\n", atual->esquerda->nome);
        if (atual->direita)  printf("(d) Ir para a direita: %s\n", atual->direita->nome);
        printf("(p) pistas encontradas\n");
        printf("(s) Sair da mansão\n");
        printf("Opção: ");
        scanf(" %c", &escolha);

        if (escolha == 'e' && atual->esquerda != NULL)
            atual = atual->esquerda;
        else if (escolha == 'd' && atual->direita != NULL)
            atual = atual->direita;
        else if (escolha == 'p'){
            printf("\nPistas coletadas até agora:\n");
            if (*pistas)
            exibirPistas(*pistas);//Ira exibir pistas coletadas
            else 
            printf("Nenhuma pista foi coletada ainda.\n");

        }else if (escolha == 's') {
            printf("\nSaindo da mansão...\n");
            break;
        } else {
            printf("Caminho inválido. Tente novamente.\n");
        }
    }
}
//Função para liberar memoria da sala
void liberarSala(Sala* sala) {
    if (sala != NULL) {
        liberarSala(sala->esquerda);
        liberarSala(sala->direita);
        if (sala->pista) free(sala->pista);
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
    // Criação do mapa fixo da mansão
    Sala* hall = criarSala("Hall de Entrada", NULL);
    Sala* salaEstar = criarSala("Sala de Estar", NULL);
    Sala* cozinha = criarSala("Cozinha", "Chave do porão");
    Sala* biblioteca = criarSala("Biblioteca", "Pegadas de lama");
    Sala* jardim = criarSala("Jardim", NULL);

    // Montagem da árvore de salas
    hall->esquerda = salaEstar;
    hall->direita = cozinha;
    salaEstar->esquerda = biblioteca;
    salaEstar->direita = jardim;

    // Inicializa árvore de pistas
    PistaNode* pistasColetadas = NULL;

    // Início da exploração
    printf("Bem-vindo ao Detective Quest!\n");
    printf("Explore a mansão e colete pistas para resolver o mistério.\n");

    explorarSalasComPistas(hall, &pistasColetadas);

    liberarSala(hall);
    liberarPistas(pistasColetadas);

    return 0;
}