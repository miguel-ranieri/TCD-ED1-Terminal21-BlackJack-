#ifndef DECK_H
#define DECK_H
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int valor; 
    char naipe;
} CartaBaralho;

typedef struct deck{
    CartaBaralho cartas[52];
    int topo;
} *Deck;

Deck cria_deck();
int embaralhar(Deck d);
int push(Deck d, int valor, char naipe);
int pop(Deck d, CartaBaralho *carta_saida);
int deck_vazio(Deck d);
void liberar_deck(Deck d);

#endif