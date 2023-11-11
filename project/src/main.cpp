#include "tests.hpp"
#include "init.hpp"

using namespace std;

const int _WIDTH = 800;
const int _HEIGHT = 600;

int main()
{
    initOpenGL(_WIDTH, _HEIGHT);
    run(_WIDTH, _HEIGHT);
}
