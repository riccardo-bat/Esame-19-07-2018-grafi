#ifndef GRAFI_H_
    #define GRAFI_H_

    typedef struct adj_node {
      int node;
      float weight;
      struct adj_node* next;
    } adj_node;


    typedef adj_node* adj_list;

    typedef struct{
      adj_list* nodes;
      int dim;
    } graph;

    graph new_graph(int);
    void add_arc(graph*, int, int, float);
    void add_edge(graph* g, int, int, float);

    int get_dim(const graph* G);
    adj_list get_adjlist(const graph* G, int);
    int get_adjnode(adj_list);
    adj_list get_nextadj(adj_list);
    void free_graph(graph *G);

#endif /* GRAFI_H_ */
