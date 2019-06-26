#include <random>

#include "diff_chars.h"

bool CONSISTENT(const DEL &in) {
    for (char bit : in)
        if (bit == '#') return false;

    return true;
}

void print_SP(const int &step, const SEARCH_SPACE &SP, ostream &out) {
    out << "\033[2J\033[1;1H";

    DEL temp;
    for (int index = 0; index < WORDS_IN_SEARCH_SPACE; index++) {
        temp = SP.W[index];
        rev(temp);
        out << temp;
        if (index == step) out << " <<";
        out << endl;
    }
}

struct search_checkpoint {
    SEARCH_SPACE SP;
    int index_changed{};
    int choice{};
};

DEL DASH_TEMPLATE;
CARRY_GRAPH G;
DEL A_7;
DEL E_7;
DEL E_8;
DEL A_8;
DEL E_9;
DEL A_9;
DEL E_10;
DEL E_11;
DEL E_12;
DEL E_13;

bool free(const DEL &word) {
    for (char c: word)
        if (c == '?')
            return true;

    return false;
}

int FIND_FIRST_FREE_RANDOM_INDEX(const DEL &in) {
    vector<int> free_bit_indices;
    for (int index = 0; index < WORD_LENGTH; index++)
        if (in[index] == '?')
            free_bit_indices.push_back(index);
    shuffle(free_bit_indices.begin(), free_bit_indices.end(), std::mt19937(std::random_device()()));

    return free_bit_indices[0];
}

void CHANGE_1(DEL &in, const int &index) {
    in[index] = '-';
}

void CHANGE_2(DEL &in, const int &index) {
    in[index] = 'x';
}

bool TEST_W_15(const SEARCH_SPACE &SP) {
    DEL TEST_INPUT_1, TEST_INPUT_2;

    G._init_carry_graph(vector<DEL>{GET_DEL_IF__WORD(DASH_TEMPLATE, E_13, E_12), E_11, SP.W[3], DASH_TEMPLATE});
    G.COMPUTE_GRAPH();
    TEST_INPUT_1 = GET_TIGHTEN_WORD(G.compressed, DASH_TEMPLATE);
    G.CLEAR_GRAPH();

    G._init_carry_graph(vector<DEL>{GET_DEL_SMALL_SIGMA_1(SP.W[3]), E_13, DASH_TEMPLATE});
    G.COMPUTE_GRAPH();
    TEST_INPUT_2 = GET_TIGHTEN_WORD(G.compressed, DASH_TEMPLATE);
    G.CLEAR_GRAPH();

    return (CONSISTENT(TEST_INPUT_1) && CONSISTENT(TEST_INPUT_2));
}

bool TEST_W_12(const SEARCH_SPACE &SP) {
    DEL TEST_INPUT_1, TEST_INPUT_2, TEST_INPUT_3, TEST_INPUT_4;

    G._init_carry_graph(
            vector<DEL>{
                    GET_DEL_MAJ_WORD(DASH_TEMPLATE, DASH_TEMPLATE, A_9),
                    DASH_TEMPLATE,
                    GET_DEL_SIGMA_1(E_11),
                    GET_DEL_IF__WORD(E_11, E_10, E_9),
                    E_8,
                    SP.W[2]
            });
    G.COMPUTE_GRAPH();
    TEST_INPUT_1 = GET_TIGHTEN_WORD(G.compressed, DASH_TEMPLATE);
    G.CLEAR_GRAPH();

    G._init_carry_graph(vector<DEL>{GET_DEL_SIGMA_1(E_11), GET_DEL_IF__WORD(E_11, E_10, E_9), E_8, SP.W[2], _K[12], A_8});
    G.COMPUTE_GRAPH();
    E_12 = G.compressed;
    G.CLEAR_GRAPH();

    G._init_carry_graph(vector<DEL>{GET_DEL_SIGMA_1(E_12), GET_DEL_IF__WORD(E_12, E_11, E_10), E_9, DASH_TEMPLATE});
    G.COMPUTE_GRAPH();
    TEST_INPUT_2 = GET_TIGHTEN_WORD(G.compressed, DASH_TEMPLATE);
    G.CLEAR_GRAPH();

    G._init_carry_graph(vector<DEL>{GET_DEL_SIGMA_1(E_13), GET_DEL_IF__WORD(E_13, E_12, E_11), E_10, DASH_TEMPLATE});
    G.COMPUTE_GRAPH();
    TEST_INPUT_3 = GET_TIGHTEN_WORD(G.compressed, DASH_TEMPLATE);
    G.CLEAR_GRAPH();

    G._init_carry_graph(vector<DEL>{GET_DEL_IF__WORD(DASH_TEMPLATE, DASH_TEMPLATE, E_13), E_12, DASH_TEMPLATE});
    G.COMPUTE_GRAPH();
    TEST_INPUT_4 = GET_TIGHTEN_WORD(G.compressed, DASH_TEMPLATE);
    G.CLEAR_GRAPH();

    return CONSISTENT(TEST_INPUT_1) && CONSISTENT(TEST_INPUT_2) && CONSISTENT(TEST_INPUT_3) && CONSISTENT(TEST_INPUT_4);
}

bool TEST_W_7_AND_W_8(const SEARCH_SPACE &SP) {
    DEL TEST_INPUT_1, TEST_INPUT_2;

    G._init_carry_graph(vector<DEL>{SP.W[0], _K[7], DASH_TEMPLATE});
    G.COMPUTE_GRAPH();
    A_7 = G.compressed;
    E_7 = G.compressed;
    G.CLEAR_GRAPH();

    G._init_carry_graph(vector<DEL>{GET_DEL_SIGMA_1(E_7), SP.W[1], DASH_TEMPLATE});
    G.COMPUTE_GRAPH();
    E_8 = G.compressed;
    G.CLEAR_GRAPH();

    G._init_carry_graph(vector<DEL>{GET_DEL_SIGMA_0(A_7), E_8, GET_DEL_MAJ_WORD(A_7, DASH_TEMPLATE, DASH_TEMPLATE)});
    G.COMPUTE_GRAPH();
    A_8 = G.compressed;
    G.CLEAR_GRAPH();

    G._init_carry_graph(vector<DEL>{GET_DEL_SIGMA_1(E_8), GET_DEL_IF__WORD(E_8, E_7, DASH_TEMPLATE), DASH_TEMPLATE});
    G.COMPUTE_GRAPH();
    E_9 = G.compressed;
    G.CLEAR_GRAPH();

    G._init_carry_graph(vector<DEL>{GET_DEL_SIGMA_0(A_8), GET_DEL_MAJ_WORD(A_8, A_7, DASH_TEMPLATE), E_9});
    G.COMPUTE_GRAPH();
    A_9 = G.compressed;
    G.CLEAR_GRAPH();

    G._init_carry_graph(vector<DEL>{A_9, DASH_TEMPLATE});
    G.COMPUTE_GRAPH();
    E_13 = G.compressed;
    G.CLEAR_GRAPH();

    G._init_carry_graph(vector<DEL>{GET_DEL_SIGMA_1(E_9), GET_DEL_IF__WORD(E_9, E_8, E_7), DASH_TEMPLATE});
    G.COMPUTE_GRAPH();
    E_10 = G.compressed;
    G.CLEAR_GRAPH();

    G._init_carry_graph(vector<DEL>{GET_DEL_SIGMA_0(A_9), GET_DEL_MAJ_WORD(A_9, A_8, A_7), E_10});
    G.COMPUTE_GRAPH();
    TEST_INPUT_1 = GET_TIGHTEN_WORD(G.compressed, DASH_TEMPLATE);
    G.CLEAR_GRAPH();

    G._init_carry_graph(vector<DEL>{GET_DEL_MAJ_WORD(DASH_TEMPLATE, A_9, A_8), DASH_TEMPLATE, GET_DEL_SIGMA_1(E_10), GET_DEL_IF__WORD(E_10, E_9, E_8), E_7});
    G.COMPUTE_GRAPH();
    TEST_INPUT_2 = GET_TIGHTEN_WORD(G.compressed, DASH_TEMPLATE);
    G.CLEAR_GRAPH();

    G._init_carry_graph(vector<DEL>{GET_DEL_SIGMA_1(E_10), GET_DEL_IF__WORD(E_10, E_9, E_8), E_7, DASH_TEMPLATE, A_7});
    G.COMPUTE_GRAPH();
    E_11 = G.compressed;
    G.CLEAR_GRAPH();

    return CONSISTENT(TEST_INPUT_1) && CONSISTENT(TEST_INPUT_2);
}

bool FIND(const int &step, SEARCH_SPACE &SP, bool (*TEST_FUNCTION)(const SEARCH_SPACE &), stack<search_checkpoint> &HISTORY) {
    search_checkpoint checkpoint;
    checkpoint.SP = SP;
    checkpoint.index_changed = WORD_LENGTH;
    checkpoint.choice = 0;

    stack<search_checkpoint> search_history;
    if (HISTORY.empty()) search_history.push(checkpoint);
    else search_history = HISTORY;

    if (free(SP.W[step])) {
        while (free(SP.W[step])) {

            checkpoint.index_changed = FIND_FIRST_FREE_RANDOM_INDEX(SP.W[step]);
            CHANGE_1(SP.W[step], checkpoint.index_changed);
            checkpoint.choice = 1;

            if (TEST_FUNCTION(SP)) {
                checkpoint.SP = SP;
                search_history.push(checkpoint);
                print_SP(step, SP);
                continue;
            }

            CHANGE_2(SP.W[step], checkpoint.index_changed);
            checkpoint.choice = 2;

            if (TEST_FUNCTION(SP)) {
                checkpoint.SP = SP;
                search_history.push(checkpoint);
                print_SP(step, SP);
                continue;
            }

            if (search_history.top().index_changed == WORD_LENGTH) return false;
            else
                do {
                    while (search_history.top().choice == 2) {
                        search_history.pop();
                        if (search_history.top().index_changed == WORD_LENGTH) return false;
                    }

                    checkpoint = search_history.top();
                    search_history.pop();

                    CHANGE_2(checkpoint.SP.W[step], checkpoint.index_changed);
                    checkpoint.choice = 2;

                    search_history.push(checkpoint);

                    print_SP(step, checkpoint.SP);

                } while (!TEST_FUNCTION(checkpoint.SP));
        }
    } else {
        do {
            while (search_history.top().choice == 2) {
                search_history.pop();
                if (search_history.top().index_changed == WORD_LENGTH) return false;
            }

            checkpoint = search_history.top();
            search_history.pop();

            CHANGE_2(checkpoint.SP.W[step], checkpoint.index_changed);
            checkpoint.choice = 2;

            search_history.push(checkpoint);

            print_SP(step, checkpoint.SP);

        } while (!TEST_FUNCTION(checkpoint.SP));

        SP = checkpoint.SP;
    }

    HISTORY = search_history;

    return true;
}

bool search_SP(const SEARCH_SPACE &DC, SEARCH_SPACE &final_DC) {
    for (int index = 0; index < WORD_LENGTH; index++) DASH_TEMPLATE += "-";

    SEARCH_SPACE virtual_SP = DC;

    stack<search_checkpoint> W_7;
    stack<search_checkpoint> W_8;
    stack<search_checkpoint> W_12;
    stack<search_checkpoint> W_15;

    while (FIND(0, virtual_SP, TEST_W_7_AND_W_8, W_7)) {
        while (FIND(1, virtual_SP, TEST_W_7_AND_W_8, W_8)) {
            while (FIND(2, virtual_SP, TEST_W_12, W_12)) {
                if (FIND(3, virtual_SP, TEST_W_15, W_15)) {
                    final_DC = virtual_SP;
                }
            }
        }
    }

    final_DC = DC;
    return false;
}

void _init_search_space(SEARCH_SPACE &searchSpace) {
    searchSpace.W[0] = "?????????????????????????????x??";
    rev(searchSpace.W[0]);
    searchSpace.W[1] = searchSpace.W[2] = searchSpace.W[3] = "????????????????????????????????";
}