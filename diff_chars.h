#ifndef DIFF_CHARS_H
#define DIFF_CHARS_H

#include "carry_graph.h"

#define WORDS_IN_SEARCH_SPACE 4

class SEARCH_SPACE {
public:
    DEL W[WORDS_IN_SEARCH_SPACE];

    bool operator==(SEARCH_SPACE &DC) {
        for (int index = 0; index < WORDS_IN_SEARCH_SPACE; index++)
            if (DC.W[index] == this->W[index]) continue;
            else return false;

        return true;
    }
};

void print_SP(const int &step, const SEARCH_SPACE &SP, ostream &out = cout);

bool search_SP(const SEARCH_SPACE &DC, SEARCH_SPACE &final_DC);

void _init_search_space(SEARCH_SPACE &);

#endif //DIFF_CHARS_H