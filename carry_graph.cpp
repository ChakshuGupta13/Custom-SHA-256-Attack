#include <utility>

#include <utility>

#include "carry_graph.h"

void CARRY_GRAPH::_init_carry_graph(vector<DEL> addends, const int num_bits) {
    ADDENDS = std::move(addends);
    int max_carry = (ADDENDS.size() - 1);

    compressed = "";
    for (int bit = 0; bit < num_bits; ++bit) compressed += "?";

    int index = 0;
    for (int bit = 0; bit < num_bits; bit++)
        for (int carry_1 = 0; carry_1 <= max_carry; carry_1++)
            for (int carry_2 = 0; carry_2 <= max_carry; carry_2++) {
                auto node = make_pair(bit, make_pair(carry_1, carry_2));

                node_to_index[node] = index;
                index_to_node[index] = node;

                adjList.emplace_back();

                index++;
            }
}

void CARRY_GRAPH::COMPUTE_GRAPH() {
    queue<int> node_index_queue;
    set<int> visited_nodes_set;
    vector<set<pair<bool, bool>>> allow_bit_config(WORD_LENGTH);

    auto start_node = make_pair(0, make_pair(0, 0));

    node_index_queue.push(node_to_index[start_node]);
    visited_nodes_set.insert(node_to_index[start_node]);

    while (!node_index_queue.empty()) {
        auto popped_node = index_to_node[node_index_queue.front()];
        node_index_queue.pop();

        int index = popped_node.first;

        vector<vector<pair<bool, bool>>> addends_config;
        for (auto addend: ADDENDS)
            addends_config.push_back(index_to_config(diff_bit_render_to_index[addend[index]]));

        vector<int> indices (addends_config.size());
        int addend_index = 0;
        bool looping_done = false;
        while (!looping_done) {
            {
                int msg_1_A_i = popped_node.second.first;
                int msg_2_A_i = popped_node.second.second;

                for (int config_index = 0; config_index < addends_config.size(); config_index++) {
                    auto config = addends_config[config_index][indices[config_index]];
                    msg_1_A_i += config.first;
                    msg_2_A_i += config.second;
                }

                int msg_1_A_i_carry_out = msg_1_A_i / 2;
                int msg_2_A_i_carry_out = msg_2_A_i / 2;

                int msg_1_A_i_bit = msg_1_A_i % 2;
                int msg_2_A_i_bit = msg_2_A_i % 2;

                allow_bit_config[index].insert(make_pair(msg_1_A_i_bit, msg_2_A_i_bit));
                auto next_node_index = node_to_index[make_pair(index + 1, make_pair(msg_1_A_i_carry_out, msg_2_A_i_carry_out))];

                adjList[node_to_index[popped_node]].push_back(next_node_index);

                if (visited_nodes_set.count(next_node_index) == 0 && index + 1 < WORD_LENGTH) {
                    node_index_queue.push(next_node_index);
                    visited_nodes_set.insert(next_node_index);
                }
            }

            indices[addend_index]++;

            while (indices[addend_index] == addends_config[addend_index].size()) {
                if (addend_index == (addends_config.size() - 1)) {
                    looping_done = true;
                    break;
                }
                indices[addend_index++] = 0;
                indices[addend_index]++;
            }

            addend_index = 0;
        }
    }

    for (int bit = 0; bit < WORD_LENGTH; bit++) compressed[bit] = diff_bit_render_2[config_to_index(allow_bit_config[bit])];
}