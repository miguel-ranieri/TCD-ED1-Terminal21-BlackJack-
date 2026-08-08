#include <stdlib.h>
#include "mao.h"
#include "cartas.h"

int iniciar_lista_maos(ListaMaos *lista, int aposta_inicial){
    if (lista == NULL)
        return 0;

    MaoJogador nova = (MaoJogador) malloc(sizeof(struct mao_jogador));
    if (nova == NULL)
        return 0;

    nova->cartas_inicio = NULL;
    nova->qtd_cartas = 0;
    nova->total = 0;
    nova->aposta = aposta_inicial;
    nova->finalizada = false;
    nova->estourou = false;
    nova->anterior = NULL;
    nova->proximo = NULL;

    lista->inicio = nova;
    lista->atual = nova;
    lista->total_maos = 1;

    return 1;
}

int pode_split(MaoJogador mao){
    if(mao == NULL || mao->qtd_cartas != 2)
        return 0;
    Carta primeira = mao->cartas_inicio;
    Carta segunda = primeira->prox;
    if (primeira->valor == segunda->valor)
        return 1;
    return 0;
}

int split(ListaMaos *lista, MaoJogador mao_original){
    if (lista == NULL || mao_original == NULL || pode_split(mao_original) == 0)
        return 0;
    MaoJogador nova = (MaoJogador) malloc(sizeof(struct mao_jogador));
    if (nova == NULL)
        return 0;
    Carta primeira = mao_original->cartas_inicio;
    Carta segunda = primeira->prox;
    primeira->prox = NULL;
    mao_original->qtd_cartas = 1;
    calcular_total(&mao_original);
    nova->cartas_inicio = segunda;
    nova->qtd_cartas = 1;
    nova->total = 0;
    calcular_total(&nova);           
    nova->aposta = mao_original->aposta;
    nova->finalizada = false;
    nova->estourou = false;
    nova->anterior = mao_original;
    nova->proximo = mao_original->proximo;
    if (mao_original->proximo != NULL) {
        mao_original->proximo->anterior = nova;
    }
    mao_original->proximo = nova;
    lista->total_maos++;
    return 1;
}

int verificar_estouro(MaoJogador *mao){
    if (mao == NULL || *mao == NULL)
        return 0;
    MaoJogador aux = *mao;
    aux->estourou = (aux->total > 21);
    return 1;
}

int avancar_mao(ListaMaos *lista){
    if(lista == NULL || lista->atual == NULL)
        return 0;
    if(lista->atual->proximo == NULL)
        return 0;
    lista->atual = lista->atual->proximo;
    return 1;
}

int voltar_mao(ListaMaos *lista){
    if(lista == NULL || lista->atual == NULL)
        return 0;
    if(lista->atual->anterior == NULL)
        return 0;
    lista->atual = lista->atual->anterior;
    return 1;
}

int todas_maos_finalizadas(ListaMaos *lista){
    if(lista == NULL)
        return 0;
    MaoJogador aux = lista->inicio; 
    while (aux != NULL) {
        if (!aux->finalizada) {
            return 0;
        }
        aux = aux->proximo;
    }

    return 1;
}

int liberar_lista_maos(ListaMaos *lista){
    if (lista == NULL)
        return 0;
    MaoJogador atual = lista->inicio;
    while (atual != NULL) {
        MaoJogador proxima = atual->proximo;
        liberar_cartas(&atual);
        free(atual);

        atual = proxima;
    }
    lista->inicio = NULL;
    lista->atual = NULL;
    lista->total_maos = 0;
    return 1;
}

