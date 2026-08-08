# Terminal21

Jogo de blackjack (21) para terminal, feito em C. Foi feito como Trabalho
Final de Estrutura de Dados I. Dá pra jogar com mais de um jogador na
mesma mesa, cada um com suas fichas, e o jogo aceita split de mão e
mostra um log dos eventos no fim de cada rodada.

## Requisitos

Precisa de um compilador C (usei GCC pelo MSYS2 no Windows, mas qualquer
GCC recente deve funcionar). O terminal também precisa aceitar cores
ANSI pra mostrar as cartas coloridas certinho — funciona bem no
PowerShell atual e no Windows Terminal. Se estiver no cmd antigo, as
cores podem não aparecer direito, mas o jogo roda igual, só sem cor.

## Compilando

Se tiver o make instalado, é só:
```
mingw32-make
```
Isso já gera o `blackjack.exe`.

Se não tiver make, dá pra compilar direto:
```
gcc -Wall -Wextra -o blackjack cartas.c mao.c deck.c turno.c eventos.c main.c
```

## Rodando

```
.\blackjack.exe
```
(no Linux/Mac seria `./blackjack`)

## Como jogar

Primeiro o jogo pede quantos jogadores vão sentar na mesa. Depois, a
cada rodada, cada um aposta um valor (não pode passar das fichas que
tem) e recebe duas cartas.

Na sua vez, dá pra escolher:
- pedir carta (hit)
- parar com o total que tem (stand)
- dar split, se as duas cartas forem iguais — separa em duas mãos e
  você joga elas separadamente
- trocar entre as mãos, depois que deu split

Quando todo mundo já jogou, o dealer compra carta até chegar em 17 (ou
mais) e o jogo compara cada mão com o total do dealer pra saber quem
ganha, perde ou empata. No fim, aparece um resumo dos eventos da
rodada e pergunta se quer jogar de novo.

## O que deixei de fora

O dealer sempre compra até 17, sem considerar a regra do "soft 17"
(quando o 17 vem com um Ás valendo 11) — algumas casas fariam ele
comprar mais uma carta nesse caso, mas aqui não tem essa distinção.
Também não coloquei limite de quantas vezes dá pra dar split na mesma
rodada.

## Autor

Miguel Ranieri Godoi Ferreira
