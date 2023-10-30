#include "tests.hpp"
#include "init.hpp"

using namespace std;

const int _WIDTH = 800;
const int _HEIGHT = 600;

#define EXEC_TEST 2

int main()
{
    initOpenGL(_WIDTH, _HEIGHT);
    #ifdef EXEC_TEST
        #if EXEC_TEST == 0
            return run_default();
        #elif EXEC_TEST == 1
            return run_001();
        #elif EXEC_TEST == 2
            return run_002();
        #endif
    #endif
}
