#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PARENT(i) (i-1)/2
#define LEFT(i) 2*i + 1
#define RIGHT(i) 2*i + 2

#define Max 100

typedef struct pQueue {
    int valores [Max];
    int tamanho;
} PriorityQueue;

#define Size 10

typedef struct nodo {
    char *chave;
    int ocorr;
    struct nodo *prox;
} Nodo, *THash1 [Size];

#define Free 0
#define Used 1
#define Del 2

typedef struct bucket {
    int status; // Free | Used | Del
    char *chave;
    int ocorr;
} THash2 [Size];



// Parte 1 - Min-Heaps

// Ex.1
/*  a) Esquerda -> 2*i + 1
    b) Direita -> 2*i + 2
    c) Pai -> (i-1)/2
*/

void swap (int heap[], int a, int b) {
    int aux;

    aux = heap[a];
    heap[a] = heap[b];
    heap[b] = aux;
}

// Ex.2
void bubbleUp (int i, int h[]) {
    int pai = (i-1)/2;

    while (i > 0 && h[i] < h[pai]) {
        swap(h,i,pai);
        i = pai;
        pai = (i-1)/2;
    }
}

// Ex.3
void bubbleDown (int i, int h[], int N) {
    int menor;

    while (2*i+1 < N) {
        menor = 2*i+1;

        if (2*i+2 < N && h[2*i+2] < h[menor])
            menor = 2*i+2;

        if (h[menor] < h[i])
            swap(h, menor, i);
        else
            break;

        i = menor;
    }
}

// Ex.4
void empty (PriorityQueue *q) {
    q->tamanho = 0;
}

int isEmpty (PriorityQueue *q) {
    return q->tamanho == 0;
}

int addQ (int x, PriorityQueue *q) {
    int r = -1;
    if(q->tamanho != Max) {
        r = 0;
        q->valores[q->tamanho] = x;
        bubbleUp(q->tamanho, q->valores);
        q->tamanho++;
    }

    return r;
}

int removeQ (PriorityQueue *q, int *rem) {
    int r = -1;
    if (!isEmpty(q)) {
        r = 0;
        q->tamanho--;
        *rem = q->valores[0];
        swap(q->valores, 0, q->tamanho);
        bubbleDown(0, q->valores, q->tamanho);
    }

    return r;
}



// Parte 2 - Tabelas de Hash
unsigned hash(char *str){
    unsigned hash = 5381;
    int c;
    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    return hash;
}


// 2.1 - Chaining

// Ex.1
void initEmpty1 (THash1 t) {
    int i;

    for (i = 0; i < Size; i++)
        t[i] = NULL;
}

void printHashTable(THash1 t){
	int i = 0;
	Nodo *aux;

	for(i=0; i<Size; i+=1){
		aux = t[i];
		printf("Nivel %d ---> ",i);
		while(aux != NULL) {
			printf(aux->chave);
			printf(" ->");
			aux=aux->prox;
		}
		printf("\n");
	}
}

// Ex.2
void add1 (char *s, THash1 t) {
    int pos = hash(s) % Size;
    Nodo *aux, *anterior;
    Nodo *novo = malloc(sizeof(Nodo));
    novo->chave = strdup(s);
    novo->ocorr = 1;
    novo->prox = NULL;

    if (t[pos] != NULL) {
        for (aux = t[pos]; aux != NULL && !strcmp(aux->chave,s); aux = aux->prox)
            anterior = aux;

        if (aux == NULL)
            anterior->prox = novo;

        else
            aux->ocorr++;
    }
    else
        t[pos] = novo;
}

// Ex.3
int lookUp1 (char *s, THash1 t) {
    int r = 0;
    int pos = hash(s) % Size;
    Nodo *aux;

    for (aux = t[pos]; aux != NULL && !strcmp(aux->chave,s); aux = aux->prox);

    if (aux != NULL)
        r = aux->ocorr;

    return r;
}

// Ex.4
int remove1 (char *s, THash1 t) {
    int r = 0;
    int pos = hash(s) % Size;
    Nodo *aux, *anterior;

    for (aux = t[pos]; aux != NULL && !strcmp(aux->chave,s); aux = aux->prox)
        anterior = aux;

    if (aux != NULL) {
        r = 1;
        aux->ocorr--;
        if (aux->ocorr == 0) {
            if (aux->prox == NULL) {
                anterior->prox = NULL;
                free(aux);
            }
            else {
                anterior->prox = aux->prox;
                free(aux);
            }
        }
    }

    return r;
}


// 2.2 - Open Addressing

// Ex.1
int where (char *s, THash2 t) {
    int hash_n = hash(s);
    int i, r = 0, del = 0;

    for (i = 0; t[(hash_n + i) % Size].status != Free && !strcmp(t[(hash_n + i) % Size].chave,s) && i < Size; i++) {
        if (t[(hash_n + i) % Size].status == Del && !del) {
            del++;
            r = (hash_n + i) % Size;
        }
    }

    if (i == Size && !del)
        r = -1;

    else if (t[(hash_n + i) % Size].status == Free && !del || t[(hash_n + i) % Size].status == Used)
        r = (hash_n + i) % Size;

    return r;
}

// Ex.2
// a)
void initEmpty2 (THash2 t) {
    int i;

    for (i = 0; i < Size; i++) {
        t[i].status = Free;
        t[i].ocorr = 0;
    }
}

// b)
void add2 (char *s, THash2 t) {
    int pos = where(s,t);

    if (t[pos].status == Used)
        t[pos].ocorr++;

    else {
        t[pos].chave = strdup(s);
        t[pos].ocorr = 1;
        t[pos].status = Used;
    }
}

// c)
int lookup (char *s, THash2 t) {
    int pos = where(s,t);
    int r = 0;

    if (t[pos].status == Used)
        r = t[pos].ocorr;

    return r;
}

// d)
int remove (char *s, THash2 t) {
    int pos = where(s,t);
    int r = 0;

    if (t[pos].status = Used) {
        r = 1;
        t[pos].ocorr--;
        if (t[pos].ocorr == 0) {
            t[pos].status == Del;
            free(t[pos].chave);
        }
    }

    return r;
}

// Ex. 3
int garb_collection (THash2 t) {
    int i, r = 0;

    for (i = 0; i < Size; i++) {
        if (t[i].status == Del) {
            r++;
            t[i].status = Free;
            t[i].ocorr = 0;
            free(t[i].chave);
        }
        else if (t[i].status == Used) {
            int pos = where(t[i].chave,t);

            if (t[pos].status == Del) {
                t[pos].status = Free;
                r++;
            }

            struct bucket aux;
            aux = t[i];
            t[i] = t[pos];
            t[pos] = aux;
        }
    }

    return r;
}
