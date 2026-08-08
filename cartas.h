#ifndef CARTAS_H
#define CARTAS_H
#include "mao.h"

struct no{
    int valor;
    char naipe;
    struct no *prox;
};

int hit(MaoJogador *mao, int elem, char naipe);
int calcular_total(MaoJogador *mao);
int liberar_cartas(MaoJogador *mao);

#endif