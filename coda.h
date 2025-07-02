#ifndef CODA_H
       #define CODA_H

       typedef int tipo_inf;

       /* Nodo della coda */
       typedef struct elem {
              tipo_inf        inf;
              struct elem    *pun;
       } elem;

       /* Struttura coda con puntatori a testa e coda */
       typedef struct {
              elem *head;
              elem *tail;
       } coda;

       /* Crea una coda vuota */
       coda    newQueue(void);

       /* Restituisce true se la coda è vuota */
       int     isEmpty(const coda *c);

       /* Inserisce un elemento in coda */
       void    enqueue(coda *c, tipo_inf i);

       /* Estrae e restituisce l’elemento in testa; la coda viene aggiornata */
       tipo_inf dequeue(coda *c);

       /* Restituisce senza rimuovere l’elemento in testa */
       tipo_inf first(const coda *c);

#endif 
