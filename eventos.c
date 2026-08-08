#include <stdlib.h>
#include "eventos.h"
#include <string.h>
#include <stdio.h>

int criar_fila(FilaEventos *fila){
    if (fila == NULL) {
        return 0; 
    }
    fila->ini = NULL;
    fila->fim = NULL;
    fila->total = 0;
    return 1;
}

int enqueue_evento(FilaEventos *fila, const char *mensagem){
    if(fila == NULL || mensagem == NULL)
        return 0;

    struct evento_no * N = (struct evento_no *)malloc(sizeof(struct evento_no));
    if(N == NULL)
        return 0;
    
    strncpy(N->mensagem, mensagem, MSG_MAX - 1);
    N->mensagem[MSG_MAX - 1] = '\0'; 
    N->prox = NULL;
    if(fila_vazia(fila) == 1){
        fila->ini = N;
    }
    else{
        (fila->fim)->prox = N;
    }
    fila->fim = N;
    fila->total++;
    return 1;
}

int dequeue_evento(FilaEventos *fila, char *mensagem){
    if(fila == NULL || mensagem == NULL || fila_vazia(fila) == 1){
        return 0;
    }
    struct evento_no* aux = fila->ini;
    strncpy( mensagem, aux->mensagem, MSG_MAX - 1);
    mensagem[MSG_MAX - 1] = '\0';
    fila->ini = aux->prox;
    if(fila->ini == fila->fim){
        fila->fim = NULL;
    }
    free(aux);
    fila->total--;
    return 1;
}

int fila_vazia(FilaEventos *f){
    if(f->ini == NULL)
        return 1;
    else
        return 0;
}

void imprimir_eventos(FilaEventos *fila){
    if(fila == NULL || fila_vazia(fila) == 1){
        return;
    }
    Evento aux = fila->ini;
    for(int i = 0; i < fila->total; i++){
        printf("%s\n",aux->mensagem);
        aux = aux->prox;
    }
    return;
}

void liberar_fila(FilaEventos *fila) {
    if (fila == NULL) {
        return;
    }
    Evento atual = fila->ini;
    while (atual != NULL) {
        Evento proximo = atual->prox;
        free(atual);
        atual = proximo;
    }
    fila->ini = NULL;
    fila->fim = NULL;
    fila->total = 0;
}