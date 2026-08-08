#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "cartas.h"
#include "mao.h"
#include "deck.h"
#include "turno.h"
#include "eventos.h"

#define COR_RESET    "\033[0m"
#define COR_NEGRITO  "\033[1m"
#define COR_VERMELHO "\033[31m"
#define COR_VERDE    "\033[32m"
#define COR_AMARELO  "\033[33m"
#define COR_CIANO    "\033[36m"

void imprimir_separador(void) {
    printf(COR_NEGRITO "----------------------------------------\n" COR_RESET);
}

const char *simbolo_naipe(char naipe) {
    switch (naipe) {
        case 'C': return "C"; // Copas
        case 'O': return "O"; // Ouros
        case 'E': return "E"; // Espadas
        case 'P': return "P"; // Paus
        default:  return "?";
    }
}

void imprimir_carta(int valor, char naipe) {
    const char *cor = (naipe == 'C' || naipe == 'O') ? COR_VERMELHO : COR_RESET;
    const char *nome_valor;
    switch (valor) {
        case 1:  nome_valor = "A";  break;
        case 11: nome_valor = "J";  break;
        case 12: nome_valor = "Q";  break;
        case 13: nome_valor = "K";  break;
        default: {
            static char buf[4];
            snprintf(buf, sizeof(buf), "%d", valor);
            nome_valor = buf;
        }
    }
    printf("%s[%s%s]%s ", cor, nome_valor, simbolo_naipe(naipe), COR_RESET);
}

void imprimir_cartas_mao(MaoJogador mao) {
    if (mao == NULL || mao->cartas_inicio == NULL) 
        return;

    printf("Cartas: ");
    Carta aux = mao->cartas_inicio;
    while (aux != NULL) {
        imprimir_carta(aux->valor, aux->naipe);
        aux = aux->prox;
    }
    printf("\n");
}

int main() {
    srand((unsigned int) time(NULL));
    Deck d = cria_deck();
    if (d == NULL) {
        printf("Erro ao criar o baralho.\n");
        return 1;
    }
    embaralhar(d);
    Mesa m;
    criar_mesa(&m);
    int num_jogadores;
    printf(COR_NEGRITO "Digite o numero de jogadores: " COR_RESET);
    if (scanf("%d", &num_jogadores) != 1) {
        while (getchar() != '\n');
        num_jogadores = 0;
    }

    for (int i = 0; i < num_jogadores; i++) {
        adicionar_jogador(&m);
    }

    FilaEventos f;
    criar_fila(&f);
    ListaMaos *listas = malloc(sizeof(ListaMaos) * (num_jogadores > 0 ? num_jogadores : 1));
    if (listas == NULL) {
        printf("Erro de memoria ao criar as listas de mao.\n");
        return 1;
    }
    int opc = 1;
    CartaBaralho c;

    while (opc != 0) {

        printf("\n");
        imprimir_separador();
        printf(COR_NEGRITO COR_AMARELO "           NOVA RODADA\n" COR_RESET);
        imprimir_separador();
        for (int j = 0; j < m.total_jogadores; j++) {
            Jogador j_atual = jogador_atual(&m);
            if (j_atual->fichas <= 0) {
                printf(COR_AMARELO "\n[!] Jogador %d perdeu tudo, mas achou uma ficha de 100 no chao!\n" COR_RESET, j_atual->id);
                j_atual->fichas = 100;
            }
            printf("\n");
            imprimir_separador();
            printf(COR_NEGRITO "Vez do Jogador %d " COR_RESET "(Fichas: " COR_VERDE "%d" COR_RESET ")\n",
                   j_atual->id, j_atual->fichas);

            int aposta_inicial;
            do {
                printf("Digite o valor da sua aposta: ");
                if (scanf("%d", &aposta_inicial) != 1) {
                    while (getchar() != '\n');
                    aposta_inicial = -1;
                }
            } while (aposta_inicial <= 0 || aposta_inicial > j_atual->fichas);

            iniciar_lista_maos(&listas[j], aposta_inicial);
            enqueue_evento(&f, "Novo jogador iniciou a rodada");
            for (int i = 0; i < 2; i++) {
                if (!pop(d, &c)) {
                    printf(COR_VERMELHO "Baralho vazio!\n" COR_RESET);
                    break;
                }
                hit(&listas[j].atual, c.valor, c.naipe);
            }
            calcular_total(&listas[j].atual);
            while (todas_maos_finalizadas(&listas[j]) != 1) {
                printf("\n--- Mao Ativa ---\n");
                imprimir_cartas_mao(listas[j].atual);
                printf("Pontuacao total: " COR_NEGRITO "%d" COR_RESET " | Aposta: %d\n",
                       listas[j].atual->total, listas[j].atual->aposta);

                if (listas[j].atual->finalizada) {
                    printf(COR_AMARELO "Essa mao ja foi finalizada.\n" COR_RESET);
                    printf("1 - Proxima mao\n");
                    printf("2 - Mao anterior\n");
                    printf("Opcao: ");
                    int opc_nav;
                    if (scanf("%d", &opc_nav) != 1) {
                        while (getchar() != '\n');
                        opc_nav = -1;
                    }
                    if (opc_nav == 1) {
                        avancar_mao(&listas[j]);
                    } else if (opc_nav == 2) {
                        voltar_mao(&listas[j]);
                    } else {
                        printf(COR_VERMELHO "Opcao invalida!\n" COR_RESET);
                    }
                    continue;
                }

                if (listas[j].atual->total >= 21) {
                    if (listas[j].atual->total > 21) {
                        printf(COR_VERMELHO "Mao estourou!\n" COR_RESET);
                        listas[j].atual->estourou = true;
                        enqueue_evento(&f, "Mao estourou!");
                    } else {
                        printf(COR_VERDE "Blackjack!\n" COR_RESET);
                    }
                    listas[j].atual->finalizada = true;
                    avancar_mao(&listas[j]);
                    continue;
                }

                int opc2;
                printf("\nEscolha sua jogada:\n");
                printf("1 - Hit (Pedir Carta)\n");
                printf("2 - Stand (Parar)\n");
                if (pode_split(listas[j].atual)) {
                    printf("3 - Split (Dividir Mao)\n");
                }
                if (listas[j].total_maos > 1) {
                    printf("4 - Proxima mao\n");
                    printf("5 - Mao anterior\n");
                }
                printf("Opcao: ");
                if (scanf("%d", &opc2) != 1) {
                    while (getchar() != '\n');
                    opc2 = -1;
                }

                switch (opc2) {
                    case 1:
                        if (pop(d, &c) == 0) {
                            printf(COR_VERMELHO "Baralho vazio!\n" COR_RESET);
                            break;
                        }
                        hit(&listas[j].atual, c.valor, c.naipe);
                        calcular_total(&listas[j].atual);
                        enqueue_evento(&f, "Jogador pediu carta (Hit)");
                        break;

                    case 2:
                        listas[j].atual->finalizada = true;
                        enqueue_evento(&f, "Jogador parou (Stand)");
                        avancar_mao(&listas[j]);
                        break;

                    case 3:
                        if (pode_split(listas[j].atual)) {
                            split(&listas[j], listas[j].atual);
                            enqueue_evento(&f, "Jogador realizou Split");
                        } else {
                            printf(COR_VERMELHO "Split invalido!\n" COR_RESET);
                        }
                        break;

                    case 4:
                        if (listas[j].total_maos > 1) {
                            avancar_mao(&listas[j]);
                        }
                        break;

                    case 5:
                        if (listas[j].total_maos > 1) {
                            voltar_mao(&listas[j]);
                        }
                        break;

                    default:
                        printf(COR_VERMELHO "Opcao invalida!\n" COR_RESET);
                        break;
                }
            }
            avancar_turno(&m);
        }
        printf("\n");
        imprimir_separador();
        //VEZ DO DEALER
        printf(COR_NEGRITO COR_AMARELO "         VEZ DO DEALER\n" COR_RESET);
        imprimir_separador();
        int total_dealer = 0;
        while (total_dealer < 17) {
            if (pop(d, &c) == 0) {
                printf(COR_VERMELHO "Baralho vazio!\n" COR_RESET);
                break;
            }
            int valor_carta = c.valor;
            if (valor_carta == 1 && total_dealer <= 10) {
                valor_carta = 11;
            } else if (valor_carta >= 11 && valor_carta <= 13) {
                valor_carta = 10;
            }
            total_dealer += valor_carta;
            printf("Dealer tirou ");
            imprimir_carta(c.valor, c.naipe);
            printf(". Total: %d\n", total_dealer);
        }
        if (total_dealer > 21) {
            printf(COR_VERMELHO COR_NEGRITO "Dealer Estourou!\n" COR_RESET);
            enqueue_evento(&f, "Dealer estourou!");
        }
        printf("\n");
        imprimir_separador();
        printf(COR_NEGRITO "         RESULTADOS\n" COR_RESET);
        imprimir_separador();
        for (int j = 0; j < m.total_jogadores; j++) {
            Jogador j_atual = jogador_atual(&m);
            MaoJogador aux_mao = listas[j].inicio;

            while (aux_mao != NULL) {
                if (!aux_mao->estourou && (total_dealer > 21 || aux_mao->total > total_dealer)) {
                    j_atual->fichas += aux_mao->aposta;
                    printf(COR_VERDE "Jogador %d VENCEU a mao! (+%d fichas)\n" COR_RESET,
                           j_atual->id, aux_mao->aposta);
                } else if (aux_mao->estourou || aux_mao->total < total_dealer) {
                    j_atual->fichas -= aux_mao->aposta;
                    printf(COR_VERMELHO "Jogador %d PERDEU a mao! (-%d fichas)\n" COR_RESET,
                           j_atual->id, aux_mao->aposta);
                } else {
                    printf(COR_AMARELO "Jogador %d EMPATOU a mao!\n" COR_RESET, j_atual->id);
                }
                aux_mao = aux_mao->proximo;
            }

            avancar_turno(&m);
        }
        printf("\n");
        imprimir_separador();
        printf(COR_NEGRITO "     LOG DE EVENTOS DA RODADA\n" COR_RESET);
        imprimir_separador();
        imprimir_eventos(&f);
        liberar_fila(&f);
        criar_fila(&f);
        for (int j = 0; j < num_jogadores; j++) {
            liberar_lista_maos(&listas[j]);
        }
        printf("\nDeseja jogar mais uma rodada? (1 - Sim / 0 - Nao): ");
        if (scanf("%d", &opc) != 1) {
            while (getchar() != '\n');
            opc = 0;
        }
    }
    free(listas);
    liberar_deck(d);
    liberar_mesa(&m);
    liberar_fila(&f);
    printf("\nNao faca isso em um cassino...\n");
    return 0;
}