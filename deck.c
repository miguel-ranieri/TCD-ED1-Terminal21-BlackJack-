#include <stdio.h>
#include <stdlib.h>
#include "cartas.h"
#include "deck.h"

Deck cria_deck(){
    Deck d;
    d = (Deck)malloc (sizeof(struct deck));
    if(d != NULL){
        d->topo = -1;
    }
    return d;
}

int embaralhar(Deck d) {
    if (d == NULL) {
        return 0;
    }

    char naipes[] = {'C', 'E', 'O', 'P'};
    int idx = 0;

    for (int i = 0; i < 4; i++) {
        for (int valor = 1; valor <= 13; valor++) {
            d->cartas[idx].valor = valor;
            d->cartas[idx].naipe = naipes[i];
            idx++;
        }
    }

    for (int i = 51; i > 0; i--) {
        int j = rand() % (i + 1);

        CartaBaralho temp = d->cartas[i];
        d->cartas[i] = d->cartas[j];
        d->cartas[j] = temp;
    }

    d->topo = 51;

    return 1;
}

int push(Deck d, int valor, char naipe){
    if(d == NULL || d->topo >= 51)
        return 0;
    d->topo++;
    d->cartas[d->topo].naipe = naipe;
    d->cartas[d->topo].valor = valor;
    return 1;
}

int pop(Deck d, CartaBaralho *carta_saida){
    if(d == NULL || deck_vazio(d) == 1)
        return 0;
    (*carta_saida).valor = d->cartas[d->topo].valor;
    (*carta_saida).naipe = d->cartas[d->topo].naipe;
    d->topo--;
    return 1;
}

int deck_vazio(Deck d){
    if(d->topo == -1)
        return 1;
    else
        return 0;
}

void liberar_deck(Deck d){
    free(d);
}
