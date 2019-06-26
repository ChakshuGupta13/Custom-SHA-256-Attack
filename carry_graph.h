#include <utility>

#ifndef CARRY_GRAPH_H
#define CARRY_GRAPH_H

#include "difference_propagation_tables.h"

class CARRY_GRAPH {
    map<CARRY_GRAPH_NODE, int> node_to_index;
    map<int, CARRY_GRAPH_NODE> index_to_node;
    vector<vector<int>> adjList;

    vector<DEL> ADDENDS;
public:
    DEL compressed;

    void _init_carry_graph(vector<DEL>, int num_bits = WORD_LENGTH);

    void COMPUTE_GRAPH();

    void CLEAR_GRAPH() {
        ADDENDS.clear();
        compressed = "";
    }
};

#endif //CARRY_GRAPH_H