#include <stdio.h>
#include <stdlib.h>

#define NV 5

typedef struct aresta {
    int dest;
    int custo;
    struct aresta *prox;
} *LAdj, *GrafoL [NV];

typedef int GrafoM[NV][NV];

// Imprime grafo a partir de uma Matriz de adjacência
void imprimeM (GrafoM g) {
    int o, d;

    for (o = 0; o < NV; o++) {
        printf("%d -> ", o);
        for (d = 0; d < NV; d++) {
            if (g[o][d] != NULL)
                printf ("%d", d);
        }
        printf ("\n");
    }
}

// Imprime grafo a partir de uma Lista de Adjacência
void imprimeL (GrafoL g) {
    int o;
    LAdj aux;

    for (o = 0; o < NV; o++) {
        printf("%d -> ", o);
        for (aux = g[o]; aux != NULL; aux = aux->prox) {
            printf("%d ", aux->dest);
        }
        printf("\n");
    }
}


// 1 - Representações

// Ex.1
// Complexidade = V_in^2
void fromMat (GrafoM in, GrafoL out) {
    int o, d;
    LAdj aux;

    for (o = 0; o < NV; o++) {
        out[o] = NULL;
    }

    for (o = 0; o < NV; o++) {
        for (d = 0; d < NV; d++) {
            if (in[o][d] != 0) {
                aux = malloc(sizeof(struct aresta));
                aux->dest = d;
                aux->custo = 1;
                aux->prox = out[o];
                out[o] = aux;
            }
        }
    }
}

// Ex.2
// Complexidade = V + A
void inverte (GrafoL in, GrafoL out) {
    int o;
    LAdj d, aux;

    for (o = 0; o < NV; o++)
        out[o] = NULL;

    for (o = 0; o < NV; o++) {
        for (d = in[o]; d != NULL; d = d->prox) {
            aux = malloc(sizeof(struct aresta));
            aux->dest = o;
            aux->custo = 1;
            aux->prox = out[d->dest];
            out[d->dest] = aux;
        }
    }
}

// Ex.3
// Complexidade = V + A
int inDegree (GrafoL g) {
    int grau[NV];
    int o, m = 0;
    LAdj d;

    for (o = 0; o < NV; o++) {
        for (d = g[o]; d != NULL; d = d->prox) {
            grau[d->dest]++;
        }
    }

    for (o = 0; o < NV; o++) {
        if (grau[o] > m)
            m = grau[o];
    }

    return m;
}


// Ex.4
// Complexidade = V + A
int colorOK (GrafoL g, int cor[]) {
    int o;
    LAdj d;

    for (o = 0; o < NV; o++) {
        for (d = g[o]; d != NULL; d = d->prox) {
            if (cor[o] == cor[d->dest])
                return 0;
        }
    }

    return 1;
}

// Ex.5
// Complexidade = V + A
int exists (GrafoL g, int from, int to) {
    LAdj l;

    for (l = g[from]; l != NULL; l = l->prox) {
        if (l->dest == to) {
            return 1;
        }
    }

  return 0;
}

int homomorfOK (GrafoL g, GrafoL h, int f[]) {
    int i;
    LAdj l;

    for (i = 0; i < NV; i++) {
        for (l = g[i]; l != NULL; l = l->prox){
            if (!exists(g, f[i], f[h[i]->dest]))
                return 0;
        }
    }

    return 1;
}


// 2 - Travessias

// Depth First Search
int DF (GrafoL g, int or, int v[], int p[], int l[]) {
    int i;

    for (i = 0; i < NV; i++) {
        v[i] = 0;
        p[i] = -1;
        l[i] = -1;
    }

    p[or] = -1;
    l[or] = 0;

    return DFRec(g, or, v, p, l);
}

int DFRec (GrafoL g, int or, int v[], int p[], int l[]) {
    int i;
    LAdj a;
    i = 1;
    v[or] = -1;

    for (a = g[or]; a != NULL; a = a->prox) {
        if (!v[a->dest]) {
            p[a->dest] = or;
            l[a->dest] = 1 + l[or];
            i += DFRec (g, a->dest, v, p, l);
        }
    }

    v[or] = 1;

    return i;
}


// Breadth First Search
int BF (GrafoL g, int or, int v[], int p[], int l[]) {
    int i, x;
    LAdj a;
    int q[NV], front, end;

    for (i = 0; i < NV; i++) {
        v[i] = 0;
        p[i] = -1;
        l[i] = -1;
    }

    front = end = 0;
    q[end++] = or; //enqueue
    v[or] = 1;
    p[or] = -1;
    l[or] = 0;
    i = 1;

    while (front != end) {
        x = q[front++]; //dequeue
        for (a = g[x]; a != NULL; a = a->prox) {
            if (!v[a->dest]) {
                i++;
                v[a->dest] = 1;
                p[a->dest] = x;
                l[a->dest] = 1 + l[x];
                q[end++] = a->dest; //enqueue
            }
        }
    }

    return i;
}


// Ex.1
int maisLonga (GrafoL g, int or, int p[]) {
    int vis[NV], pais[NV], dist[NV];
    int i, m, r;
    BF (g, or, vis, pais, dist);
    m = or;

    for (i = 0; i < NV; i++) {
        if (dist[i] > dist[m])
            m = i;
    }

    r = dist[m];
    i = r;

    while (pais[m] != -1) {
        p[i] = m;
        i--;
        m = pais[m];
    }

    return r;
}


// Ex.2
int componentes (GrafoL g, int c[]) {
    int i, j, next = 0;
    int vis[NV], pai[NV], dist[NV];

    for (i = 0; i < NV; i++)
        c[i] = 0;

    for (i = 0; next < NV; i++) {
        BF(g, next, vis, pai, dist);
        for (j = 0, next = NV; j < NV; j++) {
            if (vis[j])
                c[j] = i+1;
            else if (c[j] == 0 && j < next)
                next = j;
        }
    }

    return i;
}



int main () {
    /* Grafo Orientado

        0 - - x 1
        x     / |
        |    /  |
        |   /   |
        |  x    x
        2 - - x 4
        x
        |
        x
        3

    */
    GrafoM go = {{0,1,0,0,0},
                 {0,0,1,0,1},
                 {1,0,0,1,1},
                 {0,0,1,0,0},
                 {0,0,0,0,0}};


    /* Grafo não orientado

    0 x - x 1
    x    /
    |   /
    |  /
    x x       4 x - - x 3
    2

    */
    GrafoM gno = {{0,1,1,0,0},
                  {1,0,1,0,0},
                  {1,1,0,0,0},
                  {0,0,0,0,1},
                  {0,0,0,1,0}};


    /* Grafo Orientado e Acícilico

    0    4
    x   / |
    |  /  |
    | x   |
    2     1
    x    /
    |   /
    | x
    3

    */
    GrafoM goa = {{0,0,0,0,0},
                  {0,0,0,1,0},
                  {1,0,0,0,0},
                  {0,0,1,0,0},
                  {0,1,1,0,0}};


    GrafoL g, i;
    //int cor1[NV] = {0,1,2,0,0}, cor2[NV] = {0,1,1,0,0};
    //int cam[NV] = {0}, mi, d;

    // 1
    printf("Ex.1\n");
    fromMat(go,g);
    imprimeL(g);

/*
    // 2
    printf("\nEx.2\n");
    inverte(g,i);
    imprimeL(i);

    // 3
    printf("\nEx.3\n");
    printf("Indegree = %d\n", inDegree(g));

    // 4
    printf("\nEx.4\n");
    printf("Cor 1 -> %d\n", colorOK(g,cor1));
    printf("Cor 2 -> %d\n", colorOK(g,cor2));
*/
}
