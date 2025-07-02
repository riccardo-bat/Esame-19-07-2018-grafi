// grafi.c

#include <stdio.h>
#include <stdlib.h>
#include "grafi.h"

/* Crea un grafo con n vertici e liste di adiacenza vuote */
graph new_graph(int n) {
    graph G;
    G.dim = n;
    G.nodes = malloc(n * sizeof *G.nodes);
    if (!G.nodes) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < n; i++) {
        G.nodes[i] = NULL;
    }
    return G;
}

/* Aggiunge un arco orientato u->v con peso w (u, v 1-based) */
void add_arc(graph *G, int u, int v, float w) {
    if (!G || u < 1 || u > G->dim || v < 1 || v > G->dim) return;
    adj_node *t = malloc(sizeof *t);
    if (!t) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    t->node   = v - 1;           /* memorizziamo 0-based */
    t->weight = w;
    t->next   = G->nodes[u - 1];
    G->nodes[u - 1] = t;
}

/* Aggiunge un arco non orientato (due archi orientati) */
void add_edge(graph *G, int u, int v, float w) {
    add_arc(G, u, v, w);
    add_arc(G, v, u, w);
}

/* Restituisce il numero di vertici */
int get_dim(const graph *G) {
    return G ? G->dim : 0;
}

/* Restituisce la lista di adiacenza del vertice u (1-based) */
adj_list get_adjlist(const graph *G, int u) {
    if (!G || u < 1 || u > G->dim) return NULL;
    return G->nodes[u - 1];
}

/* Restituisce l’indice 1-based del nodo in l */
int get_adjnode(const adj_list l) {
    return l ? (l->node + 1) : -1;
}

/* Restituisce il puntatore al prossimo arco */
adj_list get_nextadj(const adj_list l) {
    return l ? l->next : NULL;
}

/* Libera tutte le liste di adiacenza e l’array di puntatori */
void free_graph(graph *G) {
    if (!G) return;
    for (int i = 0; i < G->dim; i++) {
        adj_node *p = G->nodes[i];
        while (p) {
            adj_node *next = p->next;
            free(p);
            p = next;
        }
    }
    free(G->nodes);
    G->nodes = NULL;
    G->dim   = 0;
}
