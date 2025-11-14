#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define FILA_MAX 5
#define PILHA_MAX 3

/* Estrutura da peça */
typedef struct {
    char nome;  // I, O, T, L
    int id;     // identificador único
} Peca;

/* Fila circular */
typedef struct {
    Peca itens[FILA_MAX];
    int frente;
    int tras;
    int tamanho;
} Fila;

/* Pilha */
typedef struct {
    Peca itens[PILHA_MAX];
    int topo;
} Pilha;

/* ------------ Funções auxiliares ------------ */

/* Gera uma nova peça automaticamente */
Peca gerarPeca() {
    static int contadorID = 0;
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca p;
    p.nome = tipos[rand() % 4];
    p.id = contadorID++;
    return p;
}

/* ------------ Funções da fila ------------ */

void inicializarFila(Fila *f) {
    f->frente = 0;
    f->tras = 0;
    f->tamanho = 0;

    for (int i = 0; i < FILA_MAX; i++) {
        f->itens[i] = gerarPeca();
        f->tamanho++;
        f->tras = (f->tras + 1) % FILA_MAX;
    }
}

int filaVazia(Fila *f) {
    return f->tamanho == 0;
}

int filaCheia(Fila *f) {
    return f->tamanho == FILA_MAX;
}

Peca dequeue(Fila *f) {
    Peca removida = f->itens[f->frente];
    f->frente = (f->frente + 1) % FILA_MAX;
    f->tamanho--;
    return removida;
}

void enqueue(Fila *f, Peca p) {
    if (filaCheia(f)) return;

    f->itens[f->tras] = p;
    f->tras = (f->tras + 1) % FILA_MAX;
    f->tamanho++;
}

/* ------------ Funções da pilha ------------ */

void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

int pilhaVazia(Pilha *p) {
    return p->topo == -1;
}

int pilhaCheia(Pilha *p) {
    return p->topo == PILHA_MAX - 1;
}

void push(Pilha *p, Peca x) {
    if (!pilhaCheia(p)) {
        p->itens[++p->topo] = x;
    }
}

Peca pop(Pilha *p) {
    return p->itens[p->topo--];
}

/* ------------ Outras funções ------------ */

void exibirEstado(Fila *f, Pilha *p) {
    printf("\n=== ESTADO ATUAL ===\n");

    printf("Fila de peças\t");
    int idx = f->frente;
    for (int i = 0; i < f->tamanho; i++) {
        Peca pc = f->itens[idx];
        printf("[%c %d] ", pc.nome, pc.id);
        idx = (idx + 1) % FILA_MAX;
    }

    printf("\nPilha de reserva\t(Topo -> base): ");
    for (int i = p->topo; i >= 0; i--) {
        printf("[%c %d] ", p->itens[i].nome, p->itens[i].id);
    }

    printf("\n=====================\n");
}

/* Troca a peça da frente da fila com a do topo da pilha */
void trocaSimples(Fila *f, Pilha *p) {
    if (pilhaVazia(p)) {
        printf("A pilha está vazia! Não é possível trocar.\n");
        return;
    }

    int idx = f->frente;
    Peca temp = f->itens[idx];
    f->itens[idx] = p->itens[p->topo];
    p->itens[p->topo] = temp;

    printf("Troca simples realizada!\n");
}

/* Troca múltipla entre 3 peças da fila e 3 da pilha */
void trocaMultipla(Fila *f, Pilha *p) {
    if (p->topo < 2 || f->tamanho < 3) {
        printf("Não há peças suficientes para troca múltipla!\n");
        return;
    }

    int idx = f->frente;

    for (int i = 0; i < 3; i++) {
        Peca temp = f->itens[(idx + i) % FILA_MAX];
        f->itens[(idx + i) % FILA_MAX] = p->itens[p->topo - i];
        p->itens[p->topo - i] = temp;
    }

    printf("Troca múltipla realizada com sucesso!\n");
}

/* ------------ Menu principal ------------ */

int main() {
    srand(time(NULL));

    Fila fila;
    Pilha pilha;
    inicializarFila(&fila);
    inicializarPilha(&pilha);

    int opc;

    do {
        exibirEstado(&fila, &pilha);

        printf("\nOpções disponíveis:\n");
        printf("1 - Jogar peça da frente da fila\n");
        printf("2 - Enviar peça da fila para a pilha\n");
        printf("3 - Usar peça reservada da pilha\n");
        printf("4 - Trocar a peça da frente com o topo da pilha\n");
        printf("5 - Trocar as 3 primeiras da fila com as 3 da pilha\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opc);

        switch (opc) {

        case 1: { // Jogar peça
            if (!filaVazia(&fila)) {
                Peca usada = dequeue(&fila);
                printf("Peça [%c %d] jogada!\n", usada.nome, usada.id);

                Peca nova = gerarPeca();
                enqueue(&fila, nova);
            }
            break;
        }

        case 2: { // Reservar peça
            if (!pilhaCheia(&pilha) && !filaVazia(&fila)) {
                Peca retirada = dequeue(&fila);
                push(&pilha, retirada);
                printf("Peça [%c %d] enviada para reserva!\n",
                       retirada.nome, retirada.id);

                Peca nova = gerarPeca();
                enqueue(&fila, nova);
            } else {
                printf("Ação não permitida! Pilha cheia ou fila vazia.\n");
            }
            break;
        }

        case 3: { // Usar peça da pilha
            if (!pilhaVazia(&pilha)) {
                Peca usada = pop(&pilha);
                printf("Peça reservada [%c %d] usada!\n",
                       usada.nome, usada.id);
            } else {
                printf("Pilha vazia!\n");
            }
            break;
        }

        case 4: // Troca simples
            trocaSimples(&fila, &pilha);
            break;

        case 5: // Troca múltipla
            trocaMultipla(&fila, &pilha);
            break;

        case 0:
            printf("Encerrando o programa...\n");
            break;

        default:
            printf("Opção inválida!\n");
        }

    } while (opc != 0);

    return 0;
}
