#ifndef EVENTOS_H
#define EVENTOS_H
 
#define MSG_MAX 80
 
typedef struct evento_no {
    char mensagem[MSG_MAX];
    struct evento_no *prox;
} *Evento;
 
typedef struct {
    Evento ini;
    Evento fim;
    int total;
} FilaEventos;

int criar_fila(FilaEventos *fila);
int enqueue_evento(FilaEventos *fila, const char *mensagem);
int dequeue_evento(FilaEventos *fila, char *mensagem);
int fila_vazia(FilaEventos *fila);
void imprimir_eventos(FilaEventos *fila);
void liberar_fila(FilaEventos *fila);
 
#endif