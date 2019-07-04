#include "diff_chars.h"

#define MAX_INCONSISTENCIES 10000

bool HEAD_OCCUR() {
    random_device r;
    default_random_engine e1(r());
    uniform_int_distribution<int> uniform_dist(0, 1);

    return uniform_dist(e1);
}

bool CONSISTENT(const DEL &in) {
    for (char bit : in)
        if (bit == '#') return false;

    return true;
}

void CHANGE_1(DEL &in, const int &index) {
    if (in[index] == '?') in[index] = '-';
    else if (HEAD_OCCUR() % 2 == 0) in[index] = 'u';
    else in[index] = 'n';
}

bool SPACE::_init_(const string &INPUT_FILE_NAME) {
    std::ifstream input_file;
    input_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        input_file.open(INPUT_FILE_NAME);

        INCONSISTENCIES = 0;

        input_file >> MAX_ROUNDS;
        A.resize(MAX_ROUNDS + IV_OFFSET);
        E.resize(MAX_ROUNDS + IV_OFFSET);
        W.resize(MAX_ROUNDS);

        for (int index = -IV_OFFSET; index < 0; index++) {
            input_file >> A[(MAX_ROUNDS - 1) + (-index)];
            std::reverse(A[(MAX_ROUNDS - 1) + (-index)].begin(), A[(MAX_ROUNDS - 1) + (-index)].end());

            input_file >> E[(MAX_ROUNDS - 1) + (-index)];
            std::reverse(E[(MAX_ROUNDS - 1) + (-index)].begin(), E[(MAX_ROUNDS - 1) + (-index)].end());
        }

        for (int index = 0; index < MAX_ROUNDS; index++) {
            input_file >> A[index] >> E[index] >> W[index];
            std::reverse(A[index].begin(), A[index].end());
            std::reverse(E[index].begin(), E[index].end());
            std::reverse(W[index].begin(), W[index].end());
        }

        input_file.close();

        return true;

    } catch (std::ifstream::failure &fail) {
        std::cerr << "Exception opening input file.\n";
    }

    return false;
}

bool SPACE::operator==(SPACE &SPACE) {
    for (int index = -IV_OFFSET; index < 0; index++)
        if ((A[(MAX_ROUNDS - 1) + (-index)] == SPACE.A[(MAX_ROUNDS - 1) + (-index)]) && (E[(MAX_ROUNDS - 1) + (-index)] == SPACE.E[(MAX_ROUNDS - 1) + (-index)]))
            continue;
        else
            return false;

    for (int index = 0; index < MAX_ROUNDS; index++)
        if ((A[index] == SPACE.A[index]) && (E[index] == SPACE.E[index]) && (W[index] == SPACE.W[index]))
            continue;
        else return false;

    return true;
}

void SPACE::write(ofstream &out) {
    for (int index = -IV_OFFSET; index < 0; index++) {
        std::reverse(A[(MAX_ROUNDS - 1) + (-index)].begin(), A[(MAX_ROUNDS - 1) + (-index)].end());
        out << "-" << setw(2) << setfill('0') << -index << ": " << A[(MAX_ROUNDS - 1) + (-index)] << " ";

        std::reverse(E[(MAX_ROUNDS - 1) + (-index)].begin(), E[(MAX_ROUNDS - 1) + (-index)].end());
        out << E[(MAX_ROUNDS - 1) + (-index)] << "\n";
    }

    for (int index = 0; index < MAX_ROUNDS; index++) {
        std::reverse(A[index].begin(), A[index].end());
        std::reverse(E[index].begin(), E[index].end());
        std::reverse(W[index].begin(), W[index].end());

        out << "+" << setw(2) << setfill('0') << index << ": " << A[index] << " " << E[index] << " " << W[index] << "\n";
    }
}

void SPACE::print(const int &step) {
    cout << "\033[2J\033[1;1H";
    cout << "TOTAL INCONSISTENCIES: " << INCONSISTENCIES << "\n";

    int index;
    DEL temp;
    for (index = 0; index < step; index++) {
        temp = A[index];
        reverse(temp.begin(), temp.end());
        cout << setw(2) << setfill('0') << index << ": " << temp;
        temp = E[index];
        reverse(temp.begin(), temp.end());
        cout << " " << temp;
        temp = W[index];
        reverse(temp.begin(), temp.end());
        cout << " " << temp << "\n";
    }

    {
        temp = A[index];
        reverse(temp.begin(), temp.end());
        cout << setw(2) << setfill('0') << index << ": " << temp;
        temp = E[index];
        reverse(temp.begin(), temp.end());
        cout << " " << temp;
        temp = W[index];
        reverse(temp.begin(), temp.end());
        cout << " " << temp << " <<\n";
    }

    for (index = step + 1; index < MAX_ROUNDS; index++) {
        temp = A[index];
        reverse(temp.begin(), temp.end());
        cout << setw(2) << setfill('0') << index << ": " << temp;
        temp = E[index];
        reverse(temp.begin(), temp.end());
        cout << " " << temp;
        temp = W[index];
        reverse(temp.begin(), temp.end());
        cout << " " << temp << "\n";
    }
}

void SPACE::_init_U() {
    U.clear();
    for (int index = 0; index < MAX_ROUNDS; index++) {
        for (int bit = 0; bit < WORD_LENGTH; bit++) {
            if (A[index][bit] == '?' || A[index][bit] == 'x') U.emplace_back('A', make_pair(index, bit));
            if (E[index][bit] == '?' || E[index][bit] == 'x')  U.emplace_back('E', make_pair(index, bit));
            if (W[index][bit] == '?' || W[index][bit] == 'x')  U.emplace_back('W', make_pair(index, bit));
        }
    }
    shuffle(U.begin(), U.end(), std::mt19937(std::random_device()()));
}

bool SPACE::SPACE_CONSISTENT(const int &round) {
    CARRY_GRAPH G;
    DEL new_W, new_E, new_A;

    for (int step = round; step < MAX_ROUNDS; step++) {
        if (step >= 16) {
            G._init_carry_graph(vector<DEL>{GET_DEL_SMALL_SIGMA_1(W[step - 2]), W[step - 7], GET_DEL_SMALL_SIGMA_0(W[step - 15]), W[step - 16]});
            G.COMPUTE_GRAPH();
            new_W = GET_TIGHTEN_WORD(G.compressed, W[step]);
            if (!CONSISTENT(new_W))
                return false;
            G.CLEAR_GRAPH();
        } else new_W = W[step];

        int stepm4 = ((step - 4) < 0)?(MAX_ROUNDS - 1 - (step - 4)): (step - 4);
        int stepm3 = ((step - 3) < 0)?(MAX_ROUNDS - 1 - (step - 3)): (step - 3);
        int stepm2 = ((step - 2) < 0)?(MAX_ROUNDS - 1 - (step - 2)): (step - 2);
        int stepm1 = ((step - 1) < 0)?(MAX_ROUNDS - 1 - (step - 1)): (step - 1);
        G._init_carry_graph(vector<DEL>{E[stepm4], GET_DEL_SIGMA_1(E[stepm1]), GET_DEL_IF__WORD(E[stepm1], E[stepm2], E[stepm3]), _K[step], new_W});
        G.COMPUTE_GRAPH();
        new_E = G.compressed;
        G.CLEAR_GRAPH();

        G._init_carry_graph(vector<DEL>{GET_DEL_SIGMA_0(A[stepm1]), GET_DEL_MAJ_WORD(A[stepm1], A[stepm2], A[stepm3]), new_E});
        G.COMPUTE_GRAPH();
        new_A = GET_TIGHTEN_WORD(G.compressed, A[step]);
        if (!CONSISTENT(new_A)) return false;
        G.CLEAR_GRAPH();

        G._init_carry_graph(vector<DEL>{new_A, new_E});
        G.COMPUTE_GRAPH();
        new_E = GET_TIGHTEN_WORD(G.compressed, E[step]);
        if (!CONSISTENT(new_E)) return false;
        G.CLEAR_GRAPH();

        W[step] = new_W;
        E[step] = new_E;
        A[step] = new_A;

        print(step);
    }

    _init_U();
    return true;
}

bool SPACE::PHASE_1() {
    search_checkpoint searchCheckpoint;
    searchCheckpoint.space = *this;
    searchCheckpoint.var = '#';
    searchCheckpoint.step = MAX_ROUNDS;
    searchCheckpoint.index = WORD_LENGTH;
    searchCheckpoint.choice = 0;

    HISTORY.push(searchCheckpoint);

    _init_U();

    while (!U.empty()) {

        searchCheckpoint.var = (*U.begin()).first;
        searchCheckpoint.step = (*U.begin()).second.first;
        searchCheckpoint.index = (*U.begin()).second.second;

        U.erase(U.begin());
        char change;

        switch (searchCheckpoint.var) {
            case 'W': {
                CHANGE_1(W[searchCheckpoint.step], searchCheckpoint.index);
                change = W[searchCheckpoint.step][searchCheckpoint.index];
                break;
            }
            case 'E': {
                CHANGE_1(E[searchCheckpoint.step], searchCheckpoint.index);
                change = E[searchCheckpoint.step][searchCheckpoint.index];
                break;
            }
            case 'A': {
                CHANGE_1(A[searchCheckpoint.step], searchCheckpoint.index);
                change = A[searchCheckpoint.step][searchCheckpoint.index];
                break;
            }
            default:
                return false;
        }

        searchCheckpoint.choice = 1;

        if (SPACE_CONSISTENT(searchCheckpoint.step)) {
            searchCheckpoint.space = *this;
            HISTORY.push(searchCheckpoint);
            continue;
        }

        if (++INCONSISTENCIES >= MAX_INCONSISTENCIES)
            return false;

        *this = HISTORY.top().space;
        change = (change == '-')?'x':((change == 'u')?'n':'u');
        switch (searchCheckpoint.var) {
            case 'W': {
                W[searchCheckpoint.step][searchCheckpoint.index] = change;
                break;
            }
            case 'E': {
                E[searchCheckpoint.step][searchCheckpoint.index] = change;
                break;
            }
            case 'A': {
                A[searchCheckpoint.step][searchCheckpoint.index] = change;
                break;
            }
            default:
                return false;
        }
        searchCheckpoint.choice = 2;

        if (SPACE_CONSISTENT(searchCheckpoint.step)) {
            searchCheckpoint.space = *this;
            HISTORY.push(searchCheckpoint);
            continue;
        }

        do {
            if (++INCONSISTENCIES >= MAX_INCONSISTENCIES)
                return false;

            while (HISTORY.top().choice == 2) {
                HISTORY.pop();
                if (HISTORY.empty()) return false;
            }

            searchCheckpoint = HISTORY.top();
            HISTORY.pop();
            switch (searchCheckpoint.var) {
                case 'W': {
                    change = searchCheckpoint.space.W[searchCheckpoint.step][searchCheckpoint.index];
                    change = (change == '-')?'x':((change == 'u')?'n':'u');
                    searchCheckpoint.space.W[searchCheckpoint.step][searchCheckpoint.index] = change;
                    break;
                }
                case 'E': {
                    change = searchCheckpoint.space.E[searchCheckpoint.step][searchCheckpoint.index];
                    change = (change == '-')?'x':((change == 'u')?'n':'u');
                    searchCheckpoint.space.E[searchCheckpoint.step][searchCheckpoint.index] = change;
                    break;
                }
                case 'A': {
                    change = searchCheckpoint.space.A[searchCheckpoint.step][searchCheckpoint.index];
                    change = (change == '-')?'x':((change == 'u')?'n':'u');
                    searchCheckpoint.space.E[searchCheckpoint.step][searchCheckpoint.index] = change;
                    break;
                }
                default:
                    return false;
            }
            searchCheckpoint.choice = 2;
        } while (!searchCheckpoint.space.SPACE_CONSISTENT(searchCheckpoint.step));

        HISTORY.push(searchCheckpoint);
    }

    return true;
}

bool SEARCH(SPACE &space) {
    SPACE TEMP_SPACE = space;
    while (!TEMP_SPACE.PHASE_1()) {
        while (!HISTORY.empty()) HISTORY.pop();
        TEMP_SPACE = space;
        INCONSISTENCIES = 0;
    }
    space = TEMP_SPACE;
    return true;
}