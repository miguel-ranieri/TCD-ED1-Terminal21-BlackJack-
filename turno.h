#ifndef TURNO_H
#define TURNO_H
 
typedef struct jogador_no {
    int id;
    int fichas;
    int assento;
 
    struct jogador_no *proximo;
} *Jogador;
 
typedef struct {
    Jogador atual;
    int total_jogadores;
} Mesa;
 
int criar_mesa(Mesa *mesa);
int adicionar_jogador(Mesa *mesa);
int avancar_turno(Mesa *mesa);
int remover_jogador(Mesa *mesa, int id);
Jogador jogador_atual(Mesa *mesa);
int liberar_mesa(Mesa *mesa);
 
#endif