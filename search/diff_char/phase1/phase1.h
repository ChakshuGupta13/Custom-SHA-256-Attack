#ifndef DIFF_CHARS_H
#define DIFF_CHARS_H

#include <random>

#include "../../../source/carry_graph.h"

#define MAX_INCONSISTENCIES 10000

int INCONSISTENCIES = 0;
//    shuffle(U.begin(), U.end(), std::mt19937(std::random_device()()));

class bit_loc {
public:
    char state_var;
    int step;
    int index;

    bool operator == (bit_loc bitLoc) {
        return bitLoc.state_var == state_var && bitLoc.step == step && bitLoc.index == index;
    }
};

class CHAR {
public:
    vector<DEL> A, E, W;
    int MAX_ROUNDS;

    void print(const int &step);
};

void CHAR::print(const int &step) {
    cout << "\033[2J\033[1;1H";

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

class SPACE {
public:
    // MAX_ROUNDS: Specified in 1-based index.
    CHAR space;
    vector<pair<bit_loc, vector<char>>> loose_bits, bound_bits;

    bool _init_(const string &INPUT_FILE_NAME);

    bool operator==(SPACE &SPACE);

    void write(ofstream &out);

    void _init_U();

    bool PHASE_1();

    bool SPACE_CONSISTENT();
};

stack<bit_loc> HISTORY;

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

        input_file >> space.MAX_ROUNDS;
        space.A.resize(space.MAX_ROUNDS + IV_OFFSET);
        space.E.resize(space.MAX_ROUNDS + IV_OFFSET);
        space.W.resize(space.MAX_ROUNDS);

        for (int index = -IV_OFFSET; index < 0; index++) {
            input_file >> space.A[(space.MAX_ROUNDS - 1) + (-index)];
            std::reverse(space.A[(space.MAX_ROUNDS - 1) + (-index)].begin(), space.A[(space.MAX_ROUNDS - 1) + (-index)].end());

            input_file >> space.E[(space.MAX_ROUNDS - 1) + (-index)];
            std::reverse(space.E[(space.MAX_ROUNDS - 1) + (-index)].begin(), space.E[(space.MAX_ROUNDS - 1) + (-index)].end());
        }

        for (int index = 0; index < space.MAX_ROUNDS; index++) {
            input_file >> space.A[index] >> space.E[index] >> space.W[index];
            std::reverse(space.A[index].begin(), space.A[index].end());
            std::reverse(space.E[index].begin(), space.E[index].end());
            std::reverse(space.W[index].begin(), space.W[index].end());
        }

        input_file.close();

        return true;

    } catch (std::ifstream::failure &fail) {
        std::cerr << "Exception happened while opening input file.\n";
    }

    return false;
}

bool SPACE::operator==(SPACE &SPACE) {
    for (int index = -IV_OFFSET; index < 0; index++)
        if ((space.A[(space.MAX_ROUNDS - 1) + (-index)] == SPACE.space.A[(space.MAX_ROUNDS - 1) + (-index)]) &&
            (space.E[(space.MAX_ROUNDS - 1) + (-index)] == SPACE.space.E[(space.MAX_ROUNDS - 1) + (-index)]))
            continue;
        else
            return false;

    for (int index = 0; index < space.MAX_ROUNDS; index++)
        if ((space.A[index] == SPACE.space.A[index]) &&
        (space.E[index] == SPACE.space.E[index]) &&
        (space.W[index] == SPACE.space.W[index]))
            continue;
        else return false;

    return true;
}

void SPACE::write(ofstream &out) {
    for (int index = -IV_OFFSET; index < 0; index++) {
        std::reverse(space.A[(space.MAX_ROUNDS - 1) + (-index)].begin(), space.A[(space.MAX_ROUNDS - 1) + (-index)].end());
        out << "-" << setw(2) << setfill('0') << -index << ": " << space.A[(space.MAX_ROUNDS - 1) + (-index)] << " ";

        std::reverse(space.E[(space.MAX_ROUNDS - 1) + (-index)].begin(), space.E[(space.MAX_ROUNDS - 1) + (-index)].end());
        out << space.E[(space.MAX_ROUNDS - 1) + (-index)] << "\n";
    }

    for (int index = 0; index < space.MAX_ROUNDS; index++) {
        std::reverse(space.A[index].begin(), space.A[index].end());
        std::reverse(space.E[index].begin(), space.E[index].end());
        std::reverse(space.W[index].begin(), space.W[index].end());

        out << "+" << setw(2) << setfill('0') << index << ": " << space.A[index] << " " << space.E[index] << " " << space.W[index]
            << "\n";
    }
}

void SPACE::_init_U() {
    for (int step = 0; step < space.MAX_ROUNDS; step++) {
        for (int index = 0; index < WORD_LENGTH; index++) {
            if (space.A[step][index] == '?' || space.A[step][index] == 'x')
                loose_bits.emplace_back(bit_loc{'A', step, index}, vector<char>{space.A[step][index]});
            if (space.E[step][index] == '?' || space.E[step][index] == 'x')
                loose_bits.emplace_back(bit_loc{'E', step, index}, vector<char>{space.E[step][index]});
            if (space.W[step][index] == '?' || space.W[step][index] == 'x')
                loose_bits.emplace_back(bit_loc{'W', step, index}, vector<char>{space.W[step][index]});
        }
    }
}

bool SPACE::SPACE_CONSISTENT() {
    CARRY_GRAPH G;
    DEL new_W, new_E, new_A;
    CHAR new_space = space;

    for (int step = 0; step < space.MAX_ROUNDS; step++) {
        if (step >= 16) {
            G._init_carry_graph(vector<DEL>{GET_DEL_SMALL_SIGMA_1(new_space.W[step - 2]),
                                            new_space.W[step - 7],
                                            GET_DEL_SMALL_SIGMA_0(new_space.W[step - 15]),
                                            new_space.W[step - 16]});
            G.COMPUTE_GRAPH();
            new_W = GET_TIGHTEN_WORD(G.compressed, new_space.W[step]);
            if (!CONSISTENT(new_W)) return false;
            G.CLEAR_GRAPH();
        } else new_W = new_space.W[step];

        int stepm4 = ((step - 4) < 0) ? (space.MAX_ROUNDS - 1 - (step - 4)) : (step - 4);
        int stepm3 = ((step - 3) < 0) ? (space.MAX_ROUNDS - 1 - (step - 3)) : (step - 3);
        int stepm2 = ((step - 2) < 0) ? (space.MAX_ROUNDS - 1 - (step - 2)) : (step - 2);
        int stepm1 = ((step - 1) < 0) ? (space.MAX_ROUNDS - 1 - (step - 1)) : (step - 1);
        G._init_carry_graph(vector<DEL>{new_space.E[stepm4],
                                        GET_DEL_SIGMA_1(new_space.E[stepm1]),
                                        GET_DEL_IF__WORD(new_space.E[stepm1], new_space.E[stepm2], new_space.E[stepm3]),
                                        _K[step],
                                        new_W});
        G.COMPUTE_GRAPH();
        new_E = G.compressed;
        G.CLEAR_GRAPH();

        G._init_carry_graph(vector<DEL>{GET_DEL_SIGMA_0(new_space.A[stepm1]),
                                        GET_DEL_MAJ_WORD(new_space.A[stepm1], new_space.A[stepm2], new_space.A[stepm3]),
                                        new_E});
        G.COMPUTE_GRAPH();
        new_A = GET_TIGHTEN_WORD(G.compressed, new_space.A[step]);
        if (!CONSISTENT(new_A)) return false;
        G.CLEAR_GRAPH();

        G._init_carry_graph(vector<DEL>{new_A, new_E});
        G.COMPUTE_GRAPH();
        new_E = GET_TIGHTEN_WORD(G.compressed, new_space.E[step]);
        if (!CONSISTENT(new_E)) return false;
        G.CLEAR_GRAPH();

        new_space.W[step] = new_W;
        new_space.E[step] = new_E;
        new_space.A[step] = new_A;

        new_space.print(step);
    }

    return true;
}

bool SPACE::PHASE_1() {
    _init_U();

    while (!loose_bits.empty()) {
        std::random_device device;
        std::mt19937 generator(device());
        std::uniform_int_distribution<int> distribution(0, loose_bits.size() - 1);

        auto iter = loose_bits.begin();
        int times = distribution(generator);
        while (times--) iter++;
        bit_loc bitLoc = iter->first;
        char change;

        switch (bitLoc.state_var) {
            case 'W': {
                CHANGE_1(space.W[bitLoc.step], bitLoc.index);
                change = space.W[bitLoc.step][bitLoc.index];
                break;
            }
            case 'E': {
                CHANGE_1(space.E[bitLoc.step], bitLoc.index);
                change = space.E[bitLoc.step][bitLoc.index];
                break;
            }
            case 'A': {
                CHANGE_1(space.A[bitLoc.step], bitLoc.index);
                change = space.A[bitLoc.step][bitLoc.index];
                break;
            }
            default: return false;
        }
        iter->second.push_back(change);

        if (SPACE_CONSISTENT()) {
            if (change == '-' || change == 'u' || change == 'n') {
                bound_bits.push_back(*iter);
                loose_bits.erase(iter);
            }
            HISTORY.push(bitLoc);
            continue;
        }

        change = (change == '-') ? 'x' : ((change == 'u') ? 'n' : 'u');
        switch (bitLoc.state_var) {
            case 'W': {
                space.W[bitLoc.step][bitLoc.index] = change;
                break;
            }
            case 'E': {
                space.E[bitLoc.step][bitLoc.index] = change;
                break;
            }
            case 'A': {
                space.A[bitLoc.step][bitLoc.index] = change;
                break;
            }
            default: return false;
        }
        iter->second.push_back(change);

        if (SPACE_CONSISTENT()) {
            if (change == '-' || change == 'u' || change == 'n') {
                bound_bits.push_back(*iter);
                loose_bits.erase(iter);
            }
            HISTORY.push(bitLoc);
            continue;
        }

        do {
            bitLoc = HISTORY.top();
            bool loose_bit = false;
            for (iter = loose_bits.begin(); iter != loose_bits.end(); iter++) {
                if (iter->first == bitLoc) {
                    loose_bit = true;
                    switch (bitLoc.state_var) {
                        case 'W': {
                            space.W[bitLoc.step][bitLoc.index] = '?';
                            break;
                        }
                        case 'E': {
                            space.E[bitLoc.step][bitLoc.index] = '?';
                            break;
                        }
                        case 'A': {
                            space.A[bitLoc.step][bitLoc.index] = '?';
                            break;
                        }
                        default: return false;
                    }

                    iter->second.clear();
                    iter->second.push_back('?');

                    HISTORY.pop();
                    break;
                }
            }
            if (!loose_bit) {
                for (iter = bound_bits.begin(); iter != bound_bits.end(); iter++) {
                    if (iter->first == bitLoc) break;
                }
                if (iter->second[iter->second.size() - 1] == '-') {
                    switch (bitLoc.state_var) {
                        case 'W': {
                            space.W[bitLoc.step][bitLoc.index] = 'x';
                            break;
                        }
                        case 'E': {
                            space.E[bitLoc.step][bitLoc.index] = 'x';
                            break;
                        }
                        case 'A': {
                            space.A[bitLoc.step][bitLoc.index] = 'x';
                            break;
                        }
                        default: return false;
                    }

                    iter->second.push_back('x');
                    loose_bits.push_back(*iter);
                    bound_bits.erase(iter);
                } else if (iter->second[iter->second.size() - 2] != 'x') {
                    switch (bitLoc.state_var) {
                        case 'W': {
                            space.W[bitLoc.step][bitLoc.index] = '?';
                            break;
                        }
                        case 'E': {
                            space.E[bitLoc.step][bitLoc.index] = '?';
                            break;
                        }
                        case 'A': {
                            space.A[bitLoc.step][bitLoc.index] = '?';
                            break;
                        }
                        default: return false;
                    }

                    iter->second.clear();
                    iter->second.push_back('?');
                    loose_bits.push_back(*iter);
                    bound_bits.erase(iter);

                    HISTORY.pop();
                } else {
                    change = (iter->second[iter->second.size() - 2] == 'u')?'n':'u';

                    switch (bitLoc.state_var) {
                        case 'W': {
                            space.W[bitLoc.step][bitLoc.index] = change;
                            break;
                        }
                        case 'E': {
                            space.E[bitLoc.step][bitLoc.index] = change;
                            break;
                        }
                        case 'A': {
                            space.A[bitLoc.step][bitLoc.index] = change;
                            break;
                        }
                        default: return false;
                    }

                    iter->second.push_back(change);
                }
            }

            if(HISTORY.empty()) return false;
        } while (!SPACE_CONSISTENT());
    }

    return true;
}

bool SEARCH(SPACE &space) {
    SPACE TEMP_SPACE = space;
    while (!TEMP_SPACE.PHASE_1()) {
        while (!HISTORY.empty()) HISTORY.pop();
        TEMP_SPACE = space;
    }
    space = TEMP_SPACE;
    return true;
}

#endif //DIFF_CHARS_H