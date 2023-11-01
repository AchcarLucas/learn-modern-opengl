#include "tests.hpp"
#include "init.hpp"

using namespace std;

const int _WIDTH = 800;
const int _HEIGHT = 600;

#define EXEC_TEST 4

int main()
{
    initOpenGL(_WIDTH, _HEIGHT);
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
        #endif
    #endif
}
