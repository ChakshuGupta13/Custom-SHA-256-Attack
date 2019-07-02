#ifndef DIFF_CHARS_H
#define DIFF_CHARS_H

#include <random>

#include "carry_graph.h"

int INCONSISTENCIES = 0;

class SPACE {
public:
    vector<DEL> A;
    vector<DEL> E;
    vector<DEL> W;

    /*
     * MAX_ROUNDS: Specified in 1-based index.
     */
    int MAX_ROUNDS;

    bool _init_(const string &INPUT_FILE_NAME);

    bool operator==(SPACE &SPACE);

    void print(const int &step);

    void write(ofstream &out);

    void _init_U();

    bool PHASE_1();

    bool SPACE_CONSISTENT(const int &step);
};

struct search_checkpoint {
    SPACE space;
    char var{};
    int step{};
    int index{};
    int choice{};
};

vector<pair<char, pair<int, int>>> U;
stack<search_checkpoint> HISTORY;

bool SEARCH(SPACE &);

#endif //DIFF_CHARS_H