#define FICHAS_INICIAIS 1500

#include <stdlib.h>
#include "turno.h"

int criar_mesa(Mesa *mesa){
    if (mesa == NULL)
        return 0;

    mesa->atual = NULL;
    mesa->total_jogadores = 0;
    return 1;
}

int adicionar_jogador(Mesa *mesa){
    if (mesa == NULL)
        return 0;

    Jogador novo = (Jogador) malloc(sizeof(struct jogador_no));
    if (novo == NULL)
        return 0;

    novo->id = mesa->total_jogadores + 1;
    novo->fichas = FICHAS_INICIAIS;
    novo->assento = mesa->total_jogadores;
    if (mesa->atual == NULL) {
        novo->proximo = novo;
        mesa->atual = novo;
    } else {
        Jogador ultimo = mesa->atual;
        while (ultimo->proximo != mesa->atual)
            ultimo = ultimo->proximo;
        ultimo->proximo = novo;
        novo->proximo = mesa->atual;
    }
    mesa->total_jogadores++;
    return 1;
}

int avancar_turno(Mesa *mesa){
    if(mesa == NULL || mesa->atual == NULL)
        return 0;

    mesa->atual = mesa->atual->proximo;
    return 1;
}

int remover_jogador(Mesa *mesa, int id){
    if(mesa == NULL || mesa->atual == NULL || mesa->total_jogadores == 0)
        return 0;
    if (mesa->total_jogadores == 1) {
        if (mesa->atual->id == id) {
            free(mesa->atual);
            mesa->atual = NULL;
            mesa->total_jogadores = 0;
            return 1;
        }
        return 0;
    }
    Jogador aux = mesa->atual;
    while(aux->proximo->id != id && aux->proximo != mesa->atual){
        aux = aux->proximo;
    }
    if(aux->proximo->id != id)
        return 0;
    
    Jogador aux2 = aux->proximo;
    aux->proximo = aux2->proximo;
    if (mesa->atual == aux2) {
        mesa->atual = aux2->proximo;
    }
    free(aux2);
    mesa->total_jogadores--;
    return 1;
}

Jogador jogador_atual(Mesa *mesa){
    return mesa->atual;
}

int liberar_mesa(Mesa *mesa){
    if (mesa == NULL)
        return 0;
    Jogador atual = mesa->atual;
    for(int i = 0; i < mesa->total_jogadores; i++) {
        Jogador proxima = atual->proximo;
        free(atual);
        atual = proxima;
    }
    mesa->atual = NULL;
    mesa->total_jogadores = 0;
    return 1;
}