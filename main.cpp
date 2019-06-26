#include "diff_chars.h"

void _init_() {
    _init_diff_bit_render_s();
    _init_diff_bit_render_to_index();
    _init_diff_bit_index();
    _init_DEL_IF_MAJ_XOR_DP();
    _init_DEL_K();
    _init_tighten_matrix();
}

int main(int argc, char *argv[]) {
    srandom(time(nullptr));

    cout << "Initializing Global Matrices...\n";
    _init_();

    SEARCH_SPACE searchSpace, finalSpace;
    _init_search_space(searchSpace);

    cout << "To begin the search for that differential characteristics: press Enter!" << endl;
    getchar();
    cout << "Searching..." << endl;
    search_SP(searchSpace, finalSpace);
    cout << "Found the characteristics!" << endl;
    cout << "To print the found differential characteristics: press Enter!" << endl;
    getchar();
    print_SP(0, searchSpace);

    return 0;
}