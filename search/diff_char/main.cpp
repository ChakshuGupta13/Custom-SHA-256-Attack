#include "phase1/phase1.h"

void _init_() {
    _init_diff_bit_render_s();
    _init_diff_bit_render_to_index();
    _init_diff_bit_index();
    _init_DEL_IF_MAJ_XOR_DP();
    _init_DEL_K();
    _init_tighten_matrix();
}

int main(int argc, char *argv[]) {
    if (argc != 2){
        cout << "Run: " << argv[0] << " <Characteristics File>\n";
        return 0;
    }

    srandom(time(nullptr));

    cout << "Initializing Global Matrices...\n";
    _init_();

    SPACE space;
    cout << "Reading search space...\n";
    if (space._init_(argv[1])) {

        cout << "Press 'Enter' to begin the search.\n";
        getchar();
        cout << "Searching...\n";
        SEARCH(space);
        cout << "Found the characteristics!\n";

        cout << "Storing result...\n";
        ofstream output_file("./start_points/found_characteristics.txt", ios::out | ios::trunc);
        space.write(output_file);

        output_file.close();
    }

    return 0;
}