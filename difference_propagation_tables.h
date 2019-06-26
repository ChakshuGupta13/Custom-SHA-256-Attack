#ifndef DIFFERENCE_PROPAGATION_TABLES_H
#define DIFFERENCE_PROPAGATION_TABLES_H

#include "SHA-256.h"

extern char diff_bit_render_1[2][2][2][2], diff_bit_render_2[16];
extern int diff_bit_index[2][2][2][2];
extern int delta_if[16][16][16], delta_maj[16][16][16], delta_xor[16][16][16];
extern int diff_bit_render_to_index[256];

void _init_diff_bit_render_s();

void _init_diff_bit_render_to_index();

void _init_diff_bit_index();

void _init_tighten_matrix();

int config_to_index(const set<pair<bool, bool> > &conditions_intersection);

vector<pair<bool, bool> > index_to_config(int idx);

void _init_DEL_IF_MAJ_XOR_DP();

DEL ROTATE_RIGHT_WORD(DEL in, int offset);

DEL ROTATE_LEFT__WORD(DEL in, int offset);

DEL GET_DEL_IF__WORD(DEL x, DEL y, DEL z);

DEL GET_DEL_MAJ_WORD(DEL x, DEL y, DEL z);

DEL GET_DEL_XOR_WORD(DEL x, DEL y, DEL z);

DEL GET_TIGHTEN_WORD(DEL x1, DEL x2, const int num_bits = WORD_LENGTH);

DEL GET_DEL_SIGMA_0(const DEL&);

DEL GET_DEL_SIGMA_1(const DEL& in);

DEL GET_DEL_SMALL_SIGMA_0(const DEL& in);

DEL GET_DEL_SMALL_SIGMA_1(const DEL &in);

#endif //DIFFERENCE_PROPAGATION_TABLES_H