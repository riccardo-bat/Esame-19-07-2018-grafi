#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "node.h"
#include "grafi.h"
#include "coda.h"

//prototipi 
graph create_graph_file(char* filename);
void stampa(graph grafo, struct node* arrayNodes);
struct node* getNodes(char* filename, int* dim_arrayNodes);
void follow(graph grafo_sn, struct node* arrayNodes, int source_id);

int main(){
    //PUNTO 1
    printf("\n--------------------------------------\n");
    graph grafo_sn = create_graph_file("graph.txt");
    //stampa(grafo_sn); VECCHIO FORMATO


    //PUNTO 2
    printf("\n--------------------------------------\n");
    int dim_arrayNodes = 0;

    //il nodo in pos 0 di arrayNodes corrisponde al nodo 1 del grafo
    struct node* arrayNodes = getNodes("node.txt", &dim_arrayNodes);
    stampa(grafo_sn, arrayNodes);


    //PUNTO 3b
    printf("\n--------------------------------------\n");
    follow(grafo_sn, arrayNodes, 8);



    //libero la memoria allocata
    free_graph(&grafo_sn);
    free(arrayNodes);

    printf("\n\n");
    return 0;
}

/**
 * @brief Crea un oggetto di tipo grafo leggendo un file passato come parametro
 * 
 * @param filename 
 * @return graph 
 */

graph create_graph_file(char* filename){
    char* mode = "r"; //lettura di un file txt

    //apertura del file
    FILE* fp_grafo = fopen(filename, mode);
    if(fp_grafo == NULL){printf("\n\nERRORE NELL'APERTURA DEL FILE"); exit(EXIT_FAILURE);} 


    //creazione del grafo 
    const int maxCarPerRiga = 100;
    char buffer[maxCarPerRiga]; 
    bool readingFirstLine = true;
    graph grafo; //oggetto grafo da restituire

    //variabili utili per la lettura del file
    char* endptr = NULL;
    int base = 10; 
    int peso_arco = 1; //il grafo non è pesato

    //leggo il file
    while(fgets(buffer, maxCarPerRiga+1, fp_grafo) != NULL){
        //elimino il '\n' letto
        buffer[strcspn(buffer, "\n")] = '\0';

        //se sto leggendo la prima riga
        if(readingFirstLine){
            //sto leggendo il numero totale di nodi nel grafo
            int totNodiGrafo = (int) strtol(buffer, &endptr, base); 
            if(endptr == buffer){printf("\n\nERRORE NELLA CONVERSIONE"); exit(EXIT_FAILURE);} 

            grafo = new_graph(totNodiGrafo);
        } else{
            //ogni riga, che non sia la prima, indica un arco da nodo sorgente -> dest
            //nodo sorgente nodo dest
            int source_node = 0, dest_node = 0; 
            sscanf(buffer, "%d %d", &source_node, &dest_node);

            //printf("\nSource node letto: %d", source_node);
            //printf("\nDestination node letto: %d", dest_node);

            //dest_node diventa vicino di source_node
            add_arc(&grafo, source_node, dest_node, peso_arco);
        }

        readingFirstLine = false; //d'ora in poi non sto più leggendo la prima riga
    }


    //chiusura file
    if(fclose(fp_grafo) == EOF){printf("\n\nERRORE NELLA CHIUSURA DEL FILE"); exit(EXIT_FAILURE);}
    return grafo;
}

/**
 * @brief Procedura per la stampa dei vicino di ogni nodo
 * 
 * @param grafo 
 */
void stampa(graph grafo, struct node* arrayNodes){
    //per ogni nodo stampo i suoi vicini 
    printf("\n\nSTAMPA DEL GRAFO IN FORMATO VERBOSE\n");

    //i nodi sono 1-based, ed ogni nodo ha il proprio id in pos id-1 nell'array arrayNodes
    //scorro l'array dei ptr del grafo
    for(int i=0; i<get_dim(&grafo); i++){
        /*printf("\nNodo %d (lista dei vicini) --> ", i+1);

        //scorro la lista di adiancenza del nodo i+1 per vedere i suoi vicini 
        adj_list cursor = get_adjlist(&grafo, i+1);
        while(cursor != NULL){
            printf("\t%d", (cursor->node+1) );
            cursor = cursor->next;
        }*/
        //i 0-based 
        struct node source_node = arrayNodes[i];

        //scorro la lista dei vicini del nodo sorgente
        adj_list cursor = get_adjlist(&grafo, i+1);
        while(cursor != NULL){
            //cursor->node 0-based
            struct node dest_node = arrayNodes[cursor->node];

            //in base alla tipologia dei due nodi stabilisco la relazione
            if(source_node.tipo == 'U' && dest_node.tipo == 'U')
                printf("\n%s FOLLOW %s", source_node.cont, dest_node.cont);
            else if(source_node.tipo == 'T' && dest_node.tipo == 'U')
                printf("\n%s OWNER %s", source_node.cont, dest_node.cont);
            else if(source_node.tipo == 'U' && dest_node.tipo == 'T')
                printf("\n%s LIKE %s", source_node.cont, dest_node.cont);
            else {printf("\n\nUNEXPECTED ERROR"); exit(EXIT_FAILURE);}


            cursor = cursor->next;
        }
    }
}

/**
 * @brief Get the Nodes object
 * 
 * @param filename 
 * @return struct node 
 */
struct node* getNodes(char* filename, int *dim_arrayNodes){
    char* mode = "r";

    FILE* fp_nodes = fopen(filename, mode);
    if(fp_nodes == NULL){printf("\n\nERRORE NELL'APERTURA DEL FILE"); exit(EXIT_FAILURE);} 

    struct node* arrayNodes; //array dinamico per contenere i nodi letti
    int dim = 0;

    //ogni nodo prende due righe; nella prima il contenuto, nella seconda il tipo
    int righe_per_nodo = 2; 
    int riga_attuale = 1; //riga attuale per ogni nodo
    int maxCarPerRiga = 100; 
    char buffer[maxCarPerRiga];
    struct node tmp; //oggetto temporaneo per la lettura

    

    while(fgets(buffer, maxCarPerRiga+1, fp_nodes) != NULL){
        buffer[strcspn(buffer, "\n")] = '\0';

        //in base alla riga_attuale che sto leggendo acquisisco un'informazione diversa
        if(riga_attuale == 1){
            //sto leggendo il contenuto 
            strcpy(tmp.cont, buffer);
            tmp.cont[sizeof tmp.cont - 1] = '\0';
        } else if(riga_attuale == 2){
            //sto leggendo il tipo 
            tmp.tipo = buffer[0];
        }

        //se ho finito di leggere un intero nodo
        if(riga_attuale == righe_per_nodo){
            //salvo quel nodo nell'array
            dim = dim + 1; //aumento la dimensione che l'array avrà
            if(dim == 1) arrayNodes = malloc(dim * sizeof(struct node));
            else arrayNodes = realloc(arrayNodes, dim * sizeof(struct node));
            arrayNodes[dim-1] = tmp; 

            //printf("\nNodo appena inserito: %s di tipo %c", tmp.cont, tmp.tipo);

            //ripristino i valori di tmp 
            tmp.cont[0] = '\0';
            tmp.tipo = ' '; 

            riga_attuale = 1;
        } else 
            riga_attuale++;

    }

    //chiusura file
    if(fclose(fp_nodes) == EOF){printf("\n\nERRORE NELLA CHIUSURA DEL FILE"); exit(EXIT_FAILURE);}
    *dim_arrayNodes = dim;
    return arrayNodes;
}

/**
 * @brief Stampo la lista dei nodi che source_id segue direttamente o indirettamente
 * source_id 1-based
 * 
 * @param grafo_sn 
 * @param arrayNodes 
 * @param source_id 
 */
void follow(graph grafo_sn, struct node* arrayNodes, int source_id){
    //sfrutta la BFS sui grafi
    bool* visited = malloc(get_dim(&grafo_sn) * sizeof(bool));
    for(int i=0; i<get_dim(&grafo_sn); i++) visited[i] = false;

    coda queue = newQueue(); //inserisco i nodi che sono ancora da visitare
    //nodi coda - 0 based
    enqueue(&queue, source_id-1);
    visited[source_id-1] = true;

    printf("\nL'utente %s segue: ", arrayNodes[source_id-1].cont);

    while(!isEmpty(&queue)){
        //estraggo il nodo in testa
        tipo_inf extract = dequeue(&queue); //extract è 0-based
        //se il nodo estratto non è il nodo sorgente ed e' un utente... 
        if(extract != (source_id-1) && arrayNodes[extract].tipo == 'U')
            printf("\n\t- %s", arrayNodes[extract].cont);

        //considero tutti i vicini del nodo
        adj_list cursor = get_adjlist(&grafo_sn, extract+1);
        while(cursor != NULL){
            //se quel nodo UTENTE non è ancora entrato in coda... 
            if(!visited[cursor->node] && arrayNodes[cursor->node].tipo == 'U'){
                enqueue(&queue, cursor->node);
                visited[cursor->node] = true;
            }

            cursor = cursor->next;
        }

    }
}