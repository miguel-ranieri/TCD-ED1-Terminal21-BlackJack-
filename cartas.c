#include <stdio.h>
#include <stdlib.h>
#include "cartas.h"
#include "mao.h"

int hit(MaoJogador *mao, int elem, char naipe){
    Carta N = (Carta) malloc(sizeof(struct no));
    if(N == NULL){
        return 0;
    }
    N->valor = elem;
    N->naipe = naipe;
    N->prox = (*mao)->cartas_inicio;
    (*mao)->cartas_inicio = N;
    (*mao)->qtd_cartas++;
    return 1;
}

int calcular_total(MaoJogador *mao){
    if(mao == NULL || *mao == NULL)
        return 0;
    MaoJogador aux = *mao;
    aux->total = 0;
    Carta c = aux->cartas_inicio;
    while (c != NULL) {
        if (c->valor == 1 && aux->total <= 10) {
            aux->total += 11;
        } else if (c->valor >= 11 && c->valor <= 13) {
            aux->total += 10;
        } else {
            aux->total += c->valor;
        }
        c = c->prox;
    }
    return 1;
}

int liberar_cartas(MaoJogador *mao){
    if (mao == NULL || *mao == NULL)
        return 0;
    MaoJogador aux = *mao;
    Carta c = aux->cartas_inicio;
    while (c != NULL) {
        Carta c2 = c->prox;   
        free(c);
        c = c2;               
    }

    aux->cartas_inicio = NULL;     
    aux->qtd_cartas = 0;
    aux->total = 0;
    return 1;
}