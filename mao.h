#ifndef MAO_H
#define MAO_H

#include <stdbool.h>

typedef struct no *Carta;

typedef struct mao_jogador {
    Carta cartas_inicio;
    int qtd_cartas;
    int total;
    int aposta;
    bool finalizada;
    bool estourou;

    struct mao_jogador *anterior;
    struct mao_jogador *proximo;
} *MaoJogador;

typedef struct {
    MaoJogador inicio;
    MaoJogador atual;
    int total_maos;
} ListaMaos;

int iniciar_lista_maos(ListaMaos *lista, int aposta_inicial);
int pode_split(MaoJogador mao);
int split(ListaMaos *lista, MaoJogador mao_original);
int verificar_estouro(MaoJogador *mao);
int avancar_mao(ListaMaos *lista);
int voltar_mao(ListaMaos *lista);
int todas_maos_finalizadas(ListaMaos *lista);
int liberar_lista_maos(ListaMaos *lista);

#endif