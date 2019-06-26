#ifndef SHA_256_DIFFERENTIAL_ATTACK_AUTO_TOOL_GLOBAL_CONSTANTS_H
#define SHA_256_DIFFERENTIAL_ATTACK_AUTO_TOOL_GLOBAL_CONSTANTS_H

#include <bits/stdc++.h>

#define TOTAL_NUM_ROUNDS 64
#define WORD_LENGTH 32
#define IV_OFFSET 4
#define ROT_LEFT(a, b) (((a) << (b)) | ((a) >> (WORD_LENGTH - (b))))
#define ROT_RIGHT(a, b) (((a) >> (b)) | ((a) << (WORD_LENGTH - (b))))
#define IF(x, y, z) (((x) & (y)) ^ ((~(x)) & (z)))
#define MAJ(x, y, z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define XOR(x, y, z) ((x) ^ (y) ^ (z))
#define access(index) ((index < 0) ? (TOTAL_NUM_ROUNDS - 1 - index) : (index))
#define mapKey(type, step) (vector<int> {type, step})
#define rev(S) reverse(S.begin(), S.end())

using namespace std;

typedef string DEL;
typedef unsigned char BYTE;
typedef unsigned int WORD;
typedef pair<int, pair<int, int> > CARRY_GRAPH_NODE;

#endif //SHA_256_DIFFERENTIAL_ATTACK_AUTO_TOOL_GLOBAL_CONSTANTS_H
