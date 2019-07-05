#ifndef DIFFERENCE_PROPAGATION_TABLES_H
#define DIFFERENCE_PROPAGATION_TABLES_H

#include "sha-256.h"

vector<pair<bool, bool> > index_to_config(int idx);

char diff_bit_render_1[2][2][2][2];
char diff_bit_render_2[16];
int diff_bit_index[2][2][2][2];
int diff_bit_render_to_index[256];

map<string, string> SIGMA_0, SIGMA_1;
map<string, string> SMALL_SIGMA_0, SMALL_SIGMA_1;

map<pair<string, string>, string> tighten;
int tighten_matrix[16][16];

int DEL_IF_DP[16][16][16], DEL_MAJ_DP[16][16][16], DEL_XOR_DP[16][16][16];

int config_to_index(const set<pair<bool, bool>> &conditions_intersection) {
    vector<bool> index(4);

    if (conditions_intersection.count(make_pair(0, 0)) > 0) index[0] = true;
    if (conditions_intersection.count(make_pair(1, 0)) > 0) index[1] = true;
    if (conditions_intersection.count(make_pair(0, 1)) > 0) index[2] = true;
    if (conditions_intersection.count(make_pair(1, 1)) > 0) index[3] = true;

    return diff_bit_index[index[0]][index[1]][index[2]][index[3]];
}

void _init_tighten_matrix() {
    for (int row = 0; row < 16; row++)
        for (int col = 0; col < 16; col++) {
            vector<pair<bool, bool>> row_allow_config = index_to_config(row);
            vector<pair<bool, bool>> col_allow_config = index_to_config(col);

            set<pair<bool, bool>> configurations_intersection;

            for (auto configuration : row_allow_config) {
                if (find(col_allow_config.begin(), col_allow_config.end(), configuration) != col_allow_config.end()) {
                    configurations_intersection.insert(configuration);
                }
            }

            tighten_matrix[row][col] = config_to_index(configurations_intersection);
        }
}

void _init_diff_bit_render_s() {
    diff_bit_render_1[1][1][1][1] = diff_bit_render_2[0] = '?';
    diff_bit_render_1[1][0][0][1] = diff_bit_render_2[1] = '-';
    diff_bit_render_1[0][1][1][0] = diff_bit_render_2[2] = 'x';
    diff_bit_render_1[1][0][0][0] = diff_bit_render_2[3] = '0';
    diff_bit_render_1[0][1][0][0] = diff_bit_render_2[4] = 'u';
    diff_bit_render_1[0][0][1][0] = diff_bit_render_2[5] = 'n';
    diff_bit_render_1[0][0][0][1] = diff_bit_render_2[6] = '1';
    diff_bit_render_1[0][0][0][0] = diff_bit_render_2[7] = '#';
    diff_bit_render_1[1][1][0][0] = diff_bit_render_2[8] = '3';
    diff_bit_render_1[1][0][1][0] = diff_bit_render_2[9] = '5';
    diff_bit_render_1[1][1][1][0] = diff_bit_render_2[10] = '7';
    diff_bit_render_1[0][1][0][1] = diff_bit_render_2[11] = 'A';
    diff_bit_render_1[1][1][0][1] = diff_bit_render_2[12] = 'B';
    diff_bit_render_1[0][0][1][1] = diff_bit_render_2[13] = 'C';
    diff_bit_render_1[1][0][1][1] = diff_bit_render_2[14] = 'D';
    diff_bit_render_1[0][1][1][1] = diff_bit_render_2[15] = 'E';
}

void _init_diff_bit_render_to_index() {
    diff_bit_render_to_index[(int) '?'] = 0;
    diff_bit_render_to_index[(int) '-'] = 1;
    diff_bit_render_to_index[(int) 'x'] = 2;
    diff_bit_render_to_index[(int) '0'] = 3;
    diff_bit_render_to_index[(int) 'u'] = 4;
    diff_bit_render_to_index[(int) 'n'] = 5;
    diff_bit_render_to_index[(int) '1'] = 6;
    diff_bit_render_to_index[(int) '#'] = 7;
    diff_bit_render_to_index[(int) '3'] = 8;
    diff_bit_render_to_index[(int) '5'] = 9;
    diff_bit_render_to_index[(int) '7'] = 10;
    diff_bit_render_to_index[(int) 'A'] = 11;
    diff_bit_render_to_index[(int) 'B'] = 12;
    diff_bit_render_to_index[(int) 'C'] = 13;
    diff_bit_render_to_index[(int) 'D'] = 14;
    diff_bit_render_to_index[(int) 'E'] = 15;
}

void _init_diff_bit_index() {
    diff_bit_index[1][1][1][1] = 0;
    diff_bit_index[1][0][0][1] = 1;
    diff_bit_index[0][1][1][0] = 2;
    diff_bit_index[1][0][0][0] = 3;
    diff_bit_index[0][1][0][0] = 4;
    diff_bit_index[0][0][1][0] = 5;
    diff_bit_index[0][0][0][1] = 6;
    diff_bit_index[0][0][0][0] = 7;
    diff_bit_index[1][1][0][0] = 8;
    diff_bit_index[1][0][1][0] = 9;
    diff_bit_index[1][1][1][0] = 10;
    diff_bit_index[0][1][0][1] = 11;
    diff_bit_index[1][1][0][1] = 12;
    diff_bit_index[0][0][1][1] = 13;
    diff_bit_index[1][0][1][1] = 14;
    diff_bit_index[0][1][1][1] = 15;
}

void _init_DEL_IF_MAJ_XOR_DP() {
    for (int dx = 0; dx < 16; dx++)
        for (int dy = 0; dy < 16; dy++)
            for (int dz = 0; dz < 16; dz++) {
                vector<pair<bool, bool>> x_conds = index_to_config(dx);
                vector<pair<bool, bool>> y_conds = index_to_config(dy);
                vector<pair<bool, bool>> z_conds = index_to_config(dz);
                set<pair<bool, bool>> possible_conds_if, possible_conds_maj, possible_conds_xor;
                for (auto x : x_conds)
                    for (auto y : y_conds)
                        for (auto z : z_conds) {
                            bool x1_bit = x.first, y1_bit = y.first, z1_bit = z.first;
                            bool x2_bit = x.second, y2_bit = y.second, z2_bit = z.second;

                            bool x1_if = IF(x1_bit, y1_bit, z1_bit);
                            bool x2_if = IF(x2_bit, y2_bit, z2_bit);
                            possible_conds_if.insert(make_pair(x1_if, x2_if));

                            bool x1_maj = MAJ(x1_bit, y1_bit, z1_bit);
                            bool x2_maj = MAJ(x2_bit, y2_bit, z2_bit);
                            possible_conds_maj.insert(make_pair(x1_maj, x2_maj));

                            bool x1_xor = XOR(x1_bit, y1_bit, z1_bit);
                            bool x2_xor = XOR(x2_bit, y2_bit, z2_bit);
                            possible_conds_xor.insert(make_pair(x1_xor, x2_xor));
                        }

                pair<bool, bool> c0 = make_pair(0, 0);
                pair<bool, bool> c1 = make_pair(1, 0);
                pair<bool, bool> c2 = make_pair(0, 1);
                pair<bool, bool> c3 = make_pair(1, 1);

                int b0_if = (possible_conds_if.count(c0) > 0);
                int b1_if = (possible_conds_if.count(c1) > 0);
                int b2_if = (possible_conds_if.count(c2) > 0);
                int b3_if = (possible_conds_if.count(c3) > 0);
                DEL_IF_DP[dx][dy][dz] = diff_bit_index[b0_if][b1_if][b2_if][b3_if];

                int b0_maj = (possible_conds_maj.count(c0) > 0);
                int b1_maj = (possible_conds_maj.count(c1) > 0);
                int b2_maj = (possible_conds_maj.count(c2) > 0);
                int b3_maj = (possible_conds_maj.count(c3) > 0);
                DEL_MAJ_DP[dx][dy][dz] = diff_bit_index[b0_maj][b1_maj][b2_maj][b3_maj];

                int b0_xor = (possible_conds_xor.count(c0) > 0);
                int b1_xor = (possible_conds_xor.count(c1) > 0);
                int b2_xor = (possible_conds_xor.count(c2) > 0);
                int b3_xor = (possible_conds_xor.count(c3) > 0);
                DEL_XOR_DP[dx][dy][dz] = diff_bit_index[b0_xor][b1_xor][b2_xor][b3_xor];
            }
}

vector<pair<bool, bool>> index_to_config(int diff_bit) {
    assert((0 <= diff_bit) && (diff_bit < 16));

    pair<bool, bool> _0(0, 0);
    pair<bool, bool> _u(1, 0);
    pair<bool, bool> _n(0, 1);
    pair<bool, bool> _1(1, 1);

    vector<pair<bool, bool>> config;

    switch (diff_bit) {
        case 0: {
            config.push_back(_0);
            config.push_back(_u);
            config.push_back(_n);
            config.push_back(_1);
            break;
        }
        case 1: {
            config.push_back(_0);
            config.push_back(_1);
            break;
        }
        case 2: {
            config.push_back(_u);
            config.push_back(_n);
            break;
        }
        case 3: {
            config.push_back(_0);
            break;
        }
        case 4: {
            config.push_back(_u);
            break;
        }
        case 5: {
            config.push_back(_n);
            break;
        }
        case 6: {
            config.push_back(_1);
            break;
        }
        case 8: {
            config.push_back(_0);
            config.push_back(_u);
            break;
        }
        case 9: {
            config.push_back(_0);
            config.push_back(_n);
            break;
        }
        case 10: {
            config.push_back(_0);
            config.push_back(_u);
            config.push_back(_n);
            break;
        }
        case 11: {
            config.push_back(_u);
            config.push_back(_1);
            break;
        }
        case 12: {
            config.push_back(_0);
            config.push_back(_u);
            config.push_back(_1);
            break;
        }
        case 13: {
            config.push_back(_n);
            config.push_back(_1);
            break;
        }
        case 14: {
            config.push_back(_0);
            config.push_back(_n);
            config.push_back(_1);
            break;
        }
        case 15: {
            config.push_back(_u);
            config.push_back(_n);
            config.push_back(_1);
            break;
        }
        default: {
            config.clear();
        }
    }

    return config;
}

DEL ROTATE_RIGHT_WORD(DEL in, int offset) {
    string out;
    for (int bit = 0; bit < WORD_LENGTH; bit++) out += string(1, in[(bit + offset) % WORD_LENGTH]);
    return out;
}

DEL ROTATE_LEFT__WORD(DEL in, int offset) {
    string out;
    for (int bit = 0; bit < WORD_LENGTH; bit++) out += string(1, in[((bit - offset) < 0) ? (WORD_LENGTH + bit - offset) : (bit - offset)]);
    return out;
}

DEL SHIFT__RIGHT_WORD(DEL x, int offset){
    string out;
    for (int index = 0; index < WORD_LENGTH; index++) out += "?";

    for (int index = 0; index < offset; index++) out[WORD_LENGTH - index] = '0';
    for (int index = offset; index < WORD_LENGTH; index++) out[index - offset] = x[index];

    return out;
}

DEL GET_TIGHTEN_WORD(string x1, string x2, const int num_bits = WORD_LENGTH) {
    if (tighten.count(make_pair(x1, x2)) > 0) return tighten[make_pair(x1, x2)];

    string out;
    for (int bit = 0; bit < num_bits; bit++) out += string(1, diff_bit_render_2[tighten_matrix[diff_bit_render_to_index[x1[bit]]][diff_bit_render_to_index[x2[bit]]]]);

    return tighten[make_pair(x1, x2)] = out;
}

DEL GET_DEL_IF__WORD(string x, string y, string z) {
    string DEL_IF;
    for (int bit = 0; bit < WORD_LENGTH; bit++)
        DEL_IF += string(1, diff_bit_render_2[DEL_IF_DP[diff_bit_render_to_index[x[bit]]][diff_bit_render_to_index[y[bit]]][diff_bit_render_to_index[z[bit]]]]);

    return DEL_IF;
}

DEL GET_DEL_MAJ_WORD(string x, string y, string z) {
    string DEL_MAJ;
    for (int bit = 0; bit < WORD_LENGTH; bit++)
        DEL_MAJ += string(1, diff_bit_render_2[DEL_MAJ_DP[diff_bit_render_to_index[x[bit]]][diff_bit_render_to_index[y[bit]]][diff_bit_render_to_index[z[bit]]]]);

    return DEL_MAJ;
}

DEL GET_DEL_XOR_WORD(string x, string y, string z) {
    string DEL_XOR;
    for (int bit = 0; bit < WORD_LENGTH; bit++)
        DEL_XOR += string(1, diff_bit_render_2[DEL_XOR_DP[diff_bit_render_to_index[x[bit]]][diff_bit_render_to_index[y[bit]]][diff_bit_render_to_index[z[bit]]]]);

    return DEL_XOR;
}

DEL GET_DEL_SIGMA_0(const DEL &in) {
    if (SIGMA_0.count(in) > 0) {
        return SIGMA_0[in];
    }

    DEL x = ROTATE_RIGHT_WORD(in, 2);
    DEL y = ROTATE_RIGHT_WORD(in, 13);
    DEL z = ROTATE_RIGHT_WORD(in, 22);

    string DEL_XOR;
    for (int bit = 0; bit < WORD_LENGTH; bit++)
        DEL_XOR += string(1, diff_bit_render_2[DEL_XOR_DP[diff_bit_render_to_index[x[bit]]][diff_bit_render_to_index[y[bit]]][diff_bit_render_to_index[z[bit]]]]);

    SIGMA_0[in] = DEL_XOR;
    return SIGMA_0[in];
}

DEL GET_DEL_SIGMA_1(const DEL& in) {
    if (SIGMA_1.count(in) > 0) {
        return SIGMA_1[in];
    }

    DEL x = ROTATE_RIGHT_WORD(in, 6);
    DEL y = ROTATE_RIGHT_WORD(in, 11);
    DEL z = ROTATE_RIGHT_WORD(in, 25);

    string DEL_XOR;
    for (int bit = 0; bit < WORD_LENGTH; bit++)
        DEL_XOR += string(1, diff_bit_render_2[DEL_XOR_DP[diff_bit_render_to_index[x[bit]]][diff_bit_render_to_index[y[bit]]][diff_bit_render_to_index[z[bit]]]]);

    SIGMA_1[in] = DEL_XOR;
    return SIGMA_1[in];
}

DEL GET_DEL_SMALL_SIGMA_0(const DEL& in) {
    if (SMALL_SIGMA_0.count(in) > 0)
        return SMALL_SIGMA_0[in];

    DEL x = ROTATE_RIGHT_WORD(in, 7);
    DEL y = ROTATE_RIGHT_WORD(in, 18);
    DEL z = SHIFT__RIGHT_WORD(in, 3);

    string DEL_XOR;
    for (int bit = 0; bit < WORD_LENGTH; bit++)
        DEL_XOR += string(1, diff_bit_render_2[DEL_XOR_DP[diff_bit_render_to_index[x[bit]]][diff_bit_render_to_index[y[bit]]][diff_bit_render_to_index[z[bit]]]]);

    SMALL_SIGMA_0[in] = DEL_XOR;
    return SMALL_SIGMA_0[in];
}

DEL GET_DEL_SMALL_SIGMA_1(const DEL &in) {
    if (SMALL_SIGMA_1.count(in) > 0)
        return SMALL_SIGMA_1[in];

    DEL x = ROTATE_RIGHT_WORD(in, 17);
    DEL y = ROTATE_RIGHT_WORD(in, 19);
    DEL z = SHIFT__RIGHT_WORD(in, 10);

    string DEL_XOR;
    for (int bit = 0; bit < WORD_LENGTH; bit++)
        DEL_XOR += string(1, diff_bit_render_2[DEL_XOR_DP[diff_bit_render_to_index[x[bit]]][diff_bit_render_to_index[y[bit]]][diff_bit_render_to_index[z[bit]]]]);

    SMALL_SIGMA_1[in] = DEL_XOR;
    return SMALL_SIGMA_1[in];
}

#endif //DIFFERENCE_PROPAGATION_TABLES_H