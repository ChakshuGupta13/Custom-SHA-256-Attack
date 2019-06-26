#ifndef SHA256_H
#define SHA256_H

#include "GLOBAL_CONSTANTS.h"
#include "difference_propagation_tables.h"

using namespace std;

extern const WORD K[TOTAL_NUM_ROUNDS];
extern DEL _K[TOTAL_NUM_ROUNDS];

void _init_DEL_K();

#endif //SHA256_H