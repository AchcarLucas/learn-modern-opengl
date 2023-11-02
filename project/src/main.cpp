#include "tests.hpp"
#include "init.hpp"

using namespace std;

const int _WIDTH = 800;
const int _HEIGHT = 600;

// #define ASSIMP_TEST

#ifndef ASSIMP_TEST
    #define EXEC_TEST 11
#endif

int main()
{
    initOpenGL(_WIDTH, _HEIGHT);
    #ifdef ASSIMP_TEST
        return load_assimp_test();
    #endif
    #ifdef EXEC_TEST
        #if EXEC_TEST == 0
            return run_default(_WIDTH, _HEIGHT);
        #elif EXEC_TEST == 1
            return run_001(_WIDTH, _HEIGHT);
        #elif EXEC_TEST == 2
            return run_002(_WIDTH, _HEIGHT);
        #elif EXEC_TEST == 3
            return run_003(_WIDTH, _HEIGHT);
        #elif EXEC_TEST == 4
            return run_004(_WIDTH, _HEIGHT);
        #elif EXEC_TEST == 5
            return run_005(_WIDTH, _HEIGHT);
        #elif EXEC_TEST == 6
            return run_006(_WIDTH, _HEIGHT);
        #elif EXEC_TEST == 7
            return run_007(_WIDTH, _HEIGHT);
        #elif EXEC_TEST == 8
            return run_008(_WIDTH, _HEIGHT);
        #elif EXEC_TEST == 9
            return run_009(_WIDTH, _HEIGHT);
        #elif EXEC_TEST == 10
            return run_010(_WIDTH, _HEIGHT);
        #elif EXEC_TEST == 11
            return run_011(_WIDTH, _HEIGHT);
        #endif
    #endif
}
