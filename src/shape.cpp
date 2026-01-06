#include <vector>
#include <raylib.h>
#include "shape.hpp"
using namespace std;

float s = 1;
vector<Vector3> nodes = {
    {-s, s, s},
    {s, s, s},
    {s, -s, s},
    {-s, -s, s},

    {-s, s, -s},
    {s, s, -s},
    {s, -s, -s},
    {-s, -s, -s},
};

vector<vector<int>> edges = {
    {0, 1, 2, 3},
    {4, 5, 6, 7},
    {0, 4},
    {1, 5},
    {2, 6},
    {3, 7}};