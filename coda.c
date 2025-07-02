#include <stdlib.h>
#include "coda.h"

/* Crea un nuovo nodo */
static elem* new_elem(tipo_inf inf) {
    elem *p = malloc(sizeof *p);
    if (!p) exit(EXIT_FAILURE);
    p->inf = inf;
    p->pun = NULL;
    return p;
}

/* Inizializza e restituisce una coda vuota */
coda newQueue(void) {
    coda c = { .head = NULL, .tail = NULL };
    return c;
}

/* Restituisce 1 se vuota, 0 altrimenti */
int isEmpty(const coda *c) {
    return (c->head == NULL);
}

/* Inserisce in fondo alla coda */
void enqueue(coda *c, tipo_inf i) {
    elem *e = new_elem(i);
    if (c->tail) {
        c->tail->pun = e;
    }
    c->tail = e;
    if (!c->head) {
        c->head = e;
    }
}

/* Rimuove e restituisce l’elemento in testa */
tipo_inf dequeue(coda *c) {
    if (c->head == NULL) {
        /* Gestire errore a piacere; qui usiamo exit */
        exit(EXIT_FAILURE);
    }
    elem *tmp = c->head;
    tipo_inf ris = tmp->inf;
    c->head = tmp->pun;
    if (!c->head) {
        /* la coda è diventata vuota */
        c->tail = NULL;
    }
    free(tmp);
    return ris;
}

/* Restituisce l’elemento in testa senza rimuoverlo */
tipo_inf first(const coda *c) {
    if (c->head == NULL) {
        exit(EXIT_FAILURE);
    }
    return c->head->inf;
}
